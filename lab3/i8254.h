#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_


#define BIT(n) (0x01<<(n))

#define KBD_IRQ	        1   /**< @brief KBD IRQ line */
#define TIMER0_IRQ      0

/* I/O port addresses */

#define OUT_BUFF        0x60
#define IN_BUFF         0x60


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
#define RESEND 			0xFE
#define ERROR 			0xFC

#define STAT_REG 		0x64
#define PAR_ERR 		BIT(7)
#define TO_ERR        	BIT(6)

#endif /* _LCOM_I8254_H */
