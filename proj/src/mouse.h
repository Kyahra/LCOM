#ifndef _MOUSE_H
#define _MOUSE_H

#include "bitmap.h"

/** @defgroup Mouse
 * @{
 *
 * Mouse struct
 * Functions to manipulate the mouse
 *
 */

typedef struct {
    int x;	/**< mouse x coordinate */
	int y;	/**< mouse y coordinate */

    int packet_idx;		/**< index of the packet being currently read*/
    int complete_packet;	/**< 1 if packet is ready to be read, 0 otherwise*/
    unsigned long packet[3]; 	/**< array containing the 3 mouse packets*/

    int leftButtonDown;		/**< 1 if mouse's left button is being pressed, 0 otherwise*/
    int leftButtonReleased;	/**< 1 if mouse's left button has just been released, 0 otherwise*/


    int draw;		/**< 1 if mouse must be displayed on the screen, 0 otherwise*/
} Mouse;


/**
 * @brief creates a new Mouse instance
 *
 *	Initializes Mouse struct members
 *
 * @return pointer to the Mouse instance created
 */
Mouse * newMouse();

/**
 * @brief return Mouse instance
 *
 *	If there is no mouse instance a new instance is created
 *
 * @return pointer to the Mouse instance created
 */
Mouse* getMouse();

/**
 * @brief updates the mouse instance members
 *
 * Updates the mouse coordinates according to the completed packet
 * Updates information relative to the left button
 *
 *
 */
void updateMouse();

/**
 * @brief draws mouse instance
 *
 * @param cursor pointer to the image to de displayed as the cursor
 *
 */
void drawMouse(Bitmap * cursor);

/**
 * @brief deletes mouse instance
 *
 */
void deleteMouse();

/**
 * @brief reads byte from packet
 *
 * Puts the byte in the respective position on the packet array
 * Checks is the packet is completed
 *
 *
 */
void packet_handler();

/**
 * @brief checks if the mouse is inside the given coordinates
 *
 * @param x1 initial x coordinate
 *
 * @param x2 final x coordinate
 *
 * @param y1 initial y coordinate
 *
 * @param y2 final y coordinate
 *
 * @return 1 if mouse is inside the given coordinates, 0 otherwise
 */
int checkInside(unsigned x1, unsigned x2, unsigned y1, unsigned y2);


/**
 * @brief Subscribes and enables Mouse interrupts
 *
 * @return masked id, negative value upon failure
 */
unsigned mouse_susbcribe_int();

/**
 * @brief unsubscribes Mouse interrupts
 *
 * @return 0 upon success, non-zero otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief enables data report form mouse
 *
 *
 * @return 0 upon success, non-zero otherwise
 */
int enable_data_report();


/**
 * @brief disables data reporting from mouse
 *
 * @return 0 upon success, non-zero otherwise
 */
int disable_data_report();

/**
 * @brief sends mouse command
 *
 * @return 0 upon success, non-zero otherwise
 */
int send_mouse_cmd(unsigned long cmd);

/**
 * @brief reads the byte contained in the output buffer (0x60)
 *
 * @return data read from the buffer
 */
unsigned long read_out_buf();

/**
 * @brief sends command to the kbc
 *
 * @param cmd command to be sent
 *
 * @return 0 upon success, non-zero otherwise
 */
int  send_kbc_cmd(unsigned long cmd);

/**
 * @brief sends command to the beyboard
 *
 * @param cmd command to be sent
 *
 * @return 0 upon success, non-zero otherwise
 */
int send_kbd_cmd(unsigned long cmd);


/**@}*/



#endif /* __MOUSE_H */
