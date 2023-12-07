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


#define MOVING_AVERAGE_SIZE 5	///< Number of potentiometer values averaged.
#define COUNTER_PERIOD 1000		///< Period of PWM timer counters.


extern uint32_t adc_buffer[MOVING_AVERAGE_SIZE];
extern uint32_t adc_sum;
extern uint8_t buffer_index;
extern uint32_t moving_average;


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim15;

extern State colour_mode;
extern State previous_state;
extern State current_state;
extern PotCalibrationSubstate pot_cal_substate;
extern LEDCalibrationSubstate led_cal_substate;
extern LightCalibrationSubstate light_cal_substate;

volatile extern EventType event_flag;

extern GPIO_PinState brightness_btn_state;
extern GPIO_PinState colour_btn_state;
extern GPIO_PinState sensitivity_btn_state;

extern InputFlag brightness_btn_flag;
extern InputFlag colour_btn_flag;
extern InputFlag sensitivity_btn_flag;

extern uint32_t brightness_btn_time;
extern uint32_t colour_btn_time;
extern uint32_t sensitivity_btn_time;


#endif /* GLOBALS_H */
