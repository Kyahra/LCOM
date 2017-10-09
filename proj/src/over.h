#ifndef _OVER_H
#define _OVER_H


#include "bitmap.h"
#include "explosion.h"

/** @defgroup Menu
 * @{
 *
 * GameOver Struct
 * Functions to manipulate the game over state
 *
 */



typedef enum{
	PLAY_AGAIN, EXIT_OVER
} GameOverAction;


typedef struct
{
	int done;       /**< 1 change state, 0 otherwise*/
	int points;		/**< points won in the previous game*/

	Bitmap* background;		/**< pointer to the background image*/


	GameOverAction action; /**< indicates what to do when the game over state is done*/

} GameOver;


/**
 * @brief creates a new GameOver state
 *
 * Initializes GameOver struct members
 *
 * @param duration total game duration
 *
 *
 * @return pointer to the GameOver state created
 */
GameOver * newGameOver(int duration);

/**
 * @brief updates the GameOver
 *
 * Checks if the player has chosen any option or pressed ESC key
 * If yes, changes the state action according to the palyer's choice and updates member done (=1)
 *
 * @param state pointer to the GameOver to be updated
 *
 * @param scan_code scan code from the last keyboard interrupt
 *
 *
 */
void updateGameOver(GameOver* state,unsigned long scan_code);

/**
 * @brief draws GameOver state
 *
 * @param state pointer to the GameOver to be drawn
 *
 */
void drawGameOver(GameOver* state);

/**
 * @brief deletes GameOver state
 *
 * @param state pointer to the GameOver to be deleted
 *
 */
void deleteGameOver(GameOver* state);


/**@}*/

#endif/* __OVER_H */

