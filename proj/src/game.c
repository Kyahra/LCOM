#include <minix/drivers.h>

#include "game.h"
#include "timer.h"
#include "keyboard.h"
#include "video_gr.h"
#include "menu.h"
#include "defs.h"
#include "play.h"
#include "over.h"
#include "explosion.h"
#include "RTC.h"
#include "uart.h"

Game * startGame() {
	Game* game = (Game*) malloc(sizeof(Game));

	game->kbd_irq = kbd_subscribe_int();
	game->timer_irq = timer_subscribe_int();
	game->mouse_irq = mouse_subscribe_int();
	game->rtc_irq = rtc_subscribe_int();

	vg_init(MODE);

	game->done = 0;
	game->scancode = 0;
	game->counter = 0;
	game->duration = 0;

	game->current_time = newTime();
	game->start_time = newTime();

	game->currentState = MENU;
	game->state = newMenu();

	getMouse();

	int i;
	for (i = 0; i < 10; i++) {
		char filename[50];
		sprintf(filename, "/home/lcom/proj/images/%d.bmp", i);
		digits[i] = loadBitmap(filename);
	}

	return game;
}

void updateGame(Game * game) {
	int ipc_status;
	message msg;

	if (driver_receive(ANY, &msg, &ipc_status) != 0)
		return;

	if (is_ipc_notify(ipc_status)) {

		switch (_ENDPOINT_P(msg.m_source)) {

		case HARDWARE:
			// keyboard
			if (msg.NOTIFY_ARG & game->kbd_irq)
				game->scancode = kbd_scan();
			// mouse
			if (msg.NOTIFY_ARG & game->mouse_irq)
				updateMouse();

			//rtc
			if (msg.NOTIFY_ARG & game->rtc_irq)
				updateTime(game->current_time);

			//timer
			if (msg.NOTIFY_ARG & game->timer_irq) {
				game->counter++;

				if (game->counter == 60000)
					game->counter = 0;

				updateState(game);

				game->scancode = 0;

			}
		default:
			break;
		}
	}

	checkIfStateIsDone(game);
}

void updateState(Game*game) {

	switch (game->currentState) {
	case MENU:
		updateMenu(game->state, game->scancode);
		drawMenu(game->state);
		break;
	case PLAY:
		if (!((Play*) (game->state))->collision)
			game->duration = timeElapsed(game->start_time, game->current_time);

		updatePlay(game->state, game->scancode, game->counter, game->duration);

		drawPlay(game->state);
		break;
	case GAME_OVER:
		updateGameOver(game->state, game->scancode);
		drawGameOver(game->state);
		break;

	}

}

void changeState(Game * game, State newState) {
	deleteState(game);

	game->currentState = newState;

	switch (game->currentState) {
	case MENU:
		getMouse()->draw = 1;
		game->state = newMenu();
		break;
	case PLAY:
		getMouse()->draw = 0;
		*game->start_time = *game->current_time;
		game->state = newPlay();
		break;
	case GAME_OVER:
		getMouse()->draw = 1;
		game->state = newGameOver(game->duration);
		break;
	}

}

void checkIfStateIsDone(Game* game) {
	switch (game->currentState) {
	case MENU:
		if (((Menu*) (game->state))->done) {
			MenuAction action = ((Menu*) (game->state))->action;

			if (action == EXIT_GAME)
				game->done = 1;

			if (action == PLAY_GAME)
				changeState(game, PLAY);

		}
		break;

	case PLAY:
		if (((Play*) (game->state))->done) {
			PlayAction action = ((Play*) (game->state))->action;

			if (action == PLAY_EXIT)
				changeState(game, MENU);

			if (action == PLAY_OVER)
				changeState(game, GAME_OVER);

			break;

		}
	case GAME_OVER:
		if (((GameOver*) (game->state))->done) {
			GameOverAction action = ((GameOver*) (game->state))->action;

			if (action == EXIT_OVER)
				game->done = 1;

			if (action == PLAY_AGAIN)
				changeState(game, PLAY);

			break;

		}
	}
}

void deleteState(Game* game) {

	switch (game->currentState) {
	case MENU:
		deleteMenu(game->state);
		break;
	case PLAY:
		deletePlay(game->state);
		break;
	case GAME_OVER:
		deleteGameOver(game->state);
		break;

	}
}

void endGame(Game* game) {
	vg_exit();

	kbd_unsubscribe_int();
	timer_unsubscribe_int();
	rtc_unsubscribe_int();

	deleteState(game);
	deleteTime(game->current_time);
	deleteTime(game->start_time);

	int i;
	for (i = 0; i < 10; i++)
		deleteBitmap(digits[i]);

	empty_OUTBUFF();

	free(game);
}
