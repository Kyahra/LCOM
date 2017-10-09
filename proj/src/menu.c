#include "menu.h"
#include "video_gr.h"
#include "defs.h"
#include "mouse.h"

Menu* newMenu() {
	Menu* state = (Menu*) malloc(sizeof(Menu));

	state->done = 0;

	state->background = loadBitmap("/home/lcom/proj/images/menu.bmp");
	state->play = loadBitmap("/home/lcom/proj/images/play.bmp");
	state->exit = loadBitmap("/home/lcom/proj/images/exit.bmp");


	return state;
}


void updateMenu(Menu* state, unsigned long scan_code){

	if(scan_code == ESC_CODE){
		state->done =1;
		state->action =EXIT_GAME;
	}

	unsigned x1 = 400;
	unsigned x2= 600;
	unsigned y1 = 111;
	unsigned y2= 191;


	// checks if mouse inside Play Button
	if(checkInside(x1,x2,y1,y2) && getMouse()->leftButtonReleased){
		state->action = PLAY_GAME;
		state->done =1;
		getMouse()->leftButtonReleased =0;

	}

	x1 = 400;
	x2 = 600;
	y1= 211;
	y2= 291;


	// checks if mouse inside Exit Button
	if(checkInside(x1,x2,y1,y2) && getMouse()->leftButtonReleased) {
		state->action = EXIT_GAME;
		state->done =1;
		getMouse()->leftButtonReleased =0;
	}


}


void drawMenu(Menu* state) {
	drawBitmap(state->background, 0, 0);


	drawBitmap(state->play,400, 111);
	drawBitmap(state->exit, 400, 211);



}

void deleteMenu(Menu* state) {

	deleteBitmap(state->play);
	deleteBitmap(state->exit);
	deleteBitmap(state->background);

	free(state);
}
