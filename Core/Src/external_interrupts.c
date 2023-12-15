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


/**
 * @brief EXTI Callback function (handles button presses and driver errors).
 *
 * @param GPIO_Pin: The pin that raised an external interrupt.
 *
 * @return None.
 */
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

		case XERR_G_Pin:
		case XERR_B_Pin:
			determine_led_errors();
			break;

		case INT_Pin:

			break;
	}
}


/**
 * @brief Handles button EXTIs, setting event_flag as necessary.
 *
 * Button presses and releases are debounced. Valid button releases are
 * identified as either long or short presses. Invalid button releases are
 * ignored and debounced. Buttons that are pressed when a valid button release
 * occurs are set to INVALID.
 *
 * @param button: pointer to the relevant button information struct.
 * @param current_time: system time when the interrupt occurred.
 *
 * @return None.
 */
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


/**
 * @brief Initialises the pot button states on start-up.
 *
 * The state of non-pressed buttons are changed to RELEASED. The state of
 * pressed buttons are changed to INVALID so their releases are ignored.
 *
 * @return None.
 */
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


/**
 * @brief Reads the error flags of all three LED drivers.
 *
 * The LED drivers are configured to read TEF (Thermal Error Flag). Then they
 * are configured to read LOD and the LOD status data is clocked out of the
 * driver registers. The function enables and disables TIM2 interrupts to
 * prevent interference with the driver states. The function sets global error
 * flags for the main loop.
 *
 * @return None.
 */
void determine_led_errors(void) {
	/* Temporarily stop reading potentiometers and updating LED pulse values. */
	HAL_TIM_Base_Stop_IT(&htim2);

	/* Configure LED drivers for reading error information. */
	HAL_GPIO_WritePin(MODE_GPIO_Port, MODE_Pin, RESET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);

	/* Mask out LOD flags by setting BLANK pins to HIGH. */
	set_pulse_values(COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD);
	HAL_Delay(1);

	/* Read TEF flags - inverting the values as XERR is active low. */
	red_thermal_error_flag = !HAL_GPIO_ReadPin(XERR_R_GPIO_Port, XERR_R_Pin);
	green_thermal_error_flag = !HAL_GPIO_ReadPin(XERR_G_GPIO_Port, XERR_G_Pin);
	blue_thermal_error_flag = !HAL_GPIO_ReadPin(XERR_B_GPIO_Port, XERR_B_Pin);

	/* Unmask the LOD flags by setting BLANK pins to LOW. */
	set_pulse_values(0, 0, 0);
	HAL_Delay(1);

	/* Latch the LOD data into the internal registers. */
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);
	HAL_Delay(1);

	int i;
	for (i = 0; i < 16; i++) {
		/* Clock the LOD data out of the drivers. */
		red_lod_flag |= (HAL_GPIO_ReadPin(SOUT_R_GPIO_Port, SOUT_R_Pin) << i);
		green_lod_flag |= (HAL_GPIO_ReadPin(SOUT_G_GPIO_Port, SOUT_G_Pin) << i);
		blue_lod_flag |= (HAL_GPIO_ReadPin(SOUT_B_GPIO_Port, SOUT_B_Pin) << i);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, SET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);
		HAL_Delay(1);
	}

	/* Communicate error flags over SWO. */
	printf("\n%u %u %u\n", red_thermal_error_flag,
		   green_thermal_error_flag, blue_thermal_error_flag);
	print_binary(red_lod_flag);
	print_binary(green_lod_flag);
	print_binary(blue_lod_flag);

	/* Resume reading potentiometers and updating LED pulse values. */
	HAL_TIM_Base_Start_IT(&htim2);
}


/**
 * @brief Reads the light sensor's data over I2C,
 *
 * @return None.
 */
//void read_light_sensor_data(void) {
//	uint8_t opt4001_addr = 0x44 << 1;
//	uint8_t register_0 = 0x00;
//}


/**
 * @brief Configures the light sensor's registers over I2C.
 *
 * @return None.
 */
