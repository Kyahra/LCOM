#include "play.h"
#include "video_gr.h"
#include "defs.h"
#include "RTC.h"

Play* newPlay() {
	Play* state = (Play*) malloc(sizeof(Play));

	state->done = 0;

	state->background = loadBitmap("/home/lcom/proj/images/city.bmp");
	state->ground = loadBitmap("/home/lcom/proj/images/ground.bmp");
	state->ball = newBall();


	state->spikes = newObstacle(SPIKES);
	state->fence = newObstacle(FENCE);

	state->background_x = 0;


	state->explosion = newExplosion();
	state->collision = 0;

	state->points = 0;

	return state;
}

void updatePlay(Play* state, unsigned long scan_code, unsigned long counter,
		int duration) {

	int background_w;

	// pauses game, creates explosion, ends game
	if (state->collision) {
		if (counter % 5 == 0)
			updateExplosion(state->explosion);

		if (state->explosion->done) {
			state->done = 1;
			state->action = PLAY_OVER;
		}

		return;
	}

	if (scan_code == ESC_CODE) {
		state->done = 1;
		state->action = PLAY_EXIT;
	}

	// ball update
	if (scan_code == UP_CODE || scan_code == SPACE_BAR_CODE)
		state->ball->jump = 1;

	if (counter % 2 == 0)
		updateBall(state->ball);

	// spikes update
	if (state->points % 20 == 0)
		state->spikes->draw = 1;

	updateObstacle(state->spikes);

	// fence update
	if (state->points % 25 == 0 && state->spikes->actual_x < 750)
		state->fence->draw = 1;

	updateObstacle(state->fence);

	// background update
	background_w = state->background->bitmapInfoHeader.width;

	state->background_x -= 4;

	if (state->background_x + background_w == getHorResolution())
		state->background_x = 0;

	// collision handler

	if (collision(state->ball, state->spikes) || collision(state->ball, state->fence)) {
		state->collision = 1;
		state->ball->jump = 0;
	}

	state->points = duration * 5;

	printf("points: %d\n", state->points);

}


int collision(Ball* ball, Obstacle * obstacle){
	return (obstacle->xi < ball->x+70 && obstacle->xf > ball->x && obstacle->yi < ball->y+70
				&& obstacle->yf > ball->y);
}


void drawPlay(Play* state) {

	drawBitmap(state->background, state->background_x, 0);
	drawBitmap(state->ground, 0, 597);

	drawBall(state->ball);
	drawObstacle(state->spikes);
	drawObstacle(state->fence);

	drawScore(state->points, 950, 25);

	if (state->collision) {
		drawExplosion(state->explosion, state->ball->x-65, state->ball->y-25);
	}

}

void deletePlay(Play* state) {

	deleteBitmap(state->background);
	deleteBitmap(state->ground);

	deleteBall(state->ball);
	deleteObstacle(state->spikes);
	deleteObstacle(state->fence);

	deleteExplosion(state->explosion);

	free(state);
}
