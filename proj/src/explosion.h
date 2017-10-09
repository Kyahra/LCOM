#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "bitmap.h"

/** @defgroup Explosion
 * @{
 *
 * Explosion Struct
 * Functions to manipulate explosions animation
 *
 */


typedef struct {
	int i;			 /**< animation iterator*/
	int done;			 /**< 1 if the array as been completely displayed on the screen 0 otherwise*/

	Bitmap * boom[10];   /**< array of explosion Bitmaps*/


}Explosion;


/**
 * @brief creates a new Explosion
 *
 *	Loads Bitmaps
 *	Initializes other member of Menu struct
 *
 * @return pointer to the Explosion created
 */
Explosion* newExplosion();

/**
 * @brief updates the Explosion animation iterator
 *
 * @param explosion pointer to the Explosion that is to be updated
 *
 */
void updateExplosion(Explosion *explosion);

/**
 * @brief draws Explosion in a certain position
 *
 * @param explosion pointer to the Explosion that is to be drawn
 *
 * @param x x coordinate of the Explosion
 *
 * @param y y coordinate of the Explosion
 *
 */
void drawExplosion(Explosion *explosion, int x, int y);

/**
 * @brief deletes Explosion
 *
 * @param explosion pointer to the Explosion that is to be deleted
 *
 */
void deleteExplosion(Explosion *explosion);

/**@}*/

#endif/* __EXPLOSION_H */
