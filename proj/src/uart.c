#include "uart.h"


int hookID;
int open1, open2;

char *fifo1;
char *fifo2;

void createFifos(){


	char fifo_1[] = "/tmp/fifo1";
	char fifo_2[] = "/tmp/fifo2";

	mkfifo(fifo_1, 0666);
	mkfifo(fifo_2, 0666);

	fifo1 = fifo_1;
	fifo2 = fifo_2;


}

int serial_subscribe(unsigned short address) {

	hookID = 5;
	createFifos();

	if(address == 1){

		if(sys_irqsetpolicy(IRQ_COM1, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK){

			printf("Failed to subscribe to serial port of COM1! \n");
			return -1;

		}
	}

		else{

			if(address == 2){

				if(sys_irqsetpolicy(IRQ_COM2, IRQ_REENABLE | IRQ_EXCLUSIVE, &hookID) != OK){

							printf("Failed to subscribe to serial port of COM2! \n");
							return -1;

						}

			}

		}

	createFifos();

	return 0;
}

int serial_unsubscribe(unsigned short address){

	if(sys_irqrmpolicy(&hookID)!=OK) return 1;

	disable_FIFOS();
	clear_fifos();

	return 0;

		}


int check_data(int address){

	unsigned long var;
	int res;
	char lsr;

	if(address == 1){

		res = sys_inb(COM1 + SER_LSR_STAT, &var);

	}


	if(address == 2){

		res = sys_inb(COM2 + SER_LSR_STAT, &var);

	}

	lsr = (char) var;
	return ((lsr & IER_DATA_AVAILABLE) & BIT(0));

}

int set_data_to_read(int address){

	unsigned long changeValue, var;
	char lsr;
	int res;

	if(address == 1){

		if(sys_inb(COM1 + SER_LSR_STAT, &var) != OK){

			printf("Error reading LSR  \n");

		}

		lsr = (char) var;

		changeValue = (unsigned long) (lsr ^ IER_DATA_AVAILABLE);

		res = sys_outb(COM1 + SER_LSR_STAT, changeValue);

	}

	if(address == 2){

		if(sys_inb(COM2 + SER_LSR_STAT, &var) != OK){

					printf("Error reading LSR  \n");

				}

				lsr = (char) var;

				changeValue = (unsigned long) (lsr ^ IER_DATA_AVAILABLE);

				res = sys_outb(COM2 + SER_LSR_STAT, changeValue);

		}


	return res;

}



int checkDLAB(int address){

	unsigned long var;
	char dlab;
	int res;

	if(address == 1){

		res = sys_inb(COM1 + SER_LCR, &var);

	}


	if(address == 2){

			res = sys_inb(COM2 + SER_LCR, &var);

		}

	dlab = (char) var;

	return ((dlab & BIT(7)) & BIT(7));
}

int changeDLAB(int address){

	unsigned long changeValue, var;
		char dlab;
		int res;

		if(address == 1){

			if(sys_inb(COM1 + SER_LCR, &var) != OK){

				printf("Error reading LSR status \n");
				return -1;

			}

			dlab = (char) var;

			changeValue = (BIT(7) ^ dlab);

			res = sys_outb(COM1 + SER_IER, changeValue);

		}

		if(address == 2){

				if(sys_inb(COM2 + SER_LCR, &var) != OK){

					printf("Error reading LSR status \n");
					return -1;

				}

				dlab = (char) var;

				changeValue = (BIT(7) ^ dlab);

				res = sys_outb(COM2 + SER_LCR, changeValue);
			}

		return res;

}


int set_rate(int address){

int res;

if(checkDLAB(address) != 1)
{

	if(changeDLAB(address) == -1){

		printf("Error changing dlab bit \n");
	}


}

	if(address == 1){

		if(sys_outb(COM1 + SER_RECEIVER_BUFFER, DLL) != OK){

			printf("error inputing DLL value \n");
		}

		if(sys_outb(COM1 + SER_IER, DLM) != OK){

		printf("error inputing DLM value \n");

		}
}

	if(address == 2){

		if(sys_outb(COM2 + SER_RECEIVER_BUFFER, DLL) != OK){

			printf("error inputing DLL value \n");
		}

		if(sys_outb(COM2 + SER_IER, DLM) != OK){

		printf("error inputing DLM value \n");

		}
}

	res = changeDLAB(address);

return res;

}

int send_Data(int address){

	unsigned long isEmpty;
	int res;


	if(address == 1){

		res = sys_inb(COM1 + SER_IER, &isEmpty);

	}


	if(address == 2){

		res = sys_inb(COM2 + SER_IER, &isEmpty);

	}

	if((isEmpty & BIT(1)) == 1)
	{

		printf("transmitter is empty, nothing to send \n");
		return -1;

	}

	enable_FIFOS();

	return res;

}


void enable_FIFOS(){

	unsigned long enable = 0x01;


	if(sys_outb(COM1 + SER_FCR, enable) != OK){

		printf("Error enabling FIFOS \n");

	}

	if(sys_outb(COM2 + SER_FCR, enable) != OK){

			printf("Error enabling FIFOS \n");

		}

}

void disable_FIFOS(){

	unsigned long disable = 0x00;


	if(sys_outb(COM1 + SER_FCR, disable) != OK){

		printf("Error disabling FIFOS \n");

	}

	if(sys_outb(COM2 + SER_FCR, disable) != OK){

			printf("Error disabling FIFOS \n");

		}

}

int empty_Transmitter(int address){

	unsigned long isEmpty, changeValue;

		int res;


		if(address == 1){

			res = sys_inb(COM1 + SER_IER, &isEmpty);

		}


		if(address == 2){

			res = sys_inb(COM2 + SER_IER, &isEmpty);

		}

		if((isEmpty & BIT(1)) == 1)
		{

			printf("transmitter is empty, nothing to send \n");
			return res;

		}

		changeValue = IER_TRANSMITTER_IS_EMPTY ^ isEmpty;

		if(address == 1){

					res = sys_outb(COM1 + SER_IER, changeValue);

				}


				if(address == 2){

					res = sys_inb(COM2 + SER_IER, &changeValue);

				}

		return res;

}

int open_fifos(int address){


	if(address == 1){

			open1 = open(fifo1, O_WRONLY);
			open2 = open(fifo2, O_RDONLY);

		}

	if(address == 2){

		open1 = open(fifo1, O_RDONLY);
		open2 = open(fifo2, O_WRONLY);

		}

}

int write_to_fifo(int address, char data){

	if(address == 1){

		if(write(open1, data, strlen(data)+1) == -1){

			printf("Error writing to fifo1 \n");

		}

	}

	if(address == 2){

		if(write((char)open2, data, strlen(data)+1) == -1){

			printf("Error writing to fifo2 \n");

		}

		}

	return 0;

}

int read_from_fifo(int address, void *readBuff){

	if(address == 1){

			if(read((char)open2, readBuff, READ_FIFO) == -1){

				printf("Error reading from fifo2 \n");
				return -1;
			}

		}

		if(address == 2){

			if(read(open1, readBuff, READ_FIFO) == -1){

				printf("Error reading from fifo1 \n");
				return -1;
			}

			}

		return 0;

}


void clear_fifos(){

	unsigned long clean_reader = 0x02;
	unsigned long clean_writer = 0x04;

	if(sys_outb(COM1 + SER_FCR, clean_reader) != OK){

			printf("Error cleaning FIFOS \n");

		}

	if(sys_outb(COM1 + SER_FCR, clean_writer) != OK){

				printf("Error cleaning FIFOS \n");

			}

}
