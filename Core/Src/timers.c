#include "stm32f3xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "globals.h"
#include "colour_control.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) { // Replace TIMx_INSTANCE with your timer instance
        // Assuming ADC is already running in continuous mode

        // Check if conversion is complete // Wait for a short duration for conversion to be completed
		uint32_t adc_value = HAL_ADC_GetValue(&hadc1);

		// Update moving average buffer
		adc_sum -= adc_buffer[buffer_index];
		adc_buffer[buffer_index] = adc_value;
		adc_sum += adc_value;

		// Update buffer index
		buffer_index = (buffer_index + 1) % MOVING_AVERAGE_SIZE;

		// Calculate moving average
		moving_average = adc_sum / MOVING_AVERAGE_SIZE;

		printf("%u\n", moving_average);

		set_pulse_value();
		// Here you can use 'moving_average' for further processing
	}
}
