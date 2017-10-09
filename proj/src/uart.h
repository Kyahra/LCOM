#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "defs.h"


#ifndef __UART
#define __UART

#define READ_FIFO 10

#define COM1 0x3F8
#define COM2 0x2F8

#define	IRQ_COM1 4
#define IRQ_COM2 3

#define SER_OVERRUN_ERR BIT(1)
#define SER_PARITY_ERR BIT(2)
#define SER_FRAME_ERR BIT(3)
#define SER_RECEIVER_READY BIT(0)
#define SER_TRANSMITTER_EMPTY BIT(5)

#define SER_LSR_STAT 5
#define SER_RECEIVER_BUFFER 0
#define SER_TX_RDY BIT(5)
#define SER_IIR 2
#define SER_FCR 2
#define SER_IER 1
#define SER_LCR 3

#define DIV 192
#define DLM 0x00
#define DLL 0xC0

#define IER_TRANSMITTER_IS_EMPTY BIT(1)
#define	IER_DATA_AVAILABLE 	BIT(0)
#define IER_RECEIVER_STATUS BIT(2)
#define IER_SLEEP_MODE BIT(4)

#define IIR_PENDING BIT(0)


#define ENABLE_FIFO BIT(0)
#define CLEAR_RECEIVE_FIF0 BIT(1)
#define CLEAR_TRANSMIT_FIFO BIT(2)

#define DLAB BIT(7)




int serial_subscribe(unsigned short address);

int serial_unsubscribe(unsigned short address);

int serial_handler(unsigned short address, char tx, char rx);









#endif
