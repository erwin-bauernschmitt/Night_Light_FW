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
#include "debug_flags.h"

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
	uint8_t button_number;

	switch (GPIO_Pin) {
	case BRIGHTNESS_BTN_Pin:
		button_number = 1;
		ButtonInfo brightness_btn_info = { button_number, &brightness_btn_time,
				&brightness_btn_state, POT_1_BUTTON_PRESS, POT_1_BUTTON_HOLD,
				&colour_btn_state, &sensitivity_btn_state };
		handle_button(&brightness_btn_info, current_time);
		break;

	case COLOUR_BTN_Pin:
		button_number = 2;
		ButtonInfo colour_btn_info = { button_number, &colour_btn_time,
				&colour_btn_state, POT_2_BUTTON_PRESS, POT_2_BUTTON_HOLD,
				&brightness_btn_state, &sensitivity_btn_state };
		handle_button(&colour_btn_info, current_time);
		break;

	case SENSITIVITY_BTN_Pin:
		button_number = 3;
		ButtonInfo sensitivity_btn_info = { button_number,
				&sensitivity_btn_time, &sensitivity_btn_state,
				POT_3_BUTTON_PRESS, POT_3_BUTTON_HOLD, &brightness_btn_state,
				&colour_btn_state };
		handle_button(&sensitivity_btn_info, current_time);
		break;

	case XERR_G_Pin:
	case XERR_B_Pin:
		determine_led_errors();
		break;

	case INT_Pin:
		if (read_light_sensor_data() != READ_SUCCESSFUL) {
#ifdef DEBUG_LIGHT_SENSOR
			printf("LIGHT SENSOR READ FAILED\n");
#endif /* DEBUG_LIGHT_SENSOR */
		}
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
#ifdef DEBUG_BUTTONS
		printf("Button %u release ignored\n", (button->button_number));
#endif /* DEBUG_BUTTONS */
		*(button->state) = RELEASED;
		*(button->last_time) = current_time;
	} else if (time_elapsed < DEBOUNCE_TIME) {
#ifdef DEBUG_BUTTONS
		printf("Button %u debouncing\n", (button->button_number));
#endif /* DEBUG_BUTTONS */
	} else if (*(button->state) == RELEASED) {
		*(button->state) = PRESSED;
		*(button->last_time) = current_time;
#ifdef DEBUG_BUTTONS
		printf("Button %u pressed\n", (button->button_number));
#endif /* DEBUG_BUTTONS */
	} else {
#ifdef DEBUG_BUTTONS
		printf("Button %u released ", (button->button_number));
#endif /* DEBUG_BUTTONS */
		if (time_elapsed < 5000) {
			event_flag = button->short_press_event;
#ifdef DEBUG_BUTTONS
			printf("(short press detected)\n");
#endif /* DEBUG_BUTTONS */
#ifdef DEBUG_STATE_MACHINE
			printf("\nEvent: POT_%u_BUTTON_PRESS\n", (button->button_number));
#endif /* DEBUG_STATE_MACHINE */
		} else {
			event_flag = button->long_press_event;
#ifdef DEBUG_BUTTONS
			printf("(long press detected)\n");
#endif /* DEBUG_BUTTONS */
#ifdef DEBUG_STATE_MACHINE
			printf("\nEvent: POT_%u_BUTTON_HOLD\n", (button->button_number));
#endif /* DEBUG_STATE_MACHINE */
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
#ifdef DEBUG_INIT
	printf("\nINITIALISING BUTTONS\n");
#endif /* DEBUG_INIT */
	brightness_btn_time = HAL_GetTick();
	colour_btn_time = HAL_GetTick();
	sensitivity_btn_time = HAL_GetTick();

	if (HAL_GPIO_ReadPin(BRIGHTNESS_BTN_GPIO_Port, BRIGHTNESS_BTN_Pin)
			== GPIO_PIN_RESET) {
		brightness_btn_state = RELEASED;
#ifdef DEBUG_INIT
		printf("Button 1: unpressed (state set to RELEASED)\n");
#endif /* DEBUG_INIT */
	} else {
		brightness_btn_state = INVALID;
#ifdef DEBUG_INIT
		printf("Button 1: pressed (state set to INVALID)\n");
#endif /* DEBUG_INIT */
	}
	if (HAL_GPIO_ReadPin(COLOUR_BTN_GPIO_Port, COLOUR_BTN_Pin)
			== GPIO_PIN_RESET) {
		colour_btn_state = RELEASED;
#ifdef DEBUG_INIT
		printf("Button 2: unpressed (state set to RELEASED)\n");
#endif /* DEBUG_INIT */
	} else {
		colour_btn_state = INVALID;
#ifdef DEBUG_INIT
		printf("Button 2: pressed (state set to INVALID)\n");
#endif /* DEBUG_INIT */
	}
	if (HAL_GPIO_ReadPin(SENSITIVITY_BTN_GPIO_Port, SENSITIVITY_BTN_Pin)
			== GPIO_PIN_RESET) {
		sensitivity_btn_state = RELEASED;
#ifdef DEBUG_INIT
		printf("Button 3: unpressed (state set to RELEASED)\n");
#endif /* DEBUG_INIT */
	} else {
		sensitivity_btn_state = INVALID;
#ifdef DEBUG_INIT
		printf("Button 3: pressed (state set to INVALID)\n");
#endif /* DEBUG_INIT */
	}
#ifdef DEBUG_INIT
	printf("BUTTON INITIALISATION SUCCESSFUL\n");
#endif /* DEBUG_INIT */
}

/**
 * @brief Reads the error flags of all three LED drivers.
 *
 * The LED drivers are configured to read TEF (Thermal Error Flag). Then they
 * are configured to read LOD and the LOD status data is clocked out of the
 * driver registers. The function sets global error flags for the main loop.
 *
 * @return None.
 */
void determine_led_errors(void) {
	/* Configure LED drivers for reading error information. */
	HAL_GPIO_WritePin(MODE_GPIO_Port, MODE_Pin, RESET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);

	/* Mask out LOD flags by setting BLANK pins to HIGH. */
	uint16_t off_pulse[3] = { COUNTER_PERIOD, COUNTER_PERIOD, COUNTER_PERIOD };
	set_pulse_values(off_pulse);

	/* Read TEF flags - inverting the values as XERR is active low. */
	red_thermal_error_flag = !HAL_GPIO_ReadPin(XERR_R_GPIO_Port, XERR_R_Pin);
	green_thermal_error_flag = !HAL_GPIO_ReadPin(XERR_G_GPIO_Port, XERR_G_Pin);
	blue_thermal_error_flag = !HAL_GPIO_ReadPin(XERR_B_GPIO_Port, XERR_B_Pin);

	/* Unmask the LOD flags by setting BLANK pins to LOW. */
	uint16_t on_pulse[3] = { 0, 0, 0 };
	set_pulse_values(on_pulse);

	/* Latch the LOD data into the internal registers. */
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, SET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);

	int i;
	for (i = 0; i < 16; i++) {
		/* Clock the LOD data out of the drivers. */
		red_lod_flag |= (HAL_GPIO_ReadPin(SOUT_R_GPIO_Port, SOUT_R_Pin) << i);
		green_lod_flag |= (HAL_GPIO_ReadPin(SOUT_G_GPIO_Port, SOUT_G_Pin) << i);
		blue_lod_flag |= (HAL_GPIO_ReadPin(SOUT_B_GPIO_Port, SOUT_B_Pin) << i);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, SET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);
	}

	/* Latch the configuration registers. */
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, SET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);

