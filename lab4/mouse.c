#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#include "mouse.h"
#include <math.h>

static int mouse_hook_id = 12;
static int kbd_hook_id = 1;
static int byte_index = 0;

static int x = 0, y = 0;

int send_kbc_cmd(unsigned long cmd) {
	unsigned long stat;
	int i = 0;
	while (i < 8) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;

		if ((stat & IBF) == 0) {
			if (sys_outb(KBC, cmd) != OK)
				return -1;
			return 0;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
		i++;
	}
	return -1;

}

unsigned long read_OUTBUF() {
	int i = 0;
	unsigned long data;
	unsigned long stat;

	while (i < 8) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;

		if (stat & OBF) {
			sys_inb(OUT_BUFF, &data);

			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
		i++;
	}
	return -1;

}

int mouse_subscribe_int() {
	int tmp_hook_id = mouse_hook_id;
	unsigned long ret;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&mouse_hook_id) != OK || sys_irqenable(&mouse_hook_id) != OK)
		return -1;

	return tmp_hook_id;
}

int mouse_unsubscribe_int() {
	if (sys_irqrmpolicy(&mouse_hook_id) != OK)
		return 1;
	return 0;
}

int send_kbd_cmd(unsigned long cmd) {
	unsigned long stat;
	int i = 0;
	while (i < 8) {
		if (sys_inb(STAT_REG, &stat) != OK)
			return -1;

		if ((stat & IBF) == 0) {
			if (sys_outb(OUT_BUFF, cmd) != OK)
				return -1;
			return 0;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
		i++;
	}
	return -1;

}

int send_mouse_cmd(unsigned long cmd) {

	unsigned long stat;
	int i = 0;

	if (send_kbc_cmd(WRT_B_MOUSE) != 0)
		return -1;

	if (send_kbd_cmd(cmd) != 0)
		return -1;

	return 0;

}

int disable_data_report() {
	unsigned long ret;

	while (ret != ACK) {

		if (send_mouse_cmd(DISABLE_DR) != 0)
			return -1;
		ret = read_OUTBUF();

	}

	return 0;
}

int enable_data_report() {

	unsigned long ret = 0;

	if (disable_data_report() != 0)
		return -1;

	while (ret != ACK) {

		if (send_mouse_cmd(ENABLE_DR) != 0)
			return -1;
		ret = read_OUTBUF();

	}

	return 0;

}

int packet_handler() {

	unsigned long byte_long;
	unsigned char byte_char;

	if (sys_inb(OUT_BUFF, &byte_long) != OK) {
		return 1;
	}

	byte_char = (unsigned char) byte_long;

	if (byte_index == 0) {
		if (byte_char & BIT(3)) {
			packet[byte_index] = byte_char;
			byte_index = 1;
		}
		return 0;
	}

	packet[byte_index] = byte_char;

	if (byte_index == 1) {
		byte_index++;
		return 0;
	} else if (byte_index == 2) {
		byte_index = 0;
		return 1;
	}

	return 0;

}

void print_packet() {
	printf("B1=0x%02x ", packet[0]);
	printf("B2=0x%02x ", packet[1]);
	printf("B3=0x%02x ", packet[2]);

	if ((packet[0] & LB) != 0)
		printf("LB=1 ");
	else
		printf("LB=0 ");

	if ((packet[0] & RB) != 0)
		printf("RB=1 ");
	else
		printf("RB=0 ");

	if ((packet[0] & MB) != 0)
		printf("MB=1 ");
	else
		printf("MB=0 ");

	if ((packet[0] & XOVF) != 0)
		printf("XOV=1 ");
	else
		printf("XOV=0 ");

	if ((packet[0] & YOVF) != 0)
		printf("YOV=1 ");
	else
		printf("YOV=0 ");

	int delta_x, delta_y;

	delta_x = packet[1];
	delta_y = packet[2];

	printf("X=");
	if ((packet[0] & XSIGN) != 0) {
		delta_x = (FF & ~(FF & delta_x));
		delta_x += 1;
		delta_x = -delta_x;
	}
	printf("%d ", delta_x);

	printf("Y=");
	if ((packet[0] & YSIGN) != 0) {
		delta_y = (FF & ~(FF & delta_y));
		delta_y += 1;
		delta_y = -delta_y;
	}
	printf("%d \n \n", delta_y);

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
}

int positive_slope(short length) {

	int b1, b2, b3;

	b1 = packet[0];
	b2 = packet[1];
	b3 = packet[2];



	if ((b1 & RB) == 0 ||(b1 & YOVF) != 0 ) {
		x = 0;
		y = 0;
		return 1;
	}

	if ((b1 & XSIGN) != 0) {
		b2 = (FF & ~(FF & b2));
		b2 += 1;
		b2= -b2;
	}

	if ((b1 & YSIGN) != 0) {
		b3 = (FF & ~(FF & b3));
		b3+= 1;
		b3 = -b3;
	}


	if (length > 0) {
		if ((b1 & XSIGN) != 0 || (b1 & YSIGN) != 0) {
			x = 0;
			y = 0;
			return 1;
		}
	}

	if (length < 0) {
		if ((b1 & XSIGN) == 0 || (b1 & YSIGN) == 0) {
			x = 0;
			y = 0;
			return 1;
		}
	}

	x += b2;
	y += b3;


	if (abs(x) > 0 && abs(y) > abs(length))
		return 0;


	return 1;

}