InitStatus initialise_light_sensor(void) {
	HAL_StatusTypeDef result;
	uint8_t opt4001_addr = 0x44 << 1;

	uint8_t reg_10_addr = 0x0A;
	uint8_t reg_10_config[2] = {0b00110010, 0b00110000};
	uint8_t reg_10_confirm[2] = {0, 0};
	/**
	 * Register 0Ah Configuration:
	 *
	 * D15-D15 QWAKE = 0b0 				: Quick wake disabled.
	 * D14-D14 0 = 0b0 					: Fixed value.
	 * D13-D10 RANGE = 0b1100 			: Auto-range light level.
	 * D09-D06 CONVERSION_TIME = 0b1000 : 100ms conversion time.
	 * D05-D04 OPERATING_MODE = 0b11 	: Continuous conversion.
	 * D03-D03 LATCH = 0b0 				: Transparent hysteresis mode.
	 * D02-D02 INT_POL = 0b0 			: INT pin active low.
	 * D01-D00 FAULT_COUNT = 0b00 		: One fault event.
	 */
	result = HAL_I2C_Mem_Write(&hi2c2,
					  	       opt4001_addr,			// Device address.
							   reg_10_addr,				// Register address.
							   I2C_MEMADD_SIZE_8BIT,	// Address size.
							   reg_10_config,			// Data packet pointer.
							   sizeof(reg_10_config),	// Size of data packet.
							   HAL_MAX_DELAY);			// Timeout delay.
	if (result != HAL_OK) {
		printf("Failed to configure OPT4001 Register 10.\n");
		return INIT_FAILED;
	}
	else {
			result = HAL_I2C_Mem_Read(&hi2c2,
									  opt4001_addr,
									  reg_10_addr,
									  I2C_MEMADD_SIZE_8BIT,
									  reg_10_confirm,
									  sizeof(reg_10_confirm),
									  HAL_MAX_DELAY);
			if (result != HAL_OK) {
				printf("Failed to read OPT4001 Register 10.\n ");
				return INIT_FAILED;
			}
			else {
				if (reg_10_confirm[0] == reg_10_config[0] && reg_10_confirm[1] == reg_10_config[1]) {
					printf("OPT4001 Register 10 configured successfully.\n");
				}
				else {
					printf("Failed to configure OPT4001 Register 10 successfully.\n");
					return INIT_FAILED;
				}
			}
	}

	HAL_Delay(1);

	uint8_t reg_11_addr = 0x0B;
	uint8_t reg_11_config[2] = {0b10000000, 0b00010100};
	uint8_t reg_11_confirm[2] = {0, 0};
	/**
	 * Register 0Bh Configuration:
	 *
	 * D15-D05 1024 = 0b10000000000 : Fixed value.
	 * D04-D04 INT_DIR = 0b1		: INT pin configured as output.
	 * D03-D02 INT_CFG = 0b01		: INT pin asserted after each conversion.
	 * D01-D01 0 = 0b0				: Fixed value.
	 * D00-D00 I2C_BURST = 0b0		: I2C burst mode off.
	 */
	result = HAL_I2C_Mem_Write(&hi2c2,
				      	  	   opt4001_addr,
							   reg_11_addr,
							   I2C_MEMADD_SIZE_8BIT,
							   reg_11_config,
							   sizeof(reg_11_config),
							   HAL_MAX_DELAY);
	if (result != HAL_OK) {
		printf("Failed to configure OPT4001 Register 11.\n");
		return INIT_FAILED;
	}
	else {
		result = HAL_I2C_Mem_Read(&hi2c2,
								  opt4001_addr,
								  reg_11_addr,
								  I2C_MEMADD_SIZE_8BIT,
								  reg_11_confirm,
								  sizeof(reg_11_confirm),
								  HAL_MAX_DELAY);
		if (result != HAL_OK) {
			printf("Failed to read OPT4001 Register 11.\n ");
			return INIT_FAILED;
		}
		else {
			if (reg_11_confirm[0] == reg_11_config[0] && reg_11_confirm[1] == reg_11_config[1]) {
				printf("OPT4001 Register 11 configured successfully.\n");
				return INIT_SUCCESSFUL;
			}
			else {
				printf("Failed to configure OPT4001 Register 11 successfully.\n");
				return INIT_FAILED;
			}
		}
	}
}



/**
 * @brief Prints a uint16_t as a binary using printf (over SWO).
 *
 * @param value: The 16-bit value to be printed.
 *
 * @return None.
 */
void print_binary(uint16_t value) {
    for (int i = 15; i >= 0; i--) {
        printf("%c", (value & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

