/**
 *******************************************************************************
 * @file state_machine.c
 * @brief Functions for managing the night light's states and transitions.
 *
 * @author Erwin Bauernschmitt
 * @date 3/12/2023
 *******************************************************************************
 */

#include <stdio.h>
#include "state_machine.h"
#include "globals.h"
#include "colour_control.h"
#include "external_interrupts.h"
#include "debug_flags.h"
#include "LED_driver_config.h"
#include "kelvin_to_rgb.h"
#include "stdlib.h"

/**
 * @brief Updates the state of the night light in response to events.
 *
 * @param event: The event (from user or environment) being processed.
 *
 * @return None.
 */
void update_state(EventType event) {
	switch (current_state) {
	case STANDBY:
#ifdef DEBUG_STATE_MACHINE
		printf("Current state: STANDBY\n");
#endif /* DEBUG_STATE_MACHINE */
		handle_standby(event);
		break;

	case WHITE_LIGHT:
#ifdef DEBUG_STATE_MACHINE
		printf("Current state: WHITE_LIGHT\n");
#endif /* DEBUG_STATE_MACHINE */
		handle_white_light(event);
		break;

	case RGB_LIGHT:
#ifdef DEBUG_STATE_MACHINE
		printf("Current state: RGB_LIGHT\n");
#endif /* DEBUG_STATE_MACHINE */
		handle_RGB_light(event);
		break;

	case POT_CALIBRATION:
#ifdef DEBUG_STATE_MACHINE
		printf("Current state: POT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_pot_cal_substate(event);
		break;

	case LED_CALIBRATION:
#ifdef DEBUG_STATE_MACHINE
		printf("Current state: LED_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_led_cal_substate(event);
		break;
	}
}

/**
 * @brief Handles the event processing when the state is STANDBY.
 *
 * @param event: The event (from user or environment) being processed.
 *
 * @return None.
 */
void handle_standby(EventType event) {
	switch (event) {
	case AMBIENT_LIGHT_TURN_ON:
		previous_state = STANDBY;
		current_state = colour_mode;
#ifdef DEBUG_STATE_MACHINE
		if (colour_mode == WHITE_LIGHT) {
			printf("New state: WHITE_LIGHT\n");
		} else {
			printf("New state: RGB_LIGHT\n");
		}
#endif /* DEBUG_STATE_MACHINE */
		break;
	case POT_1_BUTTON_HOLD:
		previous_state = STANDBY;
		current_state = POT_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: POT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_pot_cal_substate(event);
		break;

	case POT_2_BUTTON_HOLD:
		previous_state = STANDBY;
		current_state = LED_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: LED_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_led_cal_substate(event);
		break;

	case POT_3_BUTTON_HOLD:
		event_flag = NO_EVENT; // Reset event_flag for abort functinoality.
		sensor_calibration_process();
		break;

	case POT_3_BUTTON_PRESS:
		determine_led_errors();
		break;

	default:
		// Intentionally ignoring other cases as invalid inputs for process.
		break;
	}
}

/**
 * @brief Handles the event processing when the state is WHITE_LIGHT.
 *
 * @param event: The event (from user or environment) being processed.
 *
 * @return None.
 */
void handle_white_light(EventType event) {
	switch (event) {
	case POT_2_BUTTON_PRESS:
		/* Change Pot 2 to select from an RGB colour spectrum. */
		colour_mode = RGB_LIGHT;
		current_state = colour_mode;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: RGB_LIGHT\n");
#endif /* DEBUG_STATE_MACHINE */
		break;

	case POT_1_BUTTON_HOLD:
		previous_state = WHITE_LIGHT;
		current_state = POT_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: POT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_pot_cal_substate(event);
		break;

	case POT_2_BUTTON_HOLD:
		previous_state = WHITE_LIGHT;
		current_state = LED_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: LED_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_led_cal_substate(event);
		break;

	case POT_3_BUTTON_HOLD:
		event_flag = NO_EVENT; // Reset event_flag for abort functinoality.
		sensor_calibration_process();
		break;

	case AMBIENT_LIGHT_TURN_OFF:
		previous_state = WHITE_LIGHT;
		current_state = STANDBY;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: STANDBY\n");
#endif /* DEBUG_STATE_MACHINE */
		break;

	case POT_3_BUTTON_PRESS:
		determine_led_errors();
		break;

	default:
		// Intentionally ignoring other cases as invalid inputs for process.
		break;
	}
}

/**
 * @brief Handles the event processing when the state is RGB_LIGHT.
 *
 * @param event: The event (from user or environment) being processed.
 *
 * @return None.
 */
void handle_RGB_light(EventType event) {
	switch (event) {
	case POT_2_BUTTON_PRESS:
		/* Change Pot 2 to select from a white colour spectrum. */
		colour_mode = WHITE_LIGHT;
		current_state = colour_mode;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: WHITE_LIGHT\n");
#endif /* DEBUG_STATE_MACHINE */
		break;

	case POT_1_BUTTON_HOLD:
		previous_state = RGB_LIGHT;
		current_state = POT_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: POT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_pot_cal_substate(event);
		break;

	case POT_2_BUTTON_HOLD:
		previous_state = RGB_LIGHT;
		current_state = LED_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: LED_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_led_cal_substate(event);
		break;

	case POT_3_BUTTON_HOLD:
		event_flag = NO_EVENT; // Reset event_flag for abort functinoality.
		sensor_calibration_process();
		break;

	case AMBIENT_LIGHT_TURN_OFF:
		previous_state = RGB_LIGHT;
		current_state = STANDBY;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: STANDBY\n");
#endif /* DEBUG_STATE_MACHINE */
		break;

	case POT_3_BUTTON_PRESS:
		determine_led_errors();
		break;

	default:
		// Intentionally ignoring other cases as invalid inputs for process.
		break;
	}
}

/**
 * @brief Handles the event processing when the state is POT_CALIBRATION.
 *
 * @param event: The event (from user or environment) being processed.
 *
 * @return None.
 */
void update_pot_cal_substate(EventType event) {
	switch (pot_cal_substate) {
	case POT_CALIBRATION_START:
		pot_calibration_flag = CALIBRATION_IN_PROGRESS;
#ifdef DEBUG_CALIBRATIONS
		printf("\nSTARTING POTENTIOMETER CALIBRATION PROCESS\n\n");
#endif /* DEBUG_CALIBRATIONS */

#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */

		double_pulse();
		pot_cal_substate = POT_1_LOWER;
#ifdef DEBUG_STATE_MACHINE
		printf("New substate: POT_1_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		break;

	case POT_1_LOWER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_1_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_1_BUTTON_PRESS) {
			pot1_calibration_buffer[0] = (uint16_t) HAL_ADC_GetValue(&hadc1);
			single_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("POT_1_LOWER calibrated.\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_1_UPPER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_1_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		} else if (event == POT_1_BUTTON_HOLD) {
			pot_calibration_flag = CALIBRATION_ABORTED;
			red_long_pulse();
			red_double_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */
			break;
		}
		break;

	case POT_1_UPPER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_1_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_1_BUTTON_PRESS) {
			pot1_calibration_buffer[1] = (uint16_t) HAL_ADC_GetValue(&hadc1);
			single_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("POT_1_UPPER calibrated.\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_2_LOWER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_2_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		} else if (event == POT_1_BUTTON_HOLD) {
			pot_calibration_flag = CALIBRATION_ABORTED;
			red_long_pulse();
			red_double_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */
			break;
		}
		break;

	case POT_2_LOWER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_2_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			pot2_calibration_buffer[0] = adc2_dma_buffer[1];
			single_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("POT_2_LOWER calibrated.\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_2_UPPER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_2_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		} else if (event == POT_1_BUTTON_HOLD) {
			pot_calibration_flag = CALIBRATION_ABORTED;
			red_long_pulse();
			red_double_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */
			break;
		}
		break;

	case POT_2_UPPER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_2_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			pot2_calibration_buffer[1] = adc2_dma_buffer[1];
			single_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("POT_2_UPPER calibrated.\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_3_LOWER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_3_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		} else if (event == POT_1_BUTTON_HOLD) {
			pot_calibration_flag = CALIBRATION_ABORTED;
			red_long_pulse();
			red_double_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */
			break;
		}
		break;

	case POT_3_LOWER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_3_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_3_BUTTON_PRESS) {
			pot3_calibration_buffer[0] = adc2_dma_buffer[0];
			single_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("POT_3_LOWER calibrated.\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_3_UPPER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_3_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		} else if (event == POT_1_BUTTON_HOLD) {
			pot_calibration_flag = CALIBRATION_ABORTED;
			red_long_pulse();
			red_double_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */
			break;
		}
		break;

	case POT_3_UPPER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_3_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_3_BUTTON_PRESS) {
			pot3_calibration_buffer[1] = adc2_dma_buffer[0];
			single_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("POT_3_UPPER calibrated.\n");
#endif /* DEBUG_CALIBRATIONS */
			HAL_Delay(1000);
			long_pulse();
			double_pulse();
			pot_cal_substate = POT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */

			/* Set pot_calibration_flag. */
			pot_calibration_flag = CALIBRATION_DATA_READY;
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION COMPLETED SUCCESSFULLY!\n");
#endif
			/* Print results over SWO. */
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION READINGS\n");
			printf("Pot 1:    ");
			printf("Lower = %4u,    ", pot1_calibration_buffer[0]);
			printf("Upper = %4u\n", pot1_calibration_buffer[1]);
			printf("Pot 2:    ");
			printf("Lower = %4u,    ", pot2_calibration_buffer[0]);
			printf("Upper = %4u\n", pot2_calibration_buffer[1]);
			printf("Pot 3:    ");
			printf("Lower = %4u,    ", pot3_calibration_buffer[0]);
			printf("Upper = %4u\n", pot3_calibration_buffer[1]);
#endif /* DEBUG_CALIBRATIONS */
		} else if (event == POT_1_BUTTON_HOLD) {
			pot_calibration_flag = CALIBRATION_ABORTED;
			red_long_pulse();
			red_double_pulse();
#ifdef DEBUG_CALIBRATIONS
			printf("\nPOTENTIOMETER CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			pot_cal_substate = POT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */
			break;
		}
		break;
	}
}

/**
 * @brief Handles the event processing when the state is LED_CALIBRATION.
 *
 * @param event: The event (from user or environment) being processed.
 *
 * @return None.
 */
void update_led_cal_substate(EventType event) {
#if defined(DEBUG_STATE_MACHINE) || defined(DEBUG_CALIBRATIONS)
	static const char *substate_names[] = { "LED_CALIBRATION_START", "LED_1",
			"LED_2", "LED_3", "LED_4", "LED_5", "LED_6", "LED_7", "LED_8",
			"LED_9", "LED_10", "LED_11", "LED_12", "LED_13", "LED_14", "LED_15",
			"LED_16" };
#endif /* DEBUG_STATE_MACHINE || DEBUG_CALIBRATIONS */

#ifdef DEBUG_STATE_MACHINE
	printf("Current substate: %s\n", substate_names[led_cal_substate]);
#endif /* DEBUG_STATE_MACHINE */

	/* Handle special state of LED_CALIBRATION_START. */
	if (led_cal_substate == LED_CALIBRATION_START) {
		/* Notification pulses for the beginning of the calibration process. */
		double_pulse();
		led_calibration_flag = CALIBRATION_IN_PROGRESS;
#ifdef DEBUG_CALIBRATIONS
		printf("\nSTARTING LED CALIBRATION PROCESS\n\n");
#endif /* DEBUG_CALIBRATIONS */

		/* Turn the first LED on. */
		uint8_t led_init_config[16] = { RESET };
		led_init_config[led_cal_substate] = SET;
		initialise_LED_drivers(led_init_config);

		/* Increment to the next substate. */
		led_cal_substate++;
#ifdef DEBUG_STATE_MACHINE
		printf("New substate: %s\n", substate_names[led_cal_substate]);
#endif /* DEBUG_STATE_MACHINE */
	}

	/* Handle other states. */
	else if (event == POT_2_BUTTON_PRESS) {
#ifdef DEBUG_CALIBRATIONS
		printf("%s calibrated.\n", substate_names[led_cal_substate]);
#endif /* DEBUG_CALIBRATIONS */
		/* Capture the data. */
		led_calibration_buffer[led_cal_substate - 1][0] =
				(uint16_t) HAL_ADC_GetValue(&hadc1);
		led_calibration_buffer[led_cal_substate - 1][1] = adc2_dma_buffer[1];
		led_calibration_buffer[led_cal_substate - 1][2] = adc2_dma_buffer[0];

		/* Notification pulse for data capture. */
		single_pulse();

		if (led_cal_substate < LED_16) {
			/* Turn the next LED on. */
			uint8_t led_init_config[16] = { RESET };
			led_init_config[led_cal_substate] = SET;
			initialise_LED_drivers(led_init_config);

			/* Increment to the next substate. */
			led_cal_substate++;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: %s\n", substate_names[led_cal_substate]);
#endif /* DEBUG_STATE_MACHINE */
		} else {
			/* Turn all of the LEDs on again. */
			uint8_t led_init_config[16];
			for (int i = 0; i < 16; i++) {
				led_init_config[i] = SET;
			}
			initialise_LED_drivers(led_init_config);

			/* Notification pulses for the end of the calibration process. */
			HAL_Delay(1000);
			long_pulse();
			double_pulse();

			/* Reset the substate. */
			led_cal_substate = LED_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: %s\n", substate_names[led_cal_substate]);
#endif /* DEBUG_STATE_MACHINE */

			/* Return to the previous state. */
			current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
			if (current_state == STANDBY) {
				printf("New state: STANDBY\n");
			} else if (current_state == WHITE_LIGHT) {
				printf("New state: WHITE_LIGHT\n");
			} else {
				printf("New state: RGB_LIGHT\n");
			}
#endif /* DEBUG_STATE_MACHINE */

			/* Set led_calibration_flag. */
			led_calibration_flag = CALIBRATION_DATA_READY;
#ifdef DEBUG_CALIBRATIONS
			printf("\nLED CALIBRATION COMPLETED SUCCESSFULLY!\n");
#endif /* DEBUG_CALIBRATIONS */

			/* Print results over SWO. */
#ifdef DEBUG_CALIBRATIONS
			printf("\nLED CALIBRATION READINGS\n");
			for (int i = 0; i < 16; i++) {
				printf("LED %2u:    ", i);
				printf("R = %4u,    G = %4u,    B = %4u\n",
						led_calibration_buffer[i][0],
						led_calibration_buffer[i][1],
						led_calibration_buffer[i][2]);
			}
#endif /* DEBUG_CALIBRATIONS */
		}
	} else if (event == POT_2_BUTTON_HOLD) {
		red_long_pulse();
		red_double_pulse();

		/* Set led_calibration_flag. */
		led_calibration_flag = CALIBRATION_ABORTED;
#ifdef DEBUG_CALIBRATIONS
		printf("\nLED CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */

		/* Reset the substate. */
		led_cal_substate = LED_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
		printf("New substate: %s\n", substate_names[led_cal_substate]);
#endif /* DEBUG_STATE_MACHINE */

		/* Return to the previous state. */
		current_state = previous_state;
#ifdef DEBUG_STATE_MACHINE
		if (current_state == STANDBY) {
			printf("New state: STANDBY\n");
		} else if (current_state == WHITE_LIGHT) {
			printf("New state: WHITE_LIGHT\n");
		} else {
			printf("New state: RGB_LIGHT\n");
		}
#endif /* DEBUG_STATE_MACHINE */
	}
}

int sensor_calibration_process(void) {
	/* Flash white LEDs twice to indicate start of calibration. */
	double_pulse();

	/* Set sensor_calibration_flag. */
	sensor_calibration_flag = CALIBRATION_IN_PROGRESS;
#ifdef DEBUG_CALIBRATIONS
	printf("\nSTARTING LIGHT SENSOR CALIBRATION PROCESS\n");
#endif /* DEBUG_CALIBRATIONS */

	/* Delay to allow for environment stabilisation. */
	HAL_Delay(4000);

	int attempt;
	const int max_attempts = 5;

	/* Perform brightness calibration with retry limit. */
	for (attempt = 0; attempt < max_attempts; attempt++) {
		HAL_Delay(1000);
		int result = brightness_calibration(brightness_calibration_buffer);
		if (result == 0) {
			break;
		}
		/* Check for abort. */
		else if (result == -2) {
			HAL_Delay(1000);
			red_long_pulse();
			red_double_pulse();

			/* Set sensor_calibration_flag. */
			sensor_calibration_flag = CALIBRATION_ABORTED;
#ifdef DEBUG_CALIBRATIONS
			printf("\nLIGHT SENSOR CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			return -1;
		}
	}
	if (attempt == max_attempts) {
		HAL_Delay(1000);
		red_long_pulse();
		red_double_pulse();

		/* Set sensor_calibration_flag. */
		sensor_calibration_flag = CALIBRATION_ABORTED;
#ifdef DEBUG_CALIBRATIONS
		printf("\nLIGHT SENSOR CALIBRATION FAILED\n");
#endif /* DEBUG_CALIBRATIONS */
		return -1;
	}

	/* Perform white calibration with retry limit. */
	for (attempt = 0; attempt < max_attempts; attempt++) {
		HAL_Delay(1000);
		int result = white_calibration(white_calibration_buffer);
		if (result == 0) {
			break;
		}
		/* Check for abort. */
		else if (result == -2) {
			HAL_Delay(1000);
			red_long_pulse();
			red_double_pulse();

			/* Set sensor_calibration_flag. */
			sensor_calibration_flag = CALIBRATION_ABORTED;
#ifdef DEBUG_CALIBRATIONS
			printf("\nLIGHT SENSOR CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			return -1;
		}
	}
	if (attempt == max_attempts) {
		HAL_Delay(1000);
		red_long_pulse();
		red_double_pulse();

		/* Set sensor_calibration_flag. */
		sensor_calibration_flag = CALIBRATION_ABORTED;
#ifdef DEBUG_CALIBRATIONS
		printf("\nLIGHT SENSOR CALIBRATION FAILED\n");
#endif /* DEBUG_CALIBRATIONS */
		return -1;
	}

	/* Perform colour calibration with retry limit. */
	for (attempt = 0; attempt < max_attempts; attempt++) {
		HAL_Delay(1000);
		int result = colour_calibration(colour_calibration_buffer);
		if (result == 0) {
			break;
		}
		/* Check for abort. */
		else if (result == -2) {
			HAL_Delay(1000);
			red_long_pulse();
			red_double_pulse();

			/* Set sensor_calibration_flag. */
			sensor_calibration_flag = CALIBRATION_ABORTED;
#ifdef DEBUG_CALIBRATIONS
			printf("\nLIGHT SENSOR CALIBRATION ABORTED\n");
#endif /* DEBUG_CALIBRATIONS */
			return -1;
		}
	}
	if (attempt == max_attempts) {
		HAL_Delay(1000);
		red_long_pulse();
		red_double_pulse();

		/* Set sensor_calibration_flag. */
		sensor_calibration_flag = CALIBRATION_ABORTED;
#ifdef DEBUG_CALIBRATIONS
		printf("\nLIGHT SENSOR CALIBRATION FAILED\n");
#endif /* DEBUG_CALIBRATIONS */
		return -1;
	}

	/* Notification pulses for the end of the calibration process. */
	HAL_Delay(1000);
	long_pulse();
	double_pulse();

	/* Set sensor_calibration_flag */
	sensor_calibration_flag = CALIBRATION_DATA_READY;
#ifdef DEBUG_CALIBRATIONS
	printf("\nLIGHT SENSOR CALIBRATION COMPLETED SUCCESSFULLY!\n");
#endif /* DEBUG_CALIBRATIONS */

#ifdef DEBUG_CALIBRATIONS
	printf("\nLIGHT SENSOR CALIBRATION READINGS\n");
	printf("\nBrightness calibration:\n");
	printf("Initial baseline:    mean = %lu,    variance = %lu\n",
			brightness_calibration_buffer[0][0],
			brightness_calibration_buffer[0][1]);
	for (int i = 1; i <= (NUM_CAL_INCS + 1); i++) {
		printf("Increment %2u:        mean = %lu,    variance = %lu\n", i,
				brightness_calibration_buffer[i][0],
				brightness_calibration_buffer[i][1]);
	}
	printf("Final baseline:      mean = %lu,    variance = %lu\n",
			brightness_calibration_buffer[NUM_CAL_INCS + 2][0],
			brightness_calibration_buffer[NUM_CAL_INCS + 2][1]);

	printf("\nWhite light calibration:\n");
	printf("Initial baseline:    mean = %lu,    variance = %lu\n",
			white_calibration_buffer[0][0],
			white_calibration_buffer[0][1]);
	for (int i = 1; i <= (NUM_CAL_INCS + 1); i++) {
		printf("Increment %2u:        mean = %lu,    variance = %lu\n", i,
				white_calibration_buffer[i][0],
				white_calibration_buffer[i][1]);
	}
	printf("Final baseline:      mean = %lu,    variance = %lu\n",
			white_calibration_buffer[NUM_CAL_INCS + 2][0],
			white_calibration_buffer[NUM_CAL_INCS + 2][1]);

	printf("\nColour light calibration:\n");
	printf("Initial baseline:    mean = %lu,    variance = %lu\n",
			colour_calibration_buffer[0][0],
			colour_calibration_buffer[0][1]);
	for (int i = 1; i <= NUM_CAL_INCS; i++) {
		printf("Increment %2u:        mean = %lu,    variance = %lu\n", i,
				colour_calibration_buffer[i][0],
				colour_calibration_buffer[i][1]);
	}
	printf("Final baseline:      mean = %lu,    variance = %lu\n",
			colour_calibration_buffer[NUM_CAL_INCS + 1][0],
			colour_calibration_buffer[NUM_CAL_INCS + 1][1]);
#endif /* DEBUG_CALIBRATIONS */

	return 0;
}

int baseline_calibration(uint32_t buffer[][2], int array_index) {
	/* Turn the LEDs completely off and collect data. */
	uint8_t led_init_config[16] = { RESET };
	initialise_LED_drivers(led_init_config);
#ifdef DEBUG_CALIBRATIONS
	printf("LEDS OFF\n");
#endif /* DEBUG_CALIBRATIONS */
	int result = collect_calibration_data(buffer, array_index);

	/* Turn the LEDs back on. */
	for (int i = 0; i < 16; i++) {
		led_init_config[i] = SET;
	}
	initialise_LED_drivers(led_init_config);

	/* Check for data capture failure. */
	if (result == -1) {
		return -1;
	}
	/* Check for abort request from user. */
	else if (result == -2) {
		return -2;
	}

	return 0;
}

int brightness_calibration(uint32_t buffer[][2]) {
#ifdef DEBUG_CALIBRATIONS
	printf("\nStarting brightness calibration...\n\n");
#endif /* DEBUG_CALIBRATIONS */
	int array_index = 0;
	/* Capture the initial baseline. */
	int result = baseline_calibration(buffer, array_index);
	/* Check for data capture failure. */
	if (result == -1) {
		return -1;
	}
	/* Check for abort request from user. */
	else if (result == -2) {
		return -2;
	}
	array_index += 1;

	/* Incremenet through the brightness levels and collect data. */
	for (uint16_t brightness = 0; brightness <= NUM_CAL_INCS; brightness++) {
		uint16_t pulse_value = COUNTER_PERIOD
				- brightness * COUNTER_PERIOD / NUM_CAL_INCS;
		uint16_t pulse_values[3] = { pulse_value, pulse_value, pulse_value };
		set_pulse_values(pulse_values);
#ifdef DEBUG_CALIBRATIONS
		printf("PULSE = %u\n", pulse_value);
#endif /* DEBUG_CALIBRATIONS */
		result = collect_calibration_data(buffer, array_index);
		/* Check for data capture failure. */
		if (result == -1) {
			return -1;
		}
		/* Check for abort request from user. */
		else if (result == -2) {
			return -2;
		}
		array_index += 1;
	}

	/* Recapture the baseline. */
	result = baseline_calibration(buffer, array_index);
	/* Check for data capture failure. */
	if (result == -1) {
		return -1;
	}
	/* Check for abort request from user. */
	else if (result == -2) {
		return -2;
	}

	/* Compare the initial and final baselines to check for stability. */
	int threshold = buffer[0][0] / 50;
	int difference = buffer[0][0] - buffer[array_index][0];
	if (abs(difference) > threshold) {
		red_single_pulse();
#ifdef DEBUG_CALIBRATIONS
		printf("\nERROR: Baseline changed over brightness calibration.\n");
#endif /* DEBUG_CALIBRATIONS */
		return -1;
	}
#ifdef DEBUG_CALIBRATIONS
	printf("\nBrightness calibration completed successfully.\n");
#endif /* DEBUG_CALIBRATIONS */
	return 0;
}

int colour_calibration(uint32_t buffer[][2]) {
#ifdef DEBUG_CALIBRATIONS
	printf("\nStarting colour light calibration...\n\n");
#endif /* DEBUG_CALIBRATIONS */
	int array_index = 0;
	/* Capture the initial baseline. */
	int result = baseline_calibration(buffer, array_index);
	/* Check for data capture failure. */
	if (result == -1) {
		return -1;
	}
	/* Check for abort request from user. */
	else if (result == -2) {
		return -2;
	}
	array_index += 1;

	/* Increment through the colours and collect data. */
	uint8_t segment_length = NUM_CAL_INCS / 6;
	uint16_t pulse_values[3];
	for (uint16_t colour = 0; colour < NUM_CAL_INCS; colour++) {
		uint16_t segment = colour / segment_length;
		uint16_t segment_position = colour % segment_length;
		uint16_t value = (COUNTER_PERIOD * segment_position) / segment_length;

		/* Calculate the RGB colour vector. */
		switch (segment) {
		case 0:
			pulse_values[0] = COUNTER_PERIOD;
			pulse_values[1] = value;
			pulse_values[2] = 0;
			break;

		case 1:
			pulse_values[0] = (COUNTER_PERIOD - value);
			pulse_values[1] = COUNTER_PERIOD;
			pulse_values[2] = 0;
			break;

		case 2:
			pulse_values[0] = 0;
			pulse_values[1] = COUNTER_PERIOD;
			pulse_values[2] = value;
			break;

		case 3:
			pulse_values[0] = 0;
			pulse_values[1] = (COUNTER_PERIOD - value);
			pulse_values[2] = COUNTER_PERIOD;
			break;

		case 4:
			pulse_values[0] = value;
			pulse_values[1] = 0;
			pulse_values[2] = COUNTER_PERIOD;
			break;

		case 5:
			pulse_values[0] = COUNTER_PERIOD;
			pulse_values[1] = 0;
			pulse_values[2] = (COUNTER_PERIOD - value);
			break;
		}

#ifdef DEBUG_CALIBRATIONS
		printf("RGB PULSE VECTOR = (%4u, %4u, %4u)\n", pulse_values[0],
				pulse_values[1], pulse_values[2]);
#endif /* DEBUG_CALIBRATIONS */

		/* Invert pulse values as BLANK is active low. */
		pulse_values[0] = COUNTER_PERIOD - pulse_values[0];
		pulse_values[1] = COUNTER_PERIOD - pulse_values[1];
		pulse_values[2] = COUNTER_PERIOD - pulse_values[2];

		/* Set LED colours. */
		set_pulse_values(pulse_values);

		result = collect_calibration_data(buffer, array_index);
		/* Check for data capture failure. */
		if (result == -1) {
			return -1;
		}
		/* Check for abort request from user. */
		else if (result == -2) {
			return -2;
		}
		array_index += 1;
	}

	/* Recapture the baseline. */
	result = baseline_calibration(buffer, array_index);
	/* Check for data capture failure. */
	if (result == -1) {
		return -1;
	}
	/* Check for abort request from user. */
	else if (result == -2) {
		return -2;
	}

	/* Compare the initial and final baselines to check for stability. */
	int threshold = buffer[0][0] / 50;
	int difference = buffer[0][0] - buffer[array_index][0];
	if (abs(difference) > threshold) {
		red_single_pulse();
#ifdef DEBUG_CALIBRATIONS
		printf("\nERROR: Baseline changed over colour calibration.\n");
#endif /* DEBUG_CALIBRATIONS */
		return -1;
	}
#ifdef DEBUG_CALIBRATIONS
	printf("\nColour light calibration completed successfully.\n");
#endif /* DEBUG_CALIBRATIONS */
	return 0;
}

int white_calibration(uint32_t buffer[][2]) {
#ifdef DEBUG_CALIBRATIONS
	printf("\nStarting white light calibration...\n\n");
#endif /* DEBUG_CALIBRATIONS */
	int array_index = 0;
	/* Capture the initial baseline. */
	int result = baseline_calibration(buffer, array_index);
	/* Check for data capture failure. */
	if (result == -1) {
		return -1;
	}
	/* Check for abort request from user. */
	else if (result == -2) {
		return -2;
	}
	array_index += 1;

	uint16_t kelvin_range = kelvin_table[KELVIN_TABLE_LENGTH - 1].kelvin
			- kelvin_table[0].kelvin;
	uint16_t kelvin_increment = kelvin_range / NUM_CAL_INCS;

	uint16_t kelvin;
	KelvinToRGB lower;
	KelvinToRGB higher;
	uint16_t pulse_values[3];

	for (int i = 0; i <= NUM_CAL_INCS; i++) {
		kelvin = kelvin_table[0].kelvin + i * kelvin_increment;
		search_rgb_to_kelvin(kelvin, &lower, &higher);
		pulse_for_kelvin(kelvin, &lower, &higher, pulse_values);
		set_pulse_values(pulse_values);
#ifdef DEBUG_CALIBRATIONS
		printf("TEMPERATURE: %u\n", kelvin);
#endif /* DEBUG_CALIBRATIONS */
		result = collect_calibration_data(buffer, array_index);
		/* Check for data capture failure. */
		if (result == -1) {
			return -1;
		}
		/* Check for abort request from user. */
		else if (result == -2) {
			return -2;
		}
		array_index += 1;
	}

	/* Recapture the baseline. */
	result = baseline_calibration(buffer, array_index);
	/* Check for data capture failure. */
	if (result == -1) {
		return -1;
	}
	/* Check for abort request from user. */
	else if (result == -2) {
		return -2;
	}

	/* Compare the initial and final baselines to check for stability. */
	int threshold = buffer[0][0] / 50;
	int difference = buffer[0][0] - buffer[array_index][0];
	if (abs(difference) > threshold) {
		return -1;
		red_single_pulse();
#ifdef DEBUG_CALIBRATIONS
		printf("\nERROR: Baseline changed over white calibration.\n");
#endif /* DEBUG_CALIBRATIONS */
	}
#ifdef DEBUG_CALIBRATIONS
	printf("\nWhite light calibration completed successfully.\n");
#endif /* DEBUG_CALIBRATIONS */
	return 0;
}

int collect_calibration_data(uint32_t buffer[][2], int array_index) {
	/* Discard conversion currently in progress. */
	light_sensor_flag = WAITING;
	while (light_sensor_flag != NEW_READY) {
	}
	light_sensor_flag = WAITING;

	/* Collect required number of samples. */
	uint32_t lux_samples[NUM_CAL_SAMPLES];
	for (int i = 0; i < NUM_CAL_SAMPLES; i++) {
		while (light_sensor_flag != NEW_READY) {
		}
		lux_samples[i] = mlux_reading;
		light_sensor_flag = WAITING;
	}

	/* Check for abort input from user. */
	if (event_flag == POT_3_BUTTON_HOLD) {
		return -2;
	}

	/* Compute the sample mean. */
	uint64_t sum = 0;
	for (int i = 0; i < NUM_CAL_SAMPLES; i++) {
		sum += lux_samples[i];
	}
	uint32_t mean = sum / NUM_CAL_SAMPLES;

	/* Compute the sample variance. */
	uint64_t square_difference_sum = 0;
	for (int i = 0; i < NUM_CAL_SAMPLES; i++) {
		uint32_t difference = (lux_samples[i] - mean);
		square_difference_sum += difference * difference;
	}
	uint32_t variance = square_difference_sum / (NUM_CAL_SAMPLES - 1);

	/* Compute the required sample size implied by the variance. */
	uint32_t margin_of_error = mean / 50; // 2% of sample mean.
	uint32_t z_score = 2;
	uint32_t samples_required = z_score * z_score * variance
			/ (margin_of_error * margin_of_error);

	/* Check if required sample size is less than actual sample size. */
	if (samples_required < NUM_CAL_SAMPLES) {
		buffer[array_index][0] = mean;
		buffer[array_index][1] = variance;
	} else {
		/* Notify user of failure. */
		red_single_pulse();
#ifdef DEBUG_CALIBRATIONS
		printf("\nERROR: Data collection failed due to excessive variance.\n");
#endif /* DEBUG_CALIBRATIONS */
		/* Stop the data collection as environment is unstable. */
		return -1;
	}

	return 0;
}
