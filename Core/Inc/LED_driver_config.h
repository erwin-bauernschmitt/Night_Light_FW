/**
 *******************************************************************************
 * @file LED_driver_config.h
 * @brief Declarations for LED_driver_config.c
 *
 * @author Erwin Bauernschmitt
 * @date 3/12/2023
 *******************************************************************************
 */

#ifndef LED_DRIVER_CONFIG_H
#define LED_DRIVER_CONFIG_H

#include <stdint.h>

/**
 * @brief Represents the states of the LED drivers.
 */
typedef enum {
	LED_DRIVER_OK,				///< Successful initialisation of LED drivers.
	LED_DRIVER_INIT_FAIL,		///< Unsuccessful initialisation of LED drivers.
	LED_DRIVER_DOT_FAIL,		///< Unsuccessful config of dot correction.
	LED_DRIVER_THERMAL_ERROR,	///< Thermal flag raised by drivers.
	LED_DRIVER_OPEN_ERROR		///< Open LED detected by drivers.
} LED_Driver_Status;

LED_Driver_Status initialise_LED_drivers(uint8_t *led_init_config);

#endif /* LED_DRIVER_CONFIG_H */
