/**
 *******************************************************************************
 * @file timers.c
 * @brief Function for reading the pots and updating their moving averages.
 *
 * @author Erwin Bauernschmitt
 * @date 12/12/2023
 *******************************************************************************
 */

#include "stm32f3xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "globals.h"
#include "colour_control.h"
#include "timers.h"
#include "debug_flags.h"

/**
 * @brief Reads the potentiometers and updates their moving averages.
 *
 * @param htim: pointer to the timer instance (TIM2)
 *
 * @return None.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		/* Set flag to indicate reading is in progress. */
		potentiometer_flag = READING_IN_PROGRESS;

		/* Read potentiometer ADC values. */
		uint16_t pot1_adc_value = (uint16_t) HAL_ADC_GetValue(&hadc1);
		uint16_t pot2_adc_value = adc2_dma_buffer[1];
		uint16_t pot3_adc_value = adc2_dma_buffer[0];

		/* Update moving average buffers.*/
		pot1_buffer_sum -= pot1_moving_average_buffer[buffer_index];
		pot2_buffer_sum -= pot2_moving_average_buffer[buffer_index];
		pot3_buffer_sum -= pot3_moving_average_buffer[buffer_index];

		pot1_moving_average_buffer[buffer_index] = pot1_adc_value;
		pot2_moving_average_buffer[buffer_index] = pot2_adc_value;
		pot3_moving_average_buffer[buffer_index] = pot3_adc_value;

		pot1_buffer_sum += pot1_adc_value;
		pot2_buffer_sum += pot2_adc_value;
		pot3_buffer_sum += pot3_adc_value;

		/* Update buffer index. */
		buffer_index = (buffer_index + 1) % MOVING_AVERAGE_SIZE;

		/* Calculate moving averages. */
		pot1_moving_average = pot1_buffer_sum / MOVING_AVERAGE_SIZE;
		pot2_moving_average = pot2_buffer_sum / MOVING_AVERAGE_SIZE;
		pot3_moving_average = pot3_buffer_sum / MOVING_AVERAGE_SIZE;

#ifdef DEBUG_POTS
		printf("POT1: %4u        POT2: %4u        POT3: %4u\n", pot1_moving_average,
				pot2_moving_average, pot3_moving_average);
#endif /* DEBUG_POTS */

		/* Set flag to indicate that new moving averages are available. */
		potentiometer_flag = NEW_READING_READY;
	}
}
