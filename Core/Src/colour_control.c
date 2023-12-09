#include "stm32f3xx_hal.h"
#include <stdint.h>
#include "globals.h"



uint16_t clamp(uint16_t value, uint16_t min, uint16_t max) {
    if (value < min) {
        return min;
    }
    else if (value > max) {
        return max;
    }
    return value;
}


void set_pulse_values(uint16_t r_pulse, uint16_t g_pulse, uint16_t b_pulse) {
	clamp(r_pulse, 0, COUNTER_PERIOD);
	clamp(g_pulse, 0, COUNTER_PERIOD);
	clamp(b_pulse, 0, COUNTER_PERIOD);

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, r_pulse);
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, g_pulse);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, b_pulse);
}


void single_pulse(void) {
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(150);
	set_pulse_values(0, 0, 0);
	HAL_Delay(150);
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


void double_pulse(void) {
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(150);
	set_pulse_values(0, 0, 0);
	HAL_Delay(150);
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(150);
	set_pulse_values(0, 0, 0);
	HAL_Delay(150);
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}


void long_pulse(void) {
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(150);
	set_pulse_values(0, 0, 0);
	HAL_Delay(1000);
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
