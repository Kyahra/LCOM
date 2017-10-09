#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/syslib.h>
#include <math.h>

#include "test5.h"
#include "vbe.h"
#include "video_gr.h"
#include "lmlib.h"
#include "keyboard.h"
#include "sprite.h"
#include "defs.h"

void *test_init(unsigned short mode, unsigned short delay) {
	char * video_mem;
	vbe_mode_info_t vmi_p;
	video_mem = vg_init(mode);

	if (video_mem == NULL)
		return 1;

	if (vbe_get_mode_info(mode, &vmi_p) != 0) {
		printf("vg_init(): vbe_get_mode_info() failed \n");
		return NULL;
	}

	sleep(delay);  // se tiver tempo alterar para usar o timer

	if (vg_exit() != 0)
		return -1;

	printf("VRAM physical address: 0x%x\n", vmi_p.PhysBasePtr);

	return video_mem;

}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	if (draw_square(x, y, size, color) != 0)
		return 1;

	if (kbd_ESC_key() != 0)
		return -1;

	if (vg_exit() != 0)
		return -1;

	printf("ESC key pressed.\n");

	return 0;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {

	if (draw_line(xi, yi, xf, yf, color) != 0)
		return 1;

	if (kbd_ESC_key() != 0)
		return -1;

	if (vg_exit() != 0)
		return -1;

	printf("ESC key pressed.\n");

	return 0;

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	if (vg_init(0x105) == NULL)
		return 1;

	if (display_sprite(xi, yi, xpm) != 0)
		return 1;

	if (kbd_ESC_key() != 0)
		return -1;

	if (vg_exit() != 0)
		return -1;

	printf("ESC key pressed.\n");

	return 0;

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	if (move(xi, yi, xpm, hor, delta, time) != 0)
		return 1;

	return 0;

}

int test_controller() {

	vbe_ctrl_info_t vmi_p;
	int n = 1;
	mmap_t buf;
	if (vbe_get_ctrl_info(&vmi_p) != 0) {
		printf("test_controller(): vbe_get_crtrl_info() failed \n");
		return NULL;
	}

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	printf("      ||VBE CONTROLLER INFORMATION||\n\n");

	printf("Capabilities:\n");

	if (vmi_p.Capabilities[0] & BIT(0))
		printf("-> DAC width is switchable to 8 bits per primary color.\n");

	else
		printf("-> DAC is fixed width, with 6 bits per primary color.\n");

	if (vmi_p.Capabilities[0] & BIT(1))
		printf("-> Controller is not VGA compatible.\n");

	else
		printf("-> Controller is VGA compatible.\n");

	if (vmi_p.Capabilities[0] & BIT(2))
		printf(
				"-> When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h.\n");

	else
		printf("-> Normal RAMDAC operation.\n");


	printf("\n\n List of Video Modes \n");



	/*uint16_t video_modes = (void *) vmi_p.VideoModePtr;


	while((unsigned short)video_modes != 0xFFFF)
	{

		printf("Video Mode : %u, 0x%X \n");
		n++;
		video_modes = video_modes + 2;

	}


	It doesnt end loop and doesnt change Video Mode...*/

	printf("\n->VRAM memory size: %dKB\n\n\n", vmi_p.TotalMemory * 64);

	printf("\n\n");

	return 0;
}

