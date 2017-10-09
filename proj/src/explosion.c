#include "explosion.h"

Explosion* newExplosion() {
	Explosion * explosion = (Explosion*) malloc(sizeof(Explosion));

	int i;
	for (i = 0; i < 10; i++) {
		char filename[40];
		sprintf(filename, "/home/lcom/proj/images/explosion%d.bmp", i + 1);
		explosion->boom[i] = loadBitmap(filename);
	}

	explosion->i = 0;
	explosion->done = 0;

	return explosion;

}

void updateExplosion(Explosion* explosion) {

	explosion->i++;

	if (explosion->i == 9) {
		explosion->done = 1;
		explosion->i = 0;
	}

}

void drawExplosion(Explosion* explosion, int x, int y) {

	drawBitmap(explosion->boom[explosion->i], x, y);
}

void deleteExplosion(Explosion* explosion) {
	int i;
	for (i = 0; i < 10; i++) {
		deleteBitmap(explosion->boom[i]);
	}
	free(explosion);

}
