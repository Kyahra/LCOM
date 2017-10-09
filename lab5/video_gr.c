#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <math.h>

#include "vbe.h"
#include "defs.h"
#include "sprite.h"
#include "keyboard.h"
#include "timer.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

void *vg_init(unsigned short mode) {
	struct reg86u r;
	vbe_mode_info_t vmi_p;
	struct mem_range mr;
	unsigned int vram_base; /*VRAM’s physical addresss*/
	unsigned int vram_size; /*VRAM’s size, but you can usethe frame-buffer size, instead*/

	r.u.w.ax = SET_VBE; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = BIT(14) | mode; // set bit 14: linear framebuffer
	r.u.b.intno = BIOS_VID_SERV;

	if (sys_int86(&r) != OK) {
		printf("vg_init: sys_int86() failed \n");
		return NULL;
	}

	if (vbe_get_mode_info(mode, &vmi_p) != 0) {
		printf("vg_init(): vbe_get_mode_info() failed \n");
		return NULL;
	}

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;

	bits_per_pixel = vmi_p.BitsPerPixel;

	vram_base = vmi_p.PhysBasePtr;
	vram_size = h_res * v_res * ceil(bits_per_pixel / 8);

	/*Allow memory mapping*/

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;

	if (OK != sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))
		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/*Map memory*/
	video_mem = vm_map_phys(SELF, (void*) mr.mr_base, vram_size);

	if (video_mem == MAP_FAILED)
		panic("couldn’t map video memory");

	return video_mem;

}

int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VID_SERV; /* BIOS video services */

	reg86.u.b.ah = SET_VID_MODE; /* Set Video Mode function */
	reg86.u.b.al = TEXT_MODE; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

int paint_pixel(unsigned short x, unsigned short y, unsigned long color) {
	int i, j;
	char *p = video_mem;
	unsigned bytes_per_pixel = ceil(bits_per_pixel / 8);

	p = video_mem + y * h_res * bytes_per_pixel + x * bytes_per_pixel;

	*p = (char) color;

}

int draw_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	int i, j;

	if (vg_init(0x105) == NULL)
		return 1;

	for (j = 0; j < size; j++)
		for (i = 0; i < size; i++)
			paint_pixel(x + i, y + j, color);

	return 0;

}

int draw_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	float m, b;

	if (vg_init(0x105) == NULL)
		return 1;

	if (xi > xf) {
		unsigned short x_tmp = xf;
		xf = xi;
		xi = x_tmp;
	}

	if (yi > yf) {
		unsigned short y_tmp = yf;
		yf = yi;
		yi = y_tmp;
	}

	m = (float) (yf - yi) / (xf - xi);

	b = yi - m * xi;

	for (xi; xi <= xf; xi++) {
		yi = b + m * xi;
		paint_pixel(xi, yi, color);
	}

	return 0;

}

int display_sprite(unsigned short xi, unsigned short yi, char *xpm[]) {

	char* pixmap;
	int w, h;
	int i, j;
	unsigned long color;
	int c = 0;

	pixmap = read_xpm(xpm, &w, &h);

	for (i = xi; i < xi + h; i++) {

		for (j = yi; j < yi + w; j++) {

			color = pixmap[c];

			paint_pixel(j, i, color);

			c++;

		}

	}

	return 0;

}

int clear_screen() {

	int i;
	char *ptr;
	unsigned bytes_per_pixel = ceil(bits_per_pixel / 8);

	ptr = video_mem;

	for (i = 0; i < v_res * h_res; i++) {
		*ptr = (char) 0;
		ptr += bytes_per_pixel;
	}

	return 0;
}

int animated_sprite(Sprite *s) {
	int i, j;
	unsigned long color;
	int c = 0;
	int x, y;

	x = (int) s->x;
	y = (int) s->y;

	for (i = x; i < x + s->height; i++) {
		for (j = y; j < y + s->width; j++) {
			color = s->map[c];
			paint_pixel(j, i, color);
			c++;
		}
	}

	if ((s->x + s->xspeed) <( v_res - s->width)) {

		s->x += s->xspeed;
	}
	if ((s->y + s->yspeed) <( h_res - s->height))
		s->y += s->yspeed;

	return 0;

}

int move(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor,
		short delta, unsigned short time) {

	int ipc_status, r;
	message msg;
	int irq_set1 = kbd_subscribe_int();
	int irq_set0 = timer_subscribe_int();
	int stop = 0;
	unsigned int counter = 0;

	if (vg_init(0x105) == NULL)
		return 1;

	float xs, ys;

	Sprite * s;

	s = create_sprite(xpm, video_mem);

	s->x = (float) xi;
	s->y = (float) yi;

	if (hor == 0) {
		s->yspeed = delta / (float) (time * 60);
		s->xspeed = 0;
	} else {
		s->xspeed = delta / (float) (time * 60);
		s->yspeed = 0;
	}

// --------------------------------------------------
// INT
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

// SET GRAPHICS MODE

// -------------------------------------------------------

	while (stop == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set1) {
					stop = kbd_scan();
				}
				if (msg.NOTIFY_ARG & irq_set0) {
					clear_screen();
					animated_sprite(s);
					timer_int_handler(&counter);
					if (counter % 60 == 0) {
						if ((counter / 60) == time)
							stop = 1;
					}
				}

			default:
				break;
			}

		}

	}

	if (kbd_unsubscribe_int() != 0) {
		vg_exit();
		printf("Error - Unsubscribing Keyboard.\n");
		return 1;
	}

	if (empty_OUTBUFF() != 0) {
		vg_exit();
		printf("Error - empty_OUTBUFF() failed.\n");
		return 1;
	}

	if (timer_unsubscribe_int() != 0) {
		vg_exit();
		printf("Error - Unsubscribing Timer 0.\n");
		return 1;
	}

	destroy_sprite(s, video_mem);

	if (vg_exit() != 0)
		return -1;

	return 0;
}

