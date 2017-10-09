#include <minix/drivers.h>
#include "mouse.h"
#include "i8254.h"
#include <minix/com.h>
#include "timer.h"

int test_packet(unsigned short cnt) {
	int ipc_status, r;
	int irq_set;
	message msg;

	int i = 0;

	irq_set = mouse_subscribe_int();

	if (irq_set == -1) {
		printf("Error: Subscribing mouse\n");
		return -1;
	}

	if (enable_data_report() != 0) {
		printf("Error: Setting Data Report\n");
		return -1;
	}

	irq_set = BIT(irq_set);

	while (i != cnt) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					if (packet_handler()) {
						print_packet();
						i++;
					}

				}
			default:
				break;
			}

		}

	}

	if (disable_data_report() != 0) {
			printf("Error: Disable Data Report\n");
			return -1;
		}

	if (empty_OUTBUFF() != 0) {
		printf("Error: Empty OutBuff\n");
		return -1;
	}


	if (mouse_unsubscribe_int() != 0) {
		printf("Error: Unsubscribing Mouse\n");
		return -1;
	}

	printf("Finish.\n");
	return 0;
}

int test_async(unsigned short idle_time) {
	int ipc_status, r;
	message msg;
	int irq_set_mouse = mouse_subscribe_int();
	int irq_set_timer = timer_subscribe_int();
	int stop = 0;
	unsigned int counter = 0;

	if (irq_set_timer < 0) {
		printf("Error - Subscribing Timer.\n");
		return 1;
	} else {
		irq_set_timer = BIT(irq_set_timer);
	}

	if (irq_set_mouse < 0) {
		printf("Error - Subscribing Mouse.\n");
		return 1;
	} else {
		irq_set_mouse = BIT(irq_set_mouse);
	}

	if (enable_data_report() != 0) {
		printf("Error: Setting Data Report\n");
		return -1;
	}

	while (stop == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					counter = 0;
					if (packet_handler())
						print_packet();

				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					timer_int_handler(&counter);
					if (counter % 60 == 0) {
						if (counter / 60 == idle_time)
							stop = 1;
					}
				}

			default:
				break;
			}

		}

	}

	if (empty_OUTBUFF() != 0) {
		printf("Error - Empty Out Buff.\n");
		return -1;

	}

	if (disable_data_report() != 0) {
		printf("Error: Disable Data Report\n");
		return -1;
	}


	if (mouse_unsubscribe_int() != 0) {
		printf("Error - Unsubscribing Keyboard.\n");
		return 1;
	}


	if (timer_unsubscribe_int() != 0) {
		printf("Error - Unsubscribing Timer 0.\n");
		return 1;
	}

	printf("Finish.\n");
	return 0;

}

int test_config(void) {
	unsigned long ret;
	unsigned char b1, b2, b3;

	if (mouse_subscribe_int() == -1) {
		printf("Error: Subscribing mouse\n");
		return -1;
	}

	if (disable_data_report() != 0) {
		printf("Error: Disable Data Report\n");
		return -1;
	}

	while (ret != ACK) {

		if (send_mouse_cmd(STAT_REQUEST) != 0)
			return -1;
		ret = read_OUTBUF();

	}

	b1 = read_OUTBUF();
	b2 = read_OUTBUF();
	b3 = read_OUTBUF();

	if (b1 == -1 || b2 == -1 || b3 == -1) {
		printf("Error: Reading Mouse Configuration\n");
		return -1;
	}

	if(b2 <0 || b2 >3){
			printf("Error: Reading Mouse Configuration\n");
			return -1;
		}

	if(b3 <10 || b3 >200){
		printf("Error: Reading Mouse Configuration\n");
		return -1;
	}

	printf("\n  ||Mouse Configuration||\n");

	if ((b1 & REMOTE_MODE) != 0)
		printf("Remote (polled) Mode\n");
	else
		printf("Stream Mode\n");

	if((b1 & DR) != 0)
		printf("Data Reporting Enabled\n");
	else
		printf("Data Reporting Disable\n");

	if((b1 & SCALLING) != 0)
		printf("Scalling is 2:1\n");
	else
		printf("Scalling is 1:1\n");


	printf("Resolution: %d\n", b2);

	printf("Sample Rate: %d\n", b3);


	if (mouse_unsubscribe_int() != 0) {
		printf("Error: Unsubscribing Mouse\n");
		return -1;
	}

	if (empty_OUTBUFF() != 0) {
		printf("Error: Empty OutBuff\n");
		return -1;
	}

	printf("\n");
	return 0;

}

int test_gesture( short length) {
	int ipc_status, r;
	int irq_set;
	message msg;
	int stop = 1;

	int i = 0;


	irq_set = mouse_subscribe_int();

	if (irq_set == -1) {
		printf("Error: Subscribing mouse\n");
		return -1;
	}

	if (enable_data_report() != 0) {
		printf("Error: Setting Data Report\n");
		return -1;
	}

	irq_set = BIT(irq_set);


	while (stop) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					if (packet_handler()) {
						print_packet();
						stop = positive_slope(length);
					}

				}
			default:
				break;
			}

		}

	}

	if (empty_OUTBUFF() != 0) {
			printf("Error: Empty OutBuff\n");
			return -1;
		}

	if (disable_data_report() != 0) {
			printf("Error: Disable Data Report\n");
			return -1;
		}

	if (mouse_unsubscribe_int() != 0) {
		printf("Error: Unsubscribing Mouse\n");
		return -1;
	}




	printf("Finish.\n");
	return 0;
}
