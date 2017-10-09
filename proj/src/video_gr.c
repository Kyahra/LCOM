#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>

#include "vbe.h"
#include "defs.h"

/* Private global variables */

static char *buffer_1; /* Process address to which VRAM is mapped */
static char *buffer_2;
static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bytes_per_pixel; /* Number of VRAM bits per pixel */
static unsigned vram_size;

int vg_init(unsigned short mode) {

	struct reg86u r;
	vbe_mode_info_t vmi_p;
	struct mem_range mr;
	unsigned int vram_base; /*VRAM’s physical addresss*/

	r.u.w.ax = SET_VBE; // VBE call, function 02 -- set VBE mode
	r.u.w.bx = BIT(14) | mode; // set bit 14: linear framebuffer
	r.u.b.intno = BIOS_VID_SERV;

	if (sys_int86(&r) != OK) {

		printf("vg_init: sys_int86() failed \n");

		return 1;

	}

	if (vbe_get_mode_info(mode, &vmi_p) != 0) {

		printf("vg_init(): vbe_get_mode_info() failed \n");

		return 1;

	}

	h_res = vmi_p.XResolution;
	v_res = vmi_p.YResolution;
	bytes_per_pixel = ceil(vmi_p.BitsPerPixel / 8);

	printf("bpp: %d\n", bytes_per_pixel);
	printf("hr: %d\n", h_res);
	printf("vr: %d\n", v_res);


	vram_base = vmi_p.PhysBasePtr;

	vram_size = h_res * v_res * bytes_per_pixel;

	/*Allow memory mapping*/

	mr.mr_base = (phys_bytes) vram_base;
	mr.mr_limit = mr.mr_base + vram_size;

	if (OK != sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))

		panic("sys_privctl (ADD_MEM) failed: %d\n", r);

	/*Map memory*/

	buffer_1 = vm_map_phys(SELF, (void*) mr.mr_base, vram_size);

	if (buffer_1 == MAP_FAILED)

		panic("couldn’t map video memory");

	buffer_2 = (char*) malloc(vram_size);



	return 0;

}

int vg_exit() {

	struct reg86u reg86;

	reg86.u.b.intno = BIOS_VID_SERV; /* BIOS video services */
	reg86.u.b.ah = SET_VID_MODE; /* Set Video Mode function */
	reg86.u.b.al = TEXT_MODE; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {

		printf("vg_exit(): sys_int86() failed \n");
		return 1;

	}

	return 0;

}

void copyBuffer(){
	memcpy(buffer_1, buffer_2, vram_size);
}


char * getSecondaryBuffer(){
	return buffer_2;
}

int getVerResolution(){
	return v_res;
}


int getHorResolution(){
	return h_res;
}
