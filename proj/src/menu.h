#ifndef _MENU_H
#define _MENU_H


#include "bitmap.h"

/** @defgroup Menu
 * @{
 *
 * Menu Struct
 * Functions to manipulate the menu state
 *
 */


typedef enum{
	EXIT_GAME, PLAY_GAME
} MenuAction;

typedef struct
{
	int done;			 /**< 1 change state, 0 otherwise*/

	Bitmap* background; /**< pointer to the background image*/
	Bitmap* play;		/**< pointer to the play button image*/
	Bitmap* multiplayer;/**< pointer to the multiplayer button image*/
	Bitmap* exit;		/**< pointer to the exit button image*/

	MenuAction action; /**< indicates what to do when the state Menu is done*/

} Menu;


/**
 * @brief creates a new Menu instance
 *
 *	Loads Bitmaps
 *	Initializes other member of Menu struct
 *
 * @return pointer to the Menu created
 */
Menu * newMenu();


/**
 * @brief updates the Menu
 *
 * Checks if the player has chosen any Menu option or pressed ESC key
 * If yes, changes the Menu action according to the palyer's choice and updates member done (=1)
 *
 * @param state pointer to the Menu that is to be updated
 *
 * @param scan_code scan code from the last keyboard interrupt
 *
 */
void updateMenu(Menu* state, unsigned long scan_code);

/**
 * @brief draws Menu
 *
 * @param state pointer to the Menu that is to be drawn
 *
 */
void drawMenu(Menu* state);

/**
 * @brief deletes Menu
 *
 * @param state pointer to the Menu that is to be deleted
 *
 */
void deleteMenu(Menu* state);
/**@}*/

#endif/* __MENU_H */

