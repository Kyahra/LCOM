/**
Creates a new sprite with pixmap "pic", with specified


position (within the screen limits) and speed;

Does not draw the sprite on the screen

Returns NULL on invalid pixmap.
*/

#include <stdlib.h>
#include "sprite.h"

Sprite *create_sprite(char *pic[],char *base)
{
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
	if( sp == NULL )
		return NULL;

	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}

	return sp;
}

void destroy_sprite(Sprite * sp, char *base) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL;     // hopeless: pointer is passed by value
//           should do this @ the caller
}
