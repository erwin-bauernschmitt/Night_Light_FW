/**
 *******************************************************************************
 * @file LED_driver_config.h
 * @brief Declarations for LED_driver_config.c
 *
 * @author Erwin Bauernschmitt
 * @date 30/11/2023
 *******************************************************************************
 */


#ifndef LED_DRIVER_CONFIG_H
#define LED_DRIVER_CONFIG_H


typedef enum {
	LED_DRIVER_OK,
	LED_DRIVER_INIT_FAIL,
	LED_DRIVER_DOT_FAIL,
	LED_DRIVER_THERMAL_ERROR,
	LED_DRIVER_OPEN_ERROR
} LED_Driver_Status;


LED_Driver_Status initialise_LED_drivers(void);


#endif /* LED_DRIVER_CONFIG_H */
