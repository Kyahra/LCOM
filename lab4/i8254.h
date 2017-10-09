#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_


#define BIT(n) (0x01<<(n))

#define KBD_IRQ	        1   /**< @brief KBD IRQ line */
#define TIMER0_IRQ      0
#define MOUSE_IRQ       12

/* I/O port addresses */

#define OUT_BUFF        0x60
#define IN_BUFF         0x60
#define CTRL_REG		0x64
#define KBC				0x64

#define ESC_BCODE 		0x81
#define WAIT_KBC   		20000
#define MSB 			BIT(7)
#define E0              0xE0


// LEDs
#define CAPS 			BIT(2)
#define NUM 			BIT(1)
#define SCROLL 			BIT(0)

#define SWITCH_LED_CMD  0xED
#define OBF             BIT(0)
#define IBF 			BIT(1)

//return values from KBC
#define ACK				0xFA
#define NACK			0xFE
#define ERROR 			0xFC

#define STAT_REG 		0x64
#define PAR_ERR 		BIT(7)
#define TO_ERR        	BIT(6)

#define WRT_B_MOUSE		0xD4
#define SET_SMD			0xEA
#define STAT_REQUEST	0xE9
#define DISABLE_DR		0xF5
#define ENABLE_DR		0xF4
#define ENABLE_MOUSE	0XA8

//1st byte from packet
#define LB 				BIT(0)
#define RB 				BIT(1)
#define MB 				BIT(2)
#define XSIGN 			BIT(4)
#define YSIGN 			BIT(5)
#define XOVF 			BIT(6)
#define YOVF 			BIT(7)

//mouse config
#define REMOTE_MODE		BIT(6)
#define DR				BIT(5)
#define SCALLING 		BIT(4)
#define LB_CONF			BIT(2)
#define MB_CONF 		BIT(1)
#define RB_CONF			BIT(0)


#define FF 0xFF


#endif /* _LCOM_I8254_H */
