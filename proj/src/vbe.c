#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "defs.h"

#define LINEAR_MODEL_BIT 14
#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t buf;
	struct reg86u r;
	if (lm_init() == NULL){

		printf("vbe_get_mode_info():lm_init() failed \n");
		return 1;

	}

	lm_alloc(sizeof(vbe_mode_info_t), &buf);

	r.u.w.ax = VBE_GET_INFO;
	r.u.w.cx = BIT(14) | mode;
	r.u.w.es = PB2BASE(buf.phys);
	r.u.w.di = PB2OFF(buf.phys);
	r.u.b.intno = BIOS_VID_SERV;

	if (sys_int86(&r) != OK)

	{

		printf("vbe_get_mode_info(): sys_int86() failed \n");

		return 1;

	}

	*vmi_p = *(vbe_mode_info_t*) buf.virtual;

	lm_free(&buf);

	return 0;

}
