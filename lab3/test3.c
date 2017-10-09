#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "timer.h"
#include "kbd.h"

int handler(unsigned short ass) {
	int opt;

	unsigned long code;

	if (ass == 0)
		opt = kbd_scanIH_C(&code);
	else {
		opt = kbd_scanIH_Ass(&code);
	}

	switch (opt) {
	case (-1):
		printf("Error reading scancode! \n");
		return 1;

	case (1):
		printf("Makecode: 0x%02x\n", code);
		return 0;

	case (2):
		printf("Breakcode: 0x%02x\n", code);
		if (code == ESC_BCODE)
			return 1;
		return 0;

	case (3):
		printf("Makecode: 0xE0%02x\n", code);
		return 0;

	case (4):
		printf("Breakcode: 0xE0%02x\n", code);

		return 0;

	default:
		return 0; // for 2byte code;
	}

}

int kbd_test_scan(unsigned short ass) {
	int ipc_status, r;
	message msg;
	int irq_set = kbd_subscribe_int();
	int stop;

	if (irq_set < 0) {
		printf("Error - Subscribing Keyboard.\n");
		return 1;
	} else {
		irq_set = BIT(irq_set);
	}

	while (stop == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					stop = handler(ass);

				}
			default:
				break;
			}

		}

	}
	if (kbd_unsubscribe_int() != 0) {
		printf("Error - Unsubscribing Keyboard.\n");
		return 1;
	}

	if (empty_out_Buff() != 0)
		return -1;

	return 0;

}

int kbd_test_leds(unsigned short n, unsigned short *leds) {
	int ipc_status, r;
	message msg;
	int irq_set = timer_subscribe_int();
	int i = 0;
	unsigned int counter = 0;

	if (kbd_subscribe_int() < 0) {
		printf("Error - Subscribing Keyboard.\n");
		return 1;
	}

	if (irq_set < 0) {
		printf("Error - Subscribing Timer 0.\n");
		return 1;
	} else {
		irq_set = BIT(irq_set);
	}

	while (i < n) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					timer_int_handler(&counter);
					if (counter % 60 == 0) {
						if (toogle_leds(leds[i]) != 0) {
							printf("Error - Toogle led %d failed\n", i);
							return 1;
						}

						printf("Led %d toggled\n", leds[i]);
						i++;

					}
				}
				break;
			default:
				break;
			}
		}
	}

	if (timer_unsubscribe_int() != 0) {
		printf("Error - Unsubscribing Timer 0.\n");
		return 1;
	}

	if (kbd_unsubscribe_int() != 0) {
		printf("Error - Unsubscribing Keyboard.\n");
		return 1;
	}

	if (empty_out_Buff() != 0)
		return -1;

	return 0;
}

int kbd_test_timed_scan(unsigned short n) {
	int ipc_status, r;
	message msg;
	int irq_set1 = kbd_subscribe_int();
	int irq_set0 = timer_subscribe_int();
	int stop;
	unsigned int counter = 0;

	if (irq_set0 < 0) {
		printf("Error - Subscribing Timer.\n");
		return 1;
	} else {
		irq_set0 = BIT(irq_set0);
	}

	if (irq_set1 < 0) {
		printf("Error - Subscribing Keyboard.\n");
		return 1;
	} else {
		irq_set1 = BIT(irq_set1);
	}

	while (stop == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set0) {
					timer_int_handler(&counter);
					if (counter % 60 == 0) {
						if (counter / 60 == n)
							stop = 1;
					}
				}
				if (msg.NOTIFY_ARG & irq_set1) {
					counter = 0;
					stop = handler(0);
				}

			default:
				break;
			}

		}

	}
	if (kbd_unsubscribe_int() != 0) {
		printf("Error - Unsubscribing Keyboard.\n");
		return 1;
	}
	if (empty_out_Buff() != 0)
		return -1;

	if (timer_unsubscribe_int() != 0) {
		printf("Error - Unsubscribing Timer 0.\n");
		return 1;
	}
	return 0;

}

