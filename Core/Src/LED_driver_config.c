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
#include "hardware_defines.h"
#include "globals.h"
#include "LED_driver_config.h"


/**
 * @brief Configures the drivers to turn on all 16 RGB LEDs.
 *
 * @return The status of the LED drivers.
 */
LED_Driver_Status initialise_LED_drivers(void) {
	/* Set MODE/XLAT/SCLK low for ON/OFF configuration mode. */
	HAL_GPIO_WritePin(MODE_GPIO_Port, MODE_Pin, RESET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);

	/* Turn all three channels of all 16 LEDs ON. */
	HAL_GPIO_WritePin(SIN_R_GPIO_Port, SIN_R_Pin, SET);
	HAL_GPIO_WritePin(SIN_G_GPIO_Port, SIN_G_Pin, SET);
	HAL_GPIO_WritePin(SIN_B_GPIO_Port, SIN_B_Pin, SET);

	/* Clock 16 ones into the configuration registers. */
	int i;
	for (i = 0; i < 16; i++) {
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, SET);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, RESET);
	}

	/* Latch the configuration registers. */
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, SET);
	HAL_GPIO_WritePin(XLAT_GPIO_Port, XLAT_Pin, RESET);

	return LED_DRIVER_OK;
}


