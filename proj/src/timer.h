#ifndef __TIMER_H
#define __TIMER_H

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timer 0
 */

/**
 * @brief Subscribes and enables Timer 0 interrupts
 *
 * * @return masked id, negative value upon failure
 */

unsigned timer_subscribe_int();

/**
 * @brief Unsubscribes Timer 0 interrupts
 *
 * @return 0 upon success and non-zero otherwise
 */

int timer_unsubscribe_int();

/**
 * @brief Timer 0 interrupt handler
 *
 * Increments counter
 *
 * @param counter pointer to counter
 */

void timer_int_handler(unsigned int *counter);

/**@}*/

#endif /* __TIMER_H */
