#ifndef _LCOM_DEFS_H_
#define _LCOM_DEFS_H_


#define BIT(n) (0x01<<(n))

#define SET_VID_MODE	0x00
#define TEXT_MODE		0x03
#define BIOS_VID_SERV	0x10
#define SET_VBE		 	0x4F02
#define VBE_GET_INFO 	0x4F01
#define VBE_GET_CTRL_INFO 	0x4F00



/* 	Keyboard Defs */
#define KBD_IRQ         1
#define ESC_BCODE    	0x81
#define STAT_REG        0x64
#define OUT_BUFF        0x60
#define OBF             BIT(0)
#define WAIT_KBC        20000



#define TIMER0_IRQ      0


#endif /* _LCOM_DEFS_H */
