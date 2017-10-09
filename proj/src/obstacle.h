#ifndef _OBSTACLE_H
#define _OBSTACLE_H

#include "bitmap.h"


/** @defgroup Obstacle
 * @{
 *
 * Obstacle struct
 * Functions to manipulate obstacles
 *
 */


typedef enum{
	SPIKES, FENCE
} ObstacleType;

typedef struct {
	Bitmap * image;  /**< pointer to the obstacle image*/

	int draw;		/**<1 if obstacle is to be drawn, 0 otherwise*/

	int actual_x;    /**< obstacles x coordinate*/
	int actual_y;		/**< obstacles y coordinate*/


	int xi;	/**< initial x coordinate to test collision*/
	int xf;	/**< final x coordinate to test collision*/
	int yi; /**< initial y coordinate to test collision*/
	int yf; /**< final y coordinate to test collision*/

	ObstacleType type;


}Obstacle;


/**
 * @brief creates a new Obstacle
 *
 * Initializes Obstacle struct members
 *
 * @param type the Obstacle image and collision coordinates depend on this parameter
 *
 *
 * @return pointer to the Obstacle created
 */
Obstacle* newObstacle(ObstacleType type);

/**
 * @brief updates Obstacle position
 *
 * Is also responsible to update the collision coordinates
 *
 * @param pointer to the Obstacle to be updated
 *
 */
void updateObstacle(Obstacle* obstacle);

/**
 * @brief draws obstacle if draw member is 1
 *
 * @param pointer to the Obstacle to be drawn
 *
 */
void drawObstacle(Obstacle* obstacle);

/**
 * @brief deletes Obstacle
 *
 * @param pointer to the Obstacle to be deleted
 *
 */
void deleteObstacle(Obstacle* obstacle);

/**@}*/



#endif /* __OBSTACLE_H */
