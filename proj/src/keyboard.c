#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "defs.h"

static int kbd_hook_id = 1;

unsigned kbd_subscribe_int() {

	unsigned tmp_hook_id = kbd_hook_id;

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id)
			!= OK || sys_irqenable(&kbd_hook_id) != OK) {
		printf("kbd_subscribe_int() failed.\n");
		return -1;
	}

	return BIT(tmp_hook_id);

}

int kbd_unsubscribe_int() {

	if (sys_irqrmpolicy(&kbd_hook_id) != OK) {
		printf("kbd_unsubscribe_int() failed.\n");
		return -1;
	}

	return 0;

}
unsigned long kbd_scan(){

	unsigned long code;

	sys_inb(OUT_BUFF, &code);

	return code;

}

int empty_OUTBUFF() {

	unsigned long stat;

	if (sys_inb(STAT_REG, &stat) != OK) {
		printf("empty_OUTBUFF() failed.\n");
		return -1;
	}

	while ((stat & OBF) != 0) {

		if (sys_inb(OUT_BUFF, &stat) != OK) {
			printf("empty_OUTBUFF() failed.\n");
			return -1;
		}

		tickdelay(micros_to_ticks(WAIT_KBC));

		if (sys_inb(STAT_REG, &stat) != OK) {
			printf("empty_OUTBUFF() failed.\n");
			return -1;
		}

	}

	return 0;

}

