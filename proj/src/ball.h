#ifndef _BALL_H
#define _BALL_H

#include "bitmap.h"

/** @defgroup Ball
 * @{
 *
 * Ball Struct
 *
 * Functions to manipulate ball animation
 *
 */


typedef struct {
	int x,y;       /**<  ball position  */
	int i;		   /**<  animation iterator*/
	int down;		/**< 1 if ball is in the ground 0 otherwise*/
	int jump;     /**< 1 is ball is jumping, 0 otherwise*/

	Bitmap * balls[4];   /**< array of ball Bitmaps*/

}Ball;

/**
 * @brief creates a new Ball
 *
 *	Loads Bitmaps
 *	Initializes other member of Ball struct
 *
 * @return pointer to the Ball created
 */
Ball* newBall();

/**
 * @brief updates the current Ball Bitmap
 *
 * @param ball pointer to the Ball that is to be updated
 *
 */
void updateBall(Ball* ball);

/**
 * @brief draws Ball in its current position
 *
 * @param ball pointer to the Ball that is to be drawn
 *
 *
 */
void drawBall(Ball* ball);

/**
 * @brief deletes Ball
 *
 * @param ball pointer to the Ball that is to be delete
 *
 *
 */
void deleteBall(Ball* ball);

/**@}*/


#endif/* __BALL_H */
