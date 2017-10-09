#ifndef _RTC_H
#define _RTC_H

#include "bitmap.h"
/** @defgroup RTC
 * @{
 *
 * Time Struct
 * Functions to manipulate the rtc and the time
 *
 */

Bitmap* digits[10];  // array of pointers to digits, from 0 to 9

typedef struct{
	unsigned seconds;  /**< time seconds*/
	unsigned minutes;	/**<time minutes*/
	unsigned hour;		/**<time hour*/

}Time;

/**
 * @brief creates a new Time
 *
 * Initializes Time struct members
 *
 *
 * @return pointer to the Time created
 */
Time* newTime();

/**
 * @brief updates Time to the current time upon an RTC interrupt
 *
 *
 * @param pointer to the Time to be updated
 *
 */
void updateTime(Time * time);


/**
 * @brief deletes Time
 *
 * @param state pointer to the Time to be deleted
 *
 */
void deleteTime(Time* time);

/**
 * @brief time past between 2 Time
 *
 *
 * @param s_time start time
 *
 * @param f_time final time
 *
 * @return time between s_time and f_time
 *
 */
int timeElapsed(Time* s_time, Time* f_time);

/**
 * @brief displays score at the given position
 *
 * @param points score
 *
 * @param x score's x coordinate
 *
 * @param y score's y coordinate
 *
 */
void drawScore(int score, int x, int y);

/**
 * @brief subscribes and enables RTC interrupts
 *
 * @return masked id, negative value upon failure
 */
unsigned rtc_subscribe_int();


/**
 * @brief unsubscribes RTC interrupts
 *
 * @return 0 upon success, non-zero otherwise
 */

int rtc_unsubscribe_int();

/**@}*/

#endif/* __RTC_H */
