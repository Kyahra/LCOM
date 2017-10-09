#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/sysutil.h>
#include "i8254.h"

static int kbd_hook_id = 1;
static unsigned long status = 0; //led inicialmente desligadas

extern unsigned long codeAss;
unsigned long asm_handler();

int kbd_subscribe_int(void) {
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

int kbd_scanIH_Ass(unsigned long *code) {
	static int twoBcodeAss = 0;

	sys_enable_iop(SELF);
	asm_handler();

	*code = codeAss;

	if (twoBcodeAss) {
		twoBcodeAss = 0;
		if (((codeAss) & MSB) == 0)
			return 3;
		else
			return 4;

	}

	if ((codeAss) == E0) {
		twoBcodeAss = 1;
		return 0;
	}

	if (((codeAss) & MSB) == 0)
		return 1;
	else
		return 2;

	return -1;

}

int kbd_scanIH_C(unsigned long *code) {

	static int twoBcode = 0;

	if (sys_inb(OUT_BUFF, code) != OK)
		return -1;

	if (twoBcode) {
		twoBcode = 0;
		if (((*code) & MSB) == 0)
			return 3;
		else
			return 4;

	}

	if ((*code) == E0) {
		twoBcode = 1;
		return 0;
	}

	if (((*code) & MSB) == 0)
		return 1;
	else
		return 2;

	return -1;

}

int send_kbd_cmd(unsigned long cmd) {
	unsigned long stat;
	int i = 0;
	while (i < 8) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;

		if ((stat & IBF) == 0) {
			if (sys_outb(IN_BUFF, cmd) != OK)
				return -1;
			return 0;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
		i++;
	}
	return -1;
}

int empty_out_Buff() {
	unsigned long stat, data;

		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;

		if (stat & OBF) {
			sys_inb(OUT_BUFF, &stat);
			return 0;
		}
}

unsigned long ret_kbc() {
	int i = 0;
	unsigned long data;
	unsigned long stat;

	while (i < 8) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;

		if (stat & OBF) {
			sys_inb(OUT_BUFF, &data);

			if (data != ERROR && data != ACK && data != RESEND)
				data = ERROR;
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
	}
	i++;

}

int toogle_leds(unsigned short led) {

	unsigned long ret;

	// atualiza o estado das leds
	switch (led) {
	case 0:
		if ((status & SCROLL) == 0)
			status = status | SCROLL;
		else
			status = ~(~status | SCROLL);
		break;
	case 1:
		if ((status & NUM) == 0)
			status = status | NUM;
		else
			status = ~(~status | NUM);
		break;
	case 2:
		if ((status & CAPS) == 0)
			status = status | CAPS;
		else
			status = ~(~status | CAPS);
		break;

	default:
		break;

	}

	while (1) {
		if (send_kbd_cmd(SWITCH_LED_CMD) != 0)
			return -1;

		ret = ret_kbc();
		if (ret == -1)
			return 1;

		if (ret == ERROR)
			continue;
		else if (ret == RESEND)
			continue;
		else if (ret == ACK) {
			while (1) {
				if (send_kbd_cmd(status) != 0)
					return -1;

				ret = ret_kbc();
				if (ret == -1)
					return 1;

				if (ret == 1)
					return 1;

				if (ret == ERROR)
					break;
				else if (ret == RESEND)
					continue;
				else if (ret == ACK)
					break;
			}

		}

		if (ret == ERROR)
			continue;
		else
			break;
	}

	return 0;
}
