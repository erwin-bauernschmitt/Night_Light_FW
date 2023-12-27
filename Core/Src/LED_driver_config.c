/**
 *******************************************************************************
 * @file LED_driver_config.c
 * @brief Functions for configuring the three LED drivers.
 *
 * @author Erwin Bauernschmitt
 * @date 3/12/2023
 *******************************************************************************
 */

#include "stm32f3xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "hardware_defines.h"
#include "globals.h"
#include "LED_driver_config.h"
#include "debug_flags.h"

/**
 * @brief Configures the drivers to turn on all 16 RGB LEDs.
 *
 * @return The status of the LED drivers.
 */
LED_Driver_Status initialise_LED_drivers(uint8_t *led_init_config) {
#ifdef DEBUG_INIT
	printf("\nINITIALISING LED DRIVERS\n");
	printf("Bit-banging...\n");
#endif /* DEBUG_INIT */
	/* Set MODE/XLAT/SCLK low for ON/OFF configuration mode. */
	HAL_GPIO_WritePin(MODE_GPIO_Port, MODE_Pin, RESET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);

	int i;
	for (i = 1; i <= 16; i++) {
		/* Turn the LED on/off according to the config array. */
		HAL_GPIO_WritePin(SIN_R_GPIO_Port, SIN_R_Pin,
				led_init_config[NUM_LEDS - i]);
		HAL_GPIO_WritePin(SIN_G_GPIO_Port, SIN_G_Pin,
				led_init_config[NUM_LEDS - i]);
		HAL_GPIO_WritePin(SIN_B_GPIO_Port, SIN_B_Pin,
				led_init_config[NUM_LEDS - i]);
		/* Clock the configuration into the LED drivers. */
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, SET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);
	}

	/* Latch the configuration registers. */
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, SET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);

	return LED_DRIVER_OK;
}

