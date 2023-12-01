#include <stdint.h>
#include "stm32f3xx_hal.h"

#define MOVING_AVERAGE_SIZE 5
#define COUNTER_PERIOD 1000

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
