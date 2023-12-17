#include "stm32f3xx_hal.h"
#include <stdint.h>
#include "globals.h"
#include "state_machine.h"

void calculate_pulse_values(uint16_t *pulse_values) {
	switch (current_state) {
	case STANDBY:
		pulse_values[0] = COUNTER_PERIOD;
		pulse_values[1] = COUNTER_PERIOD;
		pulse_values[2] = COUNTER_PERIOD;
		break;

	case WHITE_LIGHT:
		pulse_values[0] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		pulse_values[1] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		pulse_values[2] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		break;

	case RGB_LIGHT:
		pulse_values[0] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		pulse_values[1] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		pulse_values[2] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		break;

	case POT_CALIBRATION:
		if ((pot_cal_substate == POT_1_LOWER)
				|| (pot_cal_substate == POT_1_UPPER)) {
			pulse_values[0] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
			pulse_values[1] = COUNTER_PERIOD;
			pulse_values[2] = COUNTER_PERIOD;
		} else if ((pot_cal_substate == POT_2_LOWER)
				|| (pot_cal_substate == POT_2_UPPER)) {
			pulse_values[0] = COUNTER_PERIOD;
			pulse_values[1] = (pot2_moving_average * COUNTER_PERIOD / ADC_RES);
			pulse_values[2] = COUNTER_PERIOD;
		} else if ((pot_cal_substate == POT_3_LOWER)
				|| (pot_cal_substate == POT_3_UPPER)) {
			pulse_values[0] = COUNTER_PERIOD;
			pulse_values[1] = COUNTER_PERIOD;
			pulse_values[2] = (pot3_moving_average * COUNTER_PERIOD / ADC_RES);
		} else {
			pulse_values[0] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
			pulse_values[1] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
			pulse_values[2] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		}
		break;

	case LED_CALIBRATION:
		pulse_values[0] = (pot1_moving_average * COUNTER_PERIOD / ADC_RES);
		pulse_values[1] = (pot2_moving_average * COUNTER_PERIOD / ADC_RES);
		pulse_values[2] = (pot3_moving_average * COUNTER_PERIOD / ADC_RES);
		break;

	case AMBIENT_LIGHT_CALIBRATION:
		break;
	}
}

uint16_t clamp(uint16_t value, uint16_t min, uint16_t max) {
	if (value < min) {
		return min;
	} else if (value > max) {
		return max;
	}
	return value;
}

void set_pulse_values(uint16_t *pulse_values) {
	clamp(pulse_values[0], 0, COUNTER_PERIOD);
	clamp(pulse_values[1], 0, COUNTER_PERIOD);
	clamp(pulse_values[2], 0, COUNTER_PERIOD);

	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pulse_values[0]);
	__HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, pulse_values[1]);
	__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse_values[2]);
}

void single_pulse(void) {
	uint16_t off_pulse[3] = { COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD };
	uint16_t on_pulse[3] = { 0, 0, 0 };
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	set_pulse_values(on_pulse);
	HAL_Delay(150);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void double_pulse(void) {
	uint16_t off_pulse[3] = { COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD };
	uint16_t on_pulse[3] = { 0, 0, 0 };
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	set_pulse_values(on_pulse);
	HAL_Delay(150);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	set_pulse_values(on_pulse);
	HAL_Delay(150);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void long_pulse(void) {
	uint16_t off_pulse[3] = { COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD };
	uint16_t on_pulse[3] = { 0, 0, 0 };
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	set_pulse_values(on_pulse);
	HAL_Delay(1000);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
