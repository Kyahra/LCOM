#ifndef _GAME_H
#define _GAME_H

#include "mouse.h"
#include "RTC.h"

/** @defgroup Game
 * @{
 *
 * This is the "base" of the program.
 * Functions to keep the state updated and to handle interrupts
 *
 */

typedef enum {
	MENU, PLAY, GAME_OVER
}State;

typedef struct {
	int kbd_irq; /**<  keyboard irq */
	int timer_irq; /**< timer irq */
	int mouse_irq; /**< mouse irq */
	int rtc_irq; /**< rtc irq*/

	int done; /**< 1 to exit the program 0 if the game is running*/

	unsigned long scancode; /**< scan code read in the keyboard interrupts*/
	unsigned long counter; /**< counter updated by timer interrupts*/
	int duration; /**< current duration of the game*/

	Time* current_time; /** current hour, minutes and seconds*/
	Time* start_time; /** hour, minutes and seconds of the last game start*/

	State currentState; /** current state*/
	void* state; /** pointer to the current state*/

} Game;

/**
 * @brief creates a new Game
 *
 * Subscribes all the devices used
 * Initializes the video module in 0x117 graphics mode
 *
 * @return pointer to the Game created
 */
Game * startGame();

/**
 * @brief deletes Game
 *
 * Unsubscribes all the devices used
 * Returns to the Minix 3 default text mode
 * Frees memory used
 * Empties output buffer
 *
 * @param game pointer to the Game that is to be deleted
 *
 */
void endGame(Game* game);

/**
 * @brief updates Game and the secondary buffer
 *
 * Deals with devices interrupts, calling the respective handlers
 * Updates the current state based on the information got by the handlers
 * Switches states if necessary
 *
 * @param game pointer to the Game that is to be updated
 *
 */
void updateGame(Game* game);

/**
 * @brief updates Game's Sate
 *
 * Writes on the secondary buffer based on the current game state (after the update)
 *
 * @param game pointer to the Game that is to be updated
 *
 */
void updateState(Game*game);

/**
 * @brief checks if current State is done
 *
 * Changes State if necessary
 *
 * @param game pointer to the Game that is to be updated
 *
 */
void checkIfStateIsDone(Game* game);

/**
 * @brief changes current Game state
 *
 * Updates some Game members in order to prepared them for the new state
 * Frees the memory used by the old state
 *
 * @param game pointer to the Game that is to be updated
 *
 * @param newState new state
 *
 */
void changeState(Game* game, State newState);

/**
 * @brief deletes current Game state
 *
 * @param game pointer to the Game that is to be updated
 *
 */
void deleteState(Game* game);
/**@}*/

#endif/* __GAME_H */
