#include <string.h>

#include "obstacle.h"

Obstacle* newObstacle(ObstacleType type) {
	Obstacle* obstacle = (Obstacle*) malloc(sizeof(Obstacle));

	obstacle->draw = 0;
	obstacle->actual_x = 1024;

	switch (type) {
	case SPIKES:

		obstacle->image = loadBitmap("/home/lcom/proj/images/spikes.bmp");
		obstacle->actual_y = 570;
		break;
	case FENCE:

		obstacle->image = loadBitmap("/home/lcom/proj/images/gate.bmp");
		obstacle->actual_y = 495;
		break;

	};

	obstacle->xi = 1024;
	obstacle->xf = 1024;
	obstacle->yi = obstacle->actual_y + 10;
	obstacle->yf = obstacle->yi + 200;

	return obstacle;

}

void updateObstacle(Obstacle* obstacle) {
	if (obstacle->actual_x <= -165) {

		obstacle->draw = 0;
		obstacle->actual_x = 1024;
	}

	if (obstacle->draw)
		obstacle->actual_x -= 4;

	obstacle->xi = obstacle->actual_x + 5;
	obstacle->xf = obstacle->xi + obstacle->image->bitmapInfoHeader.width - 5;

}

void drawObstacle(Obstacle* obstacle) {

	if (obstacle->draw)
		drawBitmap(obstacle->image, obstacle->actual_x, obstacle->actual_y);
}

void deleteObstacle(Obstacle* obstacle) {
	deleteBitmap(obstacle->image);
	free(obstacle);
}