#ifdef DEBUG_LED_DRIVERS
	/* Communicate error flags over SWO. */
	printf("\nThermal Error Flags\n");
	printf("R: %u\n", red_thermal_error_flag);
	printf("G: %u\n", green_thermal_error_flag);
	printf("B: %u\n", blue_thermal_error_flag);
	printf("\nLOD Status Data\n");
	printf("R: ");
	print_binary(red_lod_flag);
	printf("G: ");
	print_binary(green_lod_flag);
	printf("B: ");
	print_binary(blue_lod_flag);
#endif /* DEBUG_LED_DRIVERS */
}

/**
 * @brief Reads the light sensor's data over I2C, saving result in mlux_reading.
 *
 * @return None.
 */
ReadStatus read_light_sensor_data(void) {
	/* Set status flag to signal that a read is in progress. */
	light_sensor_flag = IN_PROGRESS;
	HAL_StatusTypeDef result;
	uint8_t opt4001_addr = 0x44 << 1;

	/**
	 * Register 00h Contents:
	 *
	 * D15-D12 EXPONENT		: Exponent value (0-8)
	 * D11-D00 RESULT_MSB	: 12 MSBs of the 20-bit mantissa.
	 */
	uint8_t reg_0_addr = 0x00;
	uint8_t reg_0_data[2] = { 0, 0 };

	result = HAL_I2C_Mem_Read(&hi2c2, opt4001_addr, reg_0_addr,
	I2C_MEMADD_SIZE_8BIT, reg_0_data, sizeof(reg_0_data),
	HAL_MAX_DELAY);
	if (result != HAL_OK) {
#ifdef DEBUG_LIGHT_SENSOR
		printf("Failed to read OPT4001 Register 0.\n");
#endif /* DEBUG_LIGHT_SENSOR */
		light_sensor_flag = WAITING;
		return READ_FAILED;
	}

	/**
	 * Register 01h Contents:
	 *
	 * D15-D08 RESULT_LSB	: 8 LSBs of the 20-bit mantissa.
	 * D07-D04 COUNTER		: Rolling sample counter.
	 * D03-D00 CRC			: Cyclic redundancy check bits.
	 */
	uint8_t reg_1_addr = 0x01;
	uint8_t reg_1_data[2] = { 0, 0 };

	result = HAL_I2C_Mem_Read(&hi2c2, opt4001_addr, reg_1_addr,
	I2C_MEMADD_SIZE_8BIT, reg_1_data, sizeof(reg_1_data),
	HAL_MAX_DELAY);
	if (result != HAL_OK) {
#ifdef DEBUG_LIGHT_SENSOR
		printf("Failed to read OPT4001 Register 1.\n");
#endif /* DEBUG_LIGHT_SENSOR */
		light_sensor_flag = WAITING;
		return READ_FAILED;
	}

	/* Calculates the lux reading in milli-lux. */
	uint32_t exponent = (uint32_t) ((reg_0_data[0] >> 4) & 0x0F);
	uint32_t mantissa = ((uint32_t) (reg_0_data[0] & 0x0F) << 16)
			| ((uint32_t) (reg_0_data[1]) << 8) | (uint32_t) (reg_1_data[0]);
	uint32_t ADC_code = mantissa << exponent;
	mlux_reading = ADC_code * 437.5e-3;

#ifdef DEBUG_LIGHT_SENSOR
	printf("%lu mlux\n", mlux_reading);
#endif /* DEBUG_LIGHT_SENSOR */
	light_sensor_flag = NEW_READY;
	return READ_SUCCESSFUL;
}

