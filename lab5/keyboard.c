#include <minix/syslib.h>
#include <minix/drivers.h>

#include "keyboard.h"
#include "defs.h"

static int kbd_hook_id = 1;

int kbd_subscribe_int() {
	int tmp_hook_id = kbd_hook_id;

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)
			!= OK || sys_irqenable(&kbd_hook_id) != OK)
		return -1;

	return tmp_hook_id;
}

int kbd_unsubscribe_int() {
	if (sys_irqrmpolicy(&kbd_hook_id) != OK)
		return 1;
	return 0;
}

int kbd_scan() {

	unsigned long code;

	if (sys_inb(OUT_BUFF, &code) != OK)
		;

	if (code == ESC_BCODE)
		return 1;

	return 0;

}

int empty_OUTBUFF() {

	unsigned long stat;

	if (sys_inb(STAT_REG, &stat) != OK)

		return -1;

	while ((stat & OBF) != 0) {

		if (sys_inb(OUT_BUFF, &stat) != OK)

			return -1;

		tickdelay(micros_to_ticks(WAIT_KBC));

		if (sys_inb(STAT_REG, &stat) != OK)

			return -1;

	}

	return 0;

}

int kbd_ESC_key() {
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
					stop = kbd_scan();

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

	if (empty_OUTBUFF() != 0){
		printf("Error - empty_OUTBUFF() failed.\n");
		return 1;
	}
	return 0;

}
