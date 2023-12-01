#include "stm32f3xx_hal.h"
#include <stdint.h>
#include "globals.h"


void set_pulse_value(void) {
	uint16_t pulseValue = (uint16_t)(moving_average * 1000 / 4096);

	if (pulseValue < 0) {
		pulseValue = 0;
	}
	else if (pulseValue > COUNTER_PERIOD) {
		pulseValue = COUNTER_PERIOD;
	}

	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, pulseValue);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulseValue);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulseValue);
}
