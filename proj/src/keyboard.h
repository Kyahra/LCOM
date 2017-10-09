#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @defgroup Keyboard
 * @{
 *
 * Functions to handle keyboard
 *
 */


/**
 * @brief subscribes and enables keyboard interrupts
 *
 * @return masked id, negative value upon failure
 */
unsigned kbd_subscribe_int();

/**
 * @brief unsubscribes keyboard interrupts
 *
 * @return 0 upon success, non-zero otherwise
 */
int kbd_unsubscribe_int();



/**
 * @brief empties output buffer (port 0x60)
 *
 * @return 0 upon success, non-zero otherwise
 */
int empty_OUTBUFF();


/**
 * @brief reads the scan code from the output buffer
 *
 * @return scan code read
 */
unsigned long kbd_scan();

/**@}*/

#endif/* __KEYBOARD_H */
