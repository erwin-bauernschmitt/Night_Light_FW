/**
 *******************************************************************************
 * @file globals.h
 * @brief Declaration of global variables.
 *
 * @author Erwin Bauernschmitt
 * @date 3/12/2023
 *******************************************************************************
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "state_machine.h"
#include "timers.h"
#include "external_interrupts.h"

#define MOVING_AVERAGE_SIZE 5	///< Number of potentiometer values averaged.
#define COUNTER_PERIOD 1000		///< Period of PWM timer counters.
#define NUM_DMA_CHANNELS 2		///< Number of ADC channels read with DMA.
#define ADC_RES 4096 			///< Number of distinct possible ADC values.
#define NUM_LEDS 16				///< Number of LEDs.

/* Make NUM_CAL_INCS a multiple of six for even colour sampling. */
#define NUM_CAL_INCS 24			///< Number of increments in calibration.
#define NUM_CAL_SAMPLES 10		///< Number of samples in calibration.

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim15;

extern uint16_t pot1_moving_average_buffer[MOVING_AVERAGE_SIZE];
extern uint16_t pot2_moving_average_buffer[MOVING_AVERAGE_SIZE];
extern uint16_t pot3_moving_average_buffer[MOVING_AVERAGE_SIZE];
extern uint16_t pot1_buffer_sum;
extern uint16_t pot2_buffer_sum;
extern uint16_t pot3_buffer_sum;
extern uint8_t buffer_index;
extern volatile uint16_t pot1_moving_average;
extern volatile uint16_t pot2_moving_average;
extern volatile uint16_t pot3_moving_average;
extern volatile uint16_t adc2_dma_buffer[NUM_DMA_CHANNELS];

volatile extern PotFlag potentiometer_flag;

extern State colour_mode;
extern State previous_state;
extern State current_state;
extern PotCalibrationSubstate pot_cal_substate;
extern LEDCalibrationSubstate led_cal_substate;

volatile extern EventType event_flag;

extern ButtonState brightness_btn_state;
extern ButtonState colour_btn_state;
extern ButtonState sensitivity_btn_state;

extern uint32_t brightness_btn_time;
extern uint32_t colour_btn_time;
extern uint32_t sensitivity_btn_time;

extern uint8_t red_thermal_error_flag;
extern uint8_t green_thermal_error_flag;
extern uint8_t blue_thermal_error_flag;
extern uint16_t red_lod_flag;
extern uint16_t green_lod_flag;
extern uint16_t blue_lod_flag;

extern volatile uint32_t mlux_reading;
extern volatile SensorFlag light_sensor_flag;

extern uint16_t pot1_calibration_buffer[2];
extern uint16_t pot2_calibration_buffer[2];
extern uint16_t pot3_calibration_buffer[2];

extern uint16_t led_calibration_buffer[NUM_LEDS][3];

extern uint32_t brightness_calibration_buffer[1 + (NUM_CAL_INCS + 1) + 1][2];
extern uint32_t white_calibration_buffer[1 + (NUM_CAL_INCS + 1) + 1][2];
extern uint32_t colour_calibration_buffer[1 + (NUM_CAL_INCS + 1) + 1][2];

#endif /* GLOBALS_H */
