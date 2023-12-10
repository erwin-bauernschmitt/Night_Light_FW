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
#include "external_interrupts.h"


#define DEBOUNCE_TIME 50 ///< Button debounce duration in ms.


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	uint32_t current_time = HAL_GetTick();

	switch (GPIO_Pin) {
		case BRIGHTNESS_BTN_Pin:
			ButtonInfo brightness_btn_info  = {
					&brightness_btn_time,
					&brightness_btn_state,
					POT_1_BUTTON_PRESS,
					POT_1_BUTTON_HOLD,
					&colour_btn_state,
					&sensitivity_btn_state
			};
			handle_button(&brightness_btn_info, current_time);
			break;

		case COLOUR_BTN_Pin:
			ButtonInfo colour_btn_info  = {
					&colour_btn_time,
					&colour_btn_state,
					POT_2_BUTTON_PRESS,
					POT_2_BUTTON_HOLD,
					&brightness_btn_state,
					&sensitivity_btn_state
			};
			handle_button(&colour_btn_info, current_time);
			break;

		case SENSITIVITY_BTN_Pin:
			ButtonInfo sensitivity_btn_info  = {
					&sensitivity_btn_time,
					&sensitivity_btn_state,
					POT_3_BUTTON_PRESS,
					POT_3_BUTTON_HOLD,
					&brightness_btn_state,
					&colour_btn_state
			};
			handle_button(&sensitivity_btn_info, current_time);
			break;
	}
}


void handle_button(ButtonInfo *button, uint32_t current_time) {
    uint32_t time_elapsed = current_time - *(button->last_time);

    if (*(button->state) == INVALID) {
        printf("Button release ignored\n");
        *(button->state) = RELEASED;
        *(button->last_time) = current_time;
    }
    else if (time_elapsed < DEBOUNCE_TIME) {
        printf("Button debouncing\n");
    }
    else if (*(button->state) == RELEASED) {
        *(button->state) = PRESSED;
        *(button->last_time) = current_time;
        printf("Button pressed\n");
    }
    else {
        printf("Button released\n");
        if (time_elapsed < 5000) {
        	event_flag = button->short_press_event;
        	printf("Short press detected\n");
        }
        else {
        	event_flag = button->long_press_event;
        	printf("Long press detected\n");
        }
        *(button->state) = RELEASED;
        *(button->last_time) = current_time;
        // Invalidate other buttons if they are pressed
        if (*(button->other_button1_state) == PRESSED) {
            *(button->other_button1_state) = INVALID;
        }
        if (*(button->other_button2_state) == PRESSED) {
            *(button->other_button2_state) = INVALID;
        }
    }
}


void initialise_button_states(void) {
	brightness_btn_time = HAL_GetTick();
	colour_btn_time = HAL_GetTick();
	sensitivity_btn_time = HAL_GetTick();

	if (HAL_GPIO_ReadPin(BRIGHTNESS_BTN_GPIO_Port, BRIGHTNESS_BTN_Pin) == GPIO_PIN_RESET) {
		brightness_btn_state = RELEASED;
	}
	else {
		brightness_btn_state = INVALID;
	}
	if (HAL_GPIO_ReadPin(COLOUR_BTN_GPIO_Port, COLOUR_BTN_Pin) == GPIO_PIN_RESET) {
		colour_btn_state = RELEASED;
	}
	else {
		colour_btn_state = INVALID;
	}
	if (HAL_GPIO_ReadPin(SENSITIVITY_BTN_GPIO_Port, SENSITIVITY_BTN_Pin) == GPIO_PIN_RESET) {
		sensitivity_btn_state = RELEASED;
	}
	else {
		sensitivity_btn_state = INVALID;
	}
}


