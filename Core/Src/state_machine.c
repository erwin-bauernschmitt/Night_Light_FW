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
	switch (led_cal_substate) {
	case LED_CALIBRATION_START:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_CALIBRATION_START\n");
#endif /* DEBUG_STATE_MACHINE */
		/* Flash white LEDs twice to indicate start of calibration. */
		double_pulse();
		/* Each potentiometer now changes an RGB colour channel.  */
		/* Turn off all LEDs except for LED 1. */
		led_cal_substate = LED_1;
#ifdef DEBUG_STATE_MACHINE
		printf("New substate: LED_1\n");
#endif /* DEBUG_STATE_MACHINE */
		break;

	case LED_1:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_1\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 1, turn on LED 2. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_2;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_2\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_2:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_2\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 2, turn on LED 3. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_3;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_3\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_3:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_3\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 3, turn on LED 4. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_4;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_4\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_4:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_4\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 4, turn on LED 5. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_5;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_5\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_5:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_5\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 5, turn on LED 6. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_6;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_6\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_6:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_6\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 6, turn on LED 7. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_7;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_7\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_7:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_7\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 7, turn on LED 8. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_8;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_8\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_8:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_8\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 8, turn on LED 9. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_9;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_9\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_9:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_9\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 9, turn on LED 10. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_10;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_10\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_10:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_10\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 10, turn on LED 11. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_11;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_11\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_11:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_11\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 11, turn on LED 12. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_12;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_12\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_12:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_12\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 12, turn on LED 13. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_13;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_13\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_13:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_13\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 13, turn on LED 14. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_14;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_14\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_14:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_14\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 14, turn on LED 15. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_15;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_15\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_15:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_15\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 15, turn on LED 16. */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			led_cal_substate = LED_16;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_16\n");
#endif /* DEBUG_STATE_MACHINE */
		}
		break;

	case LED_16:
#ifdef DEBUG_STATE_MACHINE
		printf("Current substate: LED_16\n");
#endif /* DEBUG_STATE_MACHINE */
		if (event == POT_2_BUTTON_PRESS) {
			/* Save calibration values in buffer. */
			/* Turn off LED 16 */
			/* Flash White LEDs once to indicate capture. */
			single_pulse();
			HAL_Delay(1000);
			/* Save the completed calibration buffer to flash memory. */
			/* Configure LED drivers with new dot correction values. */
			/* Delay, then display white LED colours for 1s. */
			long_pulse();
			/* Flash white LEDs twice to indicate end of calibration. */
			double_pulse();
			led_cal_substate = LED_CALIBRATION_START;
#ifdef DEBUG_STATE_MACHINE
			printf("New substate: LED_CALIBRATION_START\n");
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
