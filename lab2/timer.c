#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

unsigned int counter = 0;
static int hook_id =0;


int timer_set_square(unsigned long timer, unsigned long freq){

		unsigned char conf, LSB4;
		unsigned long ctrlWord;
		unsigned short LSB, MSB;


		MSB = (TIMER_FREQ /freq) >>8;
		LSB = ((TIMER_FREQ /freq) <<8) >>8;

		timer_get_conf(timer,&conf);

		LSB4= (conf << 4)>> 4;

		switch(timer){
			case 0:
				ctrlWord = (TIMER_SEL0|TIMER_LSB_MSB|LSB4);
				break;
			case 1:
				ctrlWord = (TIMER_SEL1|TIMER_LSB_MSB|LSB4);
				break;
			case 2:
				ctrlWord = (TIMER_SEL2|TIMER_LSB_MSB|LSB4);
				break;
			default:
				return 1;
			}

		if(sys_outb(TIMER_CTRL,ctrlWord) != OK){
			printf("Error - Writing to control timer.\n");
			return 1;
			}

		if(sys_outb(TIMER_0 + timer, LSB) != OK){
			printf("Error - Writing to timer's LSB.\n");
			return 1;
				}
		if(sys_outb(TIMER_0 + timer, MSB) != OK){
			printf("Error - Writing to timer's MSB.\n");
			return 1;
			}



	return 0;
}

int timer_subscribe_int(void ) {
	int tmp_hook_id = hook_id;

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&hook_id) != OK || sys_irqenable(&hook_id) != OK) return -1;


	return tmp_hook_id;
}

int timer_unsubscribe_int() {
	if(sys_irqrmpolicy(&hook_id)!=OK ||sys_irqdisable(&hook_id)!=OK) return 1;
	return 0;
}

void timer_int_handler() {
	counter++;
	if(counter %60 ==0)printf("%d sec\n", counter/60); //TIMER0 60 Hz

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	unsigned long rbCmd = TIMER_RB_CMD | TIMER_RB_SEL(timer);

	 	 if(sys_outb(TIMER_CTRL, rbCmd) != OK){
			printf("Error - Writing  Read-Back command.\n");
			return 1;
				}


		unsigned long st_ulong;

		 if(sys_inb(TIMER_0 +timer ,&st_ulong) != OK){
			printf("Error - Reading Timer Configuration.\n");
			return 1;
						}

		*st = (unsigned char)st_ulong;

		return 0;
	
}

int timer_display_conf(unsigned char conf) {
	printf("     || Timer configuration||\n\n");

	   // Out
		printf("Out Pin is ");
		printf("%d\n",(conf & BIT(7) !=0) ? 1:0);

		// Null Count
		printf((conf & BIT(6) !=0)? "Null Count\n" : "Count Available for Reading\n");

		// Type of Access
		printf("Type of Access: ");
		if((conf & TIMER_LSB_MSB) == TIMER_LSB_MSB){
			printf("LSB followed by MSB\n");
		}
		else if(conf & TIMER_LSB !=0){
			printf("LSB\n");
		}
		else{
			printf("MSB\n");
		}

		// Operating Mode
		printf("Operating Mode ");
		unsigned char opMode = (conf >> 1) & (BIT(0) | BIT(1) | BIT(2));
		if(opMode ==6){
			printf("2\n");
		} else if(opMode ==7){
			printf("3\n");
		}else{
			printf("%d\n", opMode);
		}

		// Counting Mode
		printf("Counting Mode: ");
		printf((conf & TIMER_BCD !=0)? "BCD\n": "Binary\n");


		return 0;
}

int timer_test_square(unsigned long freq) {
	
	if(freq <1){
		printf("Error - Invalid Frequency.\n");
	}

	if(timer_set_square(0, freq) != 0)
	{
		printf("Error!\n");
		return 1;

	}

	printf("The rate at which timer 0 generates interrupts as been changed.\n");

	return 0;
}

int timer_test_int(unsigned long time) {
	int ipc_status,r;
	message msg;
	int irq_set = timer_subscribe_int();

	if (irq_set <0){
		printf("Error - Subscribing.\n");
		return 1;
	} else{
		irq_set = BIT(irq_set);
	}


	while(counter/60< time ){
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					timer_int_handler();

				}
	break;
	default:
	break;
	}
	} else {
	}
	}
	
	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char conf;

	if(timer>2 || timer <0){
		printf("Error - Invalid Timer.\n");
		return 1;
	}

		timer_get_conf(timer, &conf);
		timer_display_conf(conf);
		return 0;
}
