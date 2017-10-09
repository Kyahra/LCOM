#include "over.h"
#include "mouse.h"
#include "defs.h"
#include "math.h"
#include "RTC.h"

GameOver * newGameOver(int total_time) {
	GameOver* state = (GameOver*) malloc(sizeof(GameOver));
	int i;

	state->done = 0;
	state->points = total_time * 5;

	state->background = loadBitmap("/home/lcom/proj/images/game_over.bmp");



	return state;
}

void updateGameOver(GameOver* state, unsigned long scan_code) {
	int i;

	if (scan_code == ESC_CODE) {
		state->done = 1;
		state->action = EXIT_OVER;
	}

	unsigned x1 = 360;
	unsigned x2 = 470;
	unsigned y1 = 420;
	unsigned y2 = 460;

	// checks if mouse inside Yes Button
	if (checkInside(x1, x2, y1, y2) && getMouse()->leftButtonReleased) {
		state->action = PLAY_AGAIN;
		state->done = 1;
		getMouse()->leftButtonReleased = 0;

	}

	x1 = 510;
	x2 = 620;
	y1 = 420;
	y2 = 460;

	// checks if mouse inside No Button
	if (checkInside(x1, x2, y1, y2) && getMouse()->leftButtonReleased) {
		state->action = EXIT_OVER;
		state->done = 1;
		getMouse()->leftButtonReleased = 0;
	}

}

void drawGameOver(GameOver* state) {

	drawBitmap(state->background, 0, 0);

	drawScore(state->points,602,317);



}

void deleteGameOver(GameOver* state) {
int i;
	deleteBitmap(state->background);




	free(state);
}
