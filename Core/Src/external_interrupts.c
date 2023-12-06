/**
 *******************************************************************************
 * @file external_interrupts.c
 * @brief ISRs for external interrupts (buttons).
 *
 * @author Erwin Bauernschmitt
 * @date 5/12/2023
 *******************************************************************************
 */


#include "stm32f3xx_hal.h"
#include <stdint.h>
#include "hardware_defines.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
		case BRIGHTNESS_BTN_Pin:
			break;

		case COLOUR_BTN_Pin:
			break;

		case SENSITIVITY_BTN_Pin:
			break;
	}
}




