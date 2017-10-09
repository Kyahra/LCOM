#include "bitmap.h"
#include "game.h"
#include "mouse.h"
#include "video_gr.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include "uart.h"


static char* video_mem;

int main() {

	Bitmap * cursor = loadBitmap("/home/lcom/proj/images/mouse.bmp");

	sef_startup();


	Game * game = (Game *) startGame();



	while (!game->done) {
		updateGame(game);

		if(getMouse()->draw == 1)
		drawMouse(cursor);

		copyBuffer();
	}

	deleteMouse();

	endGame(game);



}
