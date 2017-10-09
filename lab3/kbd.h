#ifndef __KBD_H
#define __KBD_H


/**
 * @brief Subscribes/enables KBD interrupts; Disables IH from Minix
 *
 * @return Returns bit order in interrupt mask; negative value on failure
 */
int kbd_subscribe_int(void );

/**
 * @brief Unsubscribes KBD interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_unsubscribe_int();

/**
 * @brief reads scancodes from KBC output buffer
 *  @param  reference of the code
 *
 *  @return -1 upon error, 1 if 1 byte makecode, 2 if 1 byte breakcode,
 *  3 if 2 byte makecode, 4 if 2 bytes breakcode, 0 when the scancode is 0xE0
 *
 */

int kbd_scanIH_C(unsigned long *code);

int toogle_leds(unsigned short led);

int send_kbd_cmd(unsigned long cmd);

int empty_out_Buff();

unsigned long ret_kbc();


#endif
