#ifndef _PLAY_H
#define _PLAY_H


#include "bitmap.h"
#include "ball.h"
#include "explosion.h"
#include "obstacle.h"

/** @defgroup Menu
 * @{
 * Play Struct
 * Functions to manipulate the play state
 *
 */

typedef enum{
	PLAY_EXIT,PLAY_OVER
} PlayAction;

typedef struct
{
	int done;	 /**< 1 change state, 0 otherwise*/

	Bitmap* background;		/**< pointer to the background image*/
	Bitmap* ground;		/**< pointer to the ground image*/
	Ball* ball;		/**< pointer to the bouncing ball*/

	Obstacle* spikes;	/**< pointer to spikes kind obstacle*/
	Obstacle* fence;	/**< pointer to fence kind obstacle*/

	Explosion * explosion;		/**< pointer to explosion*/

	int background_x;		/**< backgrounds x coordinate*/

	int collision;		/**< 1 if the ball hits the obstacles, 0 otherwise*/

	int points;		/**< current player's score*/

	PlayAction action;	/**< indicates what to do when the play state is done*/

} Play;


/**
 * @brief creates a new Play state
 *
 * Initializes Play struct members
 *
 *
 * @return pointer to the Play state created
 */
Play * newPLay();

/**
 * @brief updates the Play
 *
 * Updates Play members position
 * Checks for collisions
 * Updates jump is Space Bar or Up Key had been pressed
 * In case of collision generates an explosion and updates the state action
 * Updates the scores
 *
 * @param state pointer to the Play  to be updated
 *
 * @param scan_code scan code from the last keyboard interrupt
 *
 * @param counter counter updated by timer interrupts
 *
 * @param duration current game duration
 *
 */
void updatePlay(Play* state, unsigned long scan_code, unsigned long counter, int duration);

/**
 * @brief draws Play state
 *
 * @param state pointer to the Play to be drawn
 *
 */
void drawPlay(Play* state);

/**
 * @brief tests collisions
 *
 * @param state pointer to the states ball
 *
 * @param obstacle obstacle that might have collide with the ball
 *
 * @return 1 if there was a collision, 0 otherwise
 *
 */
int collision(Ball* ball, Obstacle * obstacle);


/**
 * @brief deletes Play state
 *
 * @param state pointer to the Play to be deleted
 *
 */
void deletePlay(Play* state);


/**@}*/

#endif/* __PLAY_H */
