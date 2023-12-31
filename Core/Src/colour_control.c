#include "stm32f3xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "globals.h"
#include "state_machine.h"
#include "external_interrupts.h"
#include "kelvin_to_rgb.h"

void calculate_pulse_values(uint16_t *pulse_values) {
	switch (current_state) {
	case STANDBY:
		break;

	case WHITE_LIGHT:
		/* Convert POT2 reading to a kelvin value. */
		uint32_t min_kelvin = kelvin_table[0].kelvin;
		uint32_t max_kelvin = kelvin_table[KELVIN_TABLE_LENGTH - 1].kelvin;
		uint32_t kelvin_range = max_kelvin - min_kelvin;
		uint32_t kelvin = max_kelvin
				- (uint32_t) ((uint64_t) pot2_moving_average * kelvin_range
						/ ADC_RES);
		kelvin = (uint16_t) kelvin;

		/* Convert the kelvin value to an RGB pulse vector. */
		KelvinToRGB lower;
		KelvinToRGB higher;
		search_rgb_to_kelvin(kelvin, &lower, &higher);
		pulse_for_kelvin(kelvin, &lower, &higher, pulse_values);

		/* Adjust the RGB pulse vector for brightness */
		pulse_values[0] = pulse_values[0]
				+ (uint16_t) ((uint32_t) pot1_moving_average
						* (COUNTER_PERIOD - pulse_values[0]) / ADC_RES);
		pulse_values[1] = pulse_values[1]
				+ (uint16_t) ((uint32_t) pot1_moving_average
						* (COUNTER_PERIOD - pulse_values[1]) / ADC_RES);
		pulse_values[2] = pulse_values[2]
				+ (uint16_t) ((uint32_t) pot1_moving_average
						* (COUNTER_PERIOD - pulse_values[2]) / ADC_RES);
		break;

	case RGB_LIGHT:
		uint32_t colour = (ADC_RES - 1) - pot2_moving_average;
		uint32_t segment_length = ADC_RES / 6;
		uint32_t segment = colour / segment_length;
		uint32_t segment_position = colour % segment_length;
		uint32_t value = (COUNTER_PERIOD * segment_position) / segment_length;

		/* Calculate the RGB colour vector. */
		switch (segment) {
		case 0:
			pulse_values[0] = COUNTER_PERIOD;
			pulse_values[1] = value;
			pulse_values[2] = 0;
			break;

		case 1:
			pulse_values[0] = (COUNTER_PERIOD - value);
			pulse_values[1] = COUNTER_PERIOD;
			pulse_values[2] = 0;
			break;

		case 2:
			pulse_values[0] = 0;
			pulse_values[1] = COUNTER_PERIOD;
			pulse_values[2] = value;
			break;

		case 3:
			pulse_values[0] = 0;
			pulse_values[1] = (COUNTER_PERIOD - value);
			pulse_values[2] = COUNTER_PERIOD;
			break;

		case 4:
			pulse_values[0] = value;
			pulse_values[1] = 0;
			pulse_values[2] = COUNTER_PERIOD;
			break;

		case 5:
			pulse_values[0] = COUNTER_PERIOD;
			pulse_values[1] = 0;
			pulse_values[2] = (COUNTER_PERIOD - value);
			break;

		case 6:
			pulse_values[0] = COUNTER_PERIOD;
			pulse_values[1] = 0;
			pulse_values[2] = 0;
		}

		/* Invert pulse values as BLANK is active low. */
		pulse_values[0] = COUNTER_PERIOD - pulse_values[0];
		pulse_values[1] = COUNTER_PERIOD - pulse_values[1];
		pulse_values[2] = COUNTER_PERIOD - pulse_values[2];

		/* Adjust the RGB pulse vector for brightness */
		pulse_values[0] = pulse_values[0]
				+ (uint16_t) ((uint32_t) pot1_moving_average
						* (COUNTER_PERIOD - pulse_values[0]) / ADC_RES);
		pulse_values[1] = pulse_values[1]
				+ (uint16_t) ((uint32_t) pot1_moving_average
						* (COUNTER_PERIOD - pulse_values[1]) / ADC_RES);
		pulse_values[2] = pulse_values[2]
				+ (uint16_t) ((uint32_t) pot1_moving_average
						* (COUNTER_PERIOD - pulse_values[2]) / ADC_RES);
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

void red_single_pulse(void) {
	uint16_t off_pulse[3] = { COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD };
	uint16_t on_pulse[3] = { 0, COUNTER_PERIOD, COUNTER_PERIOD };
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	set_pulse_values(on_pulse);
	HAL_Delay(150);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void red_double_pulse(void) {
	uint16_t off_pulse[3] = { COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD };
	uint16_t on_pulse[3] = { 0, COUNTER_PERIOD, COUNTER_PERIOD };
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

void red_long_pulse(void) {
	uint16_t off_pulse[3] = { COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD };
	uint16_t on_pulse[3] = { 0, COUNTER_PERIOD, COUNTER_PERIOD };
	HAL_NVIC_DisableIRQ(TIM2_IRQn);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	set_pulse_values(on_pulse);
	HAL_Delay(1000);
	set_pulse_values(off_pulse);
	HAL_Delay(150);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
