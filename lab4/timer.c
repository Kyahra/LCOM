#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"


static int timer_hook_id =0;



int timer_subscribe_int(void ) {
	int tmp_hook_id = timer_hook_id;

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE,&timer_hook_id) != OK || sys_irqenable(&timer_hook_id) != OK) return -1;


	return tmp_hook_id;
}

int timer_unsubscribe_int() {
	if(sys_irqrmpolicy(&timer_hook_id)!=OK) return 1;
	return 0;
}

void timer_int_handler(unsigned int * counter) {
	(*counter)++;
}

