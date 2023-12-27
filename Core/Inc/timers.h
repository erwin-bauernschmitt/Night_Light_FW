/**
 *******************************************************************************
 * @file timers.h
 * @brief Declarations for timers.c
 *
 * @author Erwin Bauernschmitt (22964301)
 * @date 12/12/2023
 *******************************************************************************
 */

#ifndef TIMERS_H
#define TIMERS_H

#include "stm32f3xx_hal.h"

/**
 * @brief Flags for processing the pot readings.
 */
typedef enum {
	READING_IN_PROGRESS,	///< Used while reading and averaging is occuring.
	NEW_READING_READY,		///< Used when a new moving average is ready.
	WAITING_FOR_READING		///< Used after processing the new moving average.
} PotFlag;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* TIMERS_G */
