#include "ball.h"

Ball* newBall() {
	Ball * ball = (Ball*) malloc(sizeof(Ball));

	int i, j=4;

	for(i=0; i<4; i++){
		char file_name[40];
		sprintf(file_name, "/home/lcom/proj/images/ball%d.bmp", j);
		ball->balls[i] = loadBitmap(file_name);
		j--;
	}

	ball->i = 0;
	ball->down = 0;
	ball->x = 70;
	ball->y = 527;

	return ball;

}

void updateBall(Ball* ball) {
	if (ball->i == 3)
		ball->i = 0;
	else
		ball->i++;

}


void drawBall(Ball* ball){

	if (ball->jump) {
			if (ball->y == 187)
				ball->down = 1;

			if (ball->down)
				ball->y += 10;
			else
				ball->y -= 10;

			if (ball->y == 527) {
				ball->jump = 0;
				ball->down = 0;
			}
		}

	drawBitmap(ball->balls[ball->i], ball->x, ball->y);
}

void deleteBall(Ball* ball) {
	int i;

	for(i =0; i <4; i++)
	deleteBitmap(ball->balls[i]);

	free(ball);

}

