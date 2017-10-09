#ifndef __MOUSE_H
#define __MOUSE_H

unsigned char packet[3];

int mouse_subscribe_int();

int mouse_unsubscribe_int() ;

int kbd_subscribe_int();

int kbd_unsubscribe_int();

int enable_stream_mode();

int send_kbd_cmd(unsigned long cmd);

int send_mouse_cmd(unsigned long cmd);


unsigned long read_OUTBUF();

int enable_data_report();

int disable_data_report();

int packet_handler();

void print_packet();

#endif
