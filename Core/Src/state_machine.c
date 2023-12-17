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

	case AMBIENT_LIGHT_CALIBRATION:
#ifdef DEBUG_STATE_MACHINE
		printf("Current state: AMBIENT_LIGHT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_light_cal_substate(event);
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
		previous_state = STANDBY;
		current_state = AMBIENT_LIGHT_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: AMBIENT_LIGHT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_light_cal_substate(event);
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
		previous_state = WHITE_LIGHT;
		current_state = AMBIENT_LIGHT_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: AMBIENT_LIGHT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_light_cal_substate(event);
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
		previous_state = RGB_LIGHT;
		current_state = AMBIENT_LIGHT_CALIBRATION;
#ifdef DEBUG_STATE_MACHINE
		printf("New state: AMBIENT_LIGHT_CALIBRATION\n");
#endif /* DEBUG_STATE_MACHINE */
		update_light_cal_substate(event);
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
			pot_cal_substate = POT_1_UPPER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_1_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case POT_1_UPPER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_1_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_1_BUTTON_PRESS) {
			pot1_calibration_buffer[1] = (uint16_t) HAL_ADC_GetValue(&hadc1);
			single_pulse();
			pot_cal_substate = POT_2_LOWER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_2_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case POT_2_LOWER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_2_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			pot2_calibration_buffer[0] = adc2_dma_buffer[1];
			single_pulse();
			pot_cal_substate = POT_2_UPPER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_2_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case POT_2_UPPER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_2_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			pot2_calibration_buffer[1] = adc2_dma_buffer[1];
			single_pulse();
			pot_cal_substate = POT_3_LOWER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_3_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case POT_3_LOWER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_3_LOWER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_3_BUTTON_PRESS) {
			pot3_calibration_buffer[0] = adc2_dma_buffer[0];
			single_pulse();
			pot_cal_substate = POT_3_UPPER;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: POT_3_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case POT_3_UPPER:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: POT_3_UPPER\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_3_BUTTON_PRESS) {
			pot3_calibration_buffer[1] = adc2_dma_buffer[0];
			single_pulse();
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

			/* Print results over SWO. */
			printf("\nPOTENTIOMETER CALIBRATION READINGS\n");
			printf("Pot 1:\n");
			printf("Lower = %4u\n", pot1_calibration_buffer[0]);
			printf("Upper = %4u\n", pot1_calibration_buffer[1]);
			printf("Pot 2:\n");
			printf("Lower = %4u\n", pot2_calibration_buffer[0]);
			printf("Upper = %4u\n", pot2_calibration_buffer[1]);
			printf("Pot 3:\n");
			printf("Lower = %4u\n", pot3_calibration_buffer[0]);
			printf("Upper = %4u\n", pot3_calibration_buffer[1]);

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
#ifdef DEBUG_STATE_MACHINE
	static const char *substate_names[] = { "LED_CALIBRATION_START", "LED_1",
			"LED_2", "LED_3", "LED_4", "LED_5", "LED_6", "LED_7", "LED_8",
			"LED_9", "LED_10", "LED_11", "LED_12", "LED_13", "LED_14", "LED_15",
			"LED_16" };

	printf("Current substate: %s\n", substate_names[led_cal_substate]);
#endif /* DEBUG_STATE_MACHINE */

	/* Handle special state of LED_CALIBRATION_START. */
	if (led_cal_substate == LED_CALIBRATION_START) {
		/* Notification pulses for the beginning of the calibration process. */
		double_pulse();

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

			/* Print results over SWO. */
			printf("\nLED CALIBRATION READINGS\n");
			for (int i = 0; i < 16; i++) {
				printf("LED %2u:    ", i);
				printf("R = %4u        G = %4u        B = %4u\n",
						led_calibration_buffer[i][0],
						led_calibration_buffer[i][1],
						led_calibration_buffer[i][2]);
			}
		}
	}
}

/**
 * @brief Handles the event processing when state is AMBIENT_LIGHT_CALIBRATION.
 *
 * @param event: The event (from user or environment) being processed.
 *
 * @return None.
 */
void update_light_cal_substate(EventType event) {
	switch (light_cal_substate) {
	case LIGHT_CALIBRATION_START:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LIGHT_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
		/* Flash white LEDs twice to indicate start of calibration. */
		double_pulse();
		light_cal_substate = LIT_ROOM;
#ifdef DEBUG_STATE_MACHINE
		printf("New substate: LIT_ROOM\n");
#endif /* DEBUG_STATE_MACHINE */
		break;

	case LIT_ROOM:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LIT_ROOM\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_3_BUTTON_PRESS) {
			/* Measure ambient light. */
			/* Measure contribution of LEDs from 0-100% brightness */
			/* Save measurements to buffer */
			/* Flash white LEDs once to indicate capture. */
			single_pulse();
			light_cal_substate = DARK_ROOM;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: DARK_ROOM\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case DARK_ROOM:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: DARK_ROOM\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_3_BUTTON_PRESS) {
			/* Measure ambient light. */
			/* Measure contribution of LEDs from 0-100% brightness */
			/* Save measurements to buffer */
			/* Flash white LEDs once to indicate capture. */
			single_pulse();
			HAL_Delay(1000);
			/* Save the calibration data to flash memory. */
			/* Configure hysteresis thresholds. */
			/* Delay, then display white LED colours for 1s. */
			long_pulse();
			/* Flash white LEDs twice to indicate end of calibration. */
			double_pulse();
			light_cal_substate = LIGHT_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LIGHT_CALIBRATION_START\n");
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
