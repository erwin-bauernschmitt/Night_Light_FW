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
#include <stdio.h>
#include "globals.h"
#include "hardware_defines.h"
#include "state_machine.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	uint32_t time_elapsed;

	switch (GPIO_Pin) {
		case BRIGHTNESS_BTN_Pin:
			time_elapsed = HAL_GetTick() - brightness_btn_time;

			if ((brightness_btn_flag == RELEASED) && (time_elapsed >= 50)) {
				brightness_btn_flag = PRESSED;
				brightness_btn_time = HAL_GetTick();
				printf("brightness button pressed\n");
			}

			else if ((brightness_btn_flag == PRESSED) && (time_elapsed < 50)) {
				printf("brightness button debouncing after press\n");
				break;
			}

			else if ((brightness_btn_flag == PRESSED) && (time_elapsed >= 50)) {
				printf("brightness button released\n");
				if (time_elapsed < 5000) {
					event_flag = POT_1_BUTTON_PRESS;
					printf("short press detected\n");
				}
				else {
					event_flag = POT_1_BUTTON_HOLD;
					printf("long press detected\n");
				}
				brightness_btn_flag = RELEASED;
				brightness_btn_time = HAL_GetTick();
			}

			else if ((brightness_btn_flag == RELEASED) && (time_elapsed < 50)) {
				printf("brightness button debouncing after release\n");
				break;
			}
			break;

		case COLOUR_BTN_Pin:
			/* Same approach as BRIGHTNESS_BTN_Pin case. */
			time_elapsed = HAL_GetTick() - colour_btn_time;

			if ((colour_btn_flag == RELEASED) && (time_elapsed >= 50)) {
				colour_btn_flag = PRESSED;
				colour_btn_time = HAL_GetTick();
			}

			else if ((colour_btn_flag == PRESSED) && (time_elapsed < 50)) {
				break;
			}

			else if ((colour_btn_flag == PRESSED) && (time_elapsed >= 50)) {
				if (time_elapsed < 5000) {
					event_flag = POT_2_BUTTON_PRESS;
				}
				else {
					event_flag = POT_2_BUTTON_HOLD;
				}
				colour_btn_flag = RELEASED;
				colour_btn_time = HAL_GetTick();
			}
			break;

		case SENSITIVITY_BTN_Pin:
			time_elapsed = HAL_GetTick() - sensitivity_btn_time;

			if ((sensitivity_btn_flag == RELEASED) && (time_elapsed >= 50)) {
				sensitivity_btn_flag = PRESSED;
				sensitivity_btn_time = HAL_GetTick();
			}

			else if ((sensitivity_btn_flag == PRESSED) && (time_elapsed < 50)) {
				break;
			}

			else if ((sensitivity_btn_flag == PRESSED) && (time_elapsed >= 50)) {
				if (time_elapsed < 5000) {
					event_flag = POT_3_BUTTON_PRESS;
				}
				else {
					event_flag = POT_3_BUTTON_HOLD;
				}
				sensitivity_btn_flag = RELEASED;
				sensitivity_btn_time = HAL_GetTick();
			}
			break;
	}
}