/**
 * @brief Configures the light sensor's registers over I2C.
 *
 * @return None.
 */
InitStatus initialise_light_sensor(void) {
#ifdef DEBUG_INIT
	printf("\nINITIALISING LIGHT SENSOR\n");
#endif /* DEBUG_INIT */
	HAL_StatusTypeDef result;
	uint8_t opt4001_addr = 0x44 << 1;

	uint8_t reg_10_addr = 0x0A;
	uint8_t reg_10_config[2] = { 0b00110010, 0b00110000 };
	uint8_t reg_10_confirm[2] = { 0, 0 };
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
	result = HAL_I2C_Mem_Write(&hi2c2, opt4001_addr,		// Device address.
			reg_10_addr,				// Register address.
			I2C_MEMADD_SIZE_8BIT,	// Address size.
			reg_10_config,			// Data packet pointer.
			sizeof(reg_10_config),	// Size of data packet.
			HAL_MAX_DELAY);			// Timeout delay.
	if (result != HAL_OK) {
#ifdef DEBUG_INIT
		printf("Failed to write to OPT4001 Register 10.\n");
#endif /* DEBUG_INIT */
		return INIT_FAILED;
	} else {
#ifdef DEBUG_INIT
		printf("Successful write to OPT4001 Register 10.\n");
#endif /* DEBUG_INIT */
		result = HAL_I2C_Mem_Read(&hi2c2, opt4001_addr, reg_10_addr,
		I2C_MEMADD_SIZE_8BIT, reg_10_confirm, sizeof(reg_10_confirm),
		HAL_MAX_DELAY);
		if (result != HAL_OK) {
#ifdef DEBUG_INIT
			printf("Failed to read OPT4001 Register 10.\n ");
#endif /* DEBUG_INIT */
			return INIT_FAILED;
		} else {
#ifdef DEBUG_INIT
			printf("Successful read of OPT4001 Register 10.\n");
#endif /* DEBUG_INIT */
			if (reg_10_confirm[0] == reg_10_config[0]
					&& reg_10_confirm[1] == reg_10_config[1]) {
#ifdef DEBUG_INIT
				printf("OPT4001 Register 10 configured successfully.\n");
#endif /* DEBUG_INIT */
			} else {
#ifdef DEBUG_INIT
				printf(
						"Failed to configure OPT4001 Register 10 successfully.\n");
#endif /* DEBUG_INIT */
				return INIT_FAILED;
			}
		}
	}

	HAL_Delay(1);

	uint8_t reg_11_addr = 0x0B;
	uint8_t reg_11_config[2] = { 0b10000000, 0b00010100 };
	uint8_t reg_11_confirm[2] = { 0, 0 };
	/**
	 * Register 0Bh Configuration:
	 *
	 * D15-D05 1024 = 0b10000000000 : Fixed value.
	 * D04-D04 INT_DIR = 0b1		: INT pin configured as output.
	 * D03-D02 INT_CFG = 0b01		: INT pin asserted after each conversion.
	 * D01-D01 0 = 0b0				: Fixed value.
	 * D00-D00 I2C_BURST = 0b0		: I2C burst mode off.
	 */
	result = HAL_I2C_Mem_Write(&hi2c2, opt4001_addr, reg_11_addr,
	I2C_MEMADD_SIZE_8BIT, reg_11_config, sizeof(reg_11_config),
	HAL_MAX_DELAY);
	if (result != HAL_OK) {
#ifdef DEBUG_INIT
		printf("Failed to write to OPT4001 Register 11.\n");
#endif /* DEBUG_INIT */
		return INIT_FAILED;
	} else {
#ifdef DEBUG_INIT
		printf("Sucessful write to OPT4001 Register 11.\n");
#endif /* DEBUG_INIT */
		result = HAL_I2C_Mem_Read(&hi2c2, opt4001_addr, reg_11_addr,
		I2C_MEMADD_SIZE_8BIT, reg_11_confirm, sizeof(reg_11_confirm),
		HAL_MAX_DELAY);
		if (result != HAL_OK) {
#ifdef DEBUG_INIT
			printf("Failed to read OPT4001 Register 11.\n ");
#endif /* DEBUG_INIT */
			return INIT_FAILED;
		} else {
#ifdef DEBUG_INIT
			printf("Successful read of OPT4001 Register 11.\n");
#endif /* DEBUG_INIT */
			if (reg_11_confirm[0] == reg_11_config[0]
					&& reg_11_confirm[1] == reg_11_config[1]) {
#ifdef DEBUG_INIT
				printf("OPT4001 Register 11 configured successfully.\n");
#endif /* DEBUG_INIT */
				return INIT_SUCCESSFUL;
			} else {
#ifdef DEBUG_INIT
				printf(
						"Failed to configure OPT4001 Register 11 successfully.\n");
#endif /* DEBUG_INIT */
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

