#ifndef _DEFS_H_
#define _DEFS_H_

#define BIT(n) (0x01<<(n))

/*    Graphic Card Defs   */

#define SET_VID_MODE        0x00
#define TEXT_MODE           0x03
#define BIOS_VID_SERV       0x10
#define SET_VBE             0x4F02
#define VBE_GET_INFO        0x4F01
#define VBE_GET_CTRL_INFO   0x4F00
#define GREEN1   0x07
#define GREEN2  0xffffffe0
#define MODE			0x117


/*         Keyboard Defs */

#define KBD_IRQ         1
#define ESC_CODE            0x81
#define UP_CODE		0x48
#define SPACE_BAR_CODE		0x39

/*      KBC Defs */

#define STAT_REG        0x64
#define OUT_BUFF        0x60
#define OBF             BIT(0)
#define IBF 			BIT(1)
#define WAIT_KBC        20000
#define KBC				0x64
#define PAR_ERR 		BIT(7)
#define TO_ERR        	BIT(6)
#define ACK				0xFA



/*        Mouse Defs   */


#define MOUSE_IRQ       12
#define WRT_B_MOUSE		0xD4
#define DISABLE_DR		0xF5
#define ENABLE_DR		0xF4
#define ENABLE_MOUSE	0XA8
#define LEFT_BUTTON 	BIT(0)
#define XSIGN 			BIT(4)
#define YSIGN 			BIT(5)
#define FF 				0xFF

/* 		RTC Defs      */

#define RTC_IRQ			8
#define RTC_ADDR_REG	0x70
#define RTC_DATA_REG	0x71
#define RTC_CONF 		BIT(4)|BIT(1)|BIT(2)
#define REG_A			10
#define REG_B  			11
#define REG_C 			12
#define SECONDS     	0
#define MINUTES 		2
#define HOURS			4



#define TIMER0_IRQ      0



#endif /* _LCOM_DEFS_H */
