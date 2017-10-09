#include <minix/drivers.h>

#include "mouse.h"
#include "defs.h"
#include "video_gr.h"

static int mouse_hook_id = 12;
Mouse* mouse = NULL;
static int byte_index = 0;
unsigned char packet[3];

Mouse* newMouse() {
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = 0;
	mouse->y = 0;

	mouse->packet_idx = 0;
	mouse->complete_packet = 0;

	mouse->leftButtonDown = 0;
	mouse->leftButtonReleased = 0;

	mouse->draw =1;

}

Mouse* getMouse() {
	if (!mouse) {
		enable_data_report();
		mouse = newMouse();
	}

	return mouse;
}

void updateMouse() {

	unsigned long b1, b2, b3;

	packet_handler();

	if (mouse->complete_packet) {
		b1 = mouse->packet[0];
		b2 = mouse->packet[1];
		b3 = mouse->packet[2];

		if ((b1 & XSIGN) != 0) {
			b2 = (FF & ~(FF & b2));
			b2 += 1;
			b2 = -b2;
		}

		if ((b1 & YSIGN) != 0) {
			b3 = (FF & ~(FF & b3));
			b3 += 1;
			b3 = -b3;
		}

		mouse->x += b2;
		mouse->y -= b3;

		if (mouse->x >= getHorResolution())
			mouse->x = getHorResolution() - 10;
		else if (mouse->x < 0)
			mouse->x = 0;

		if (mouse->y >= getVerResolution())
			mouse->y = getVerResolution() - 10;
		else if (mouse->y < 0)
			mouse->y = 0;

		if (mouse->packet[0] & LEFT_BUTTON) {

			mouse->leftButtonDown = 1;
			mouse->leftButtonReleased = 0;

		} else if (mouse->leftButtonDown) {

			mouse->leftButtonDown = 0;
			mouse->leftButtonReleased = 1;

		} else {
			mouse->leftButtonDown = 0;
			mouse->leftButtonReleased = 0;
		}



	}



}

void drawMouse(Bitmap * cursor) {
	drawBitmap(cursor, mouse->x, mouse->y);
}

void deleteMouse() {
	free(getMouse());
}

void packet_handler() {

	unsigned long byte;

	sys_inb(OUT_BUFF, &byte);

	switch (mouse->packet_idx) {
	case 0:
		if (byte & BIT(3)) {
			mouse->packet[mouse->packet_idx] = byte;
			mouse->packet_idx = 1;
			mouse->complete_packet = 0;
		}
		break;

	case 1:
		mouse->packet[mouse->packet_idx] = byte;
		mouse->packet_idx = 2;
		mouse->complete_packet = 0;
		break;

	case 2:
		mouse->packet[mouse->packet_idx] = byte;
		mouse->packet_idx = 0;
		mouse->complete_packet = 1;
		break;

	};

}

int checkInside(unsigned x1, unsigned x2, unsigned y1, unsigned y2) {

	if ((mouse->x >= x1) && (mouse->x <= x2))
		if ((mouse->y >= y1) && (mouse->y <= y2))
			return 1;

	return 0;
}

int send_kbc_cmd(unsigned long cmd) {
	unsigned long stat;
	int i = 0;
	while (i < 8) {
		if(sys_inb(STAT_REG, &stat)!= OK) return 1;

		if ((stat & IBF) == 0) {
			if(sys_outb(KBC, cmd)!= OK) return 1;
			return 0 ;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
		i++;
	}

	return 1;

}

unsigned long read_out_buff() {
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

unsigned mouse_subscribe_int() {
	int tmp_hook_id = mouse_hook_id;
	unsigned long ret;

	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,
			&mouse_hook_id) != OK || sys_irqenable(&mouse_hook_id) != OK)
		return -1;

	return BIT(tmp_hook_id);
}

int mouse_unsubscribe_int() {
	if(sys_irqrmpolicy(&mouse_hook_id) !=OK)
		return 1;

	return 0;

}

int send_kbd_cmd(unsigned long cmd) {
	unsigned long stat;
	int i = 0;

	while (i < 8) {
		if(sys_inb(STAT_REG, &stat)!= OK) return 1;

		if ((stat & IBF) == 0) {
			if(sys_outb(OUT_BUFF, cmd)!=OK) return 1;
			return 0;
		}
		tickdelay(micros_to_ticks(WAIT_KBC));
		i++;
	}

	return 1;

}

int send_mouse_cmd(unsigned long cmd) {

	if(send_kbc_cmd(WRT_B_MOUSE)!=0) return 1;
	if(send_kbd_cmd(cmd)!=0) return 1;

	return 0;

}

int disable_data_report() {
	unsigned long ret;

	while (ret != ACK) {

		if(send_mouse_cmd(DISABLE_DR) !=0) return 1;
		ret = read_out_buff();

	}

	return 0;

}

int enable_data_report() {

	unsigned long ret = 0;

	disable_data_report();

	while (ret != ACK) {

		if(send_mouse_cmd(ENABLE_DR)!=0) return 1;
		ret = read_out_buff();

	}

	return 0;

}

