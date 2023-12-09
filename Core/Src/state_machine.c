/**
 *******************************************************************************
 * @file state_machine.c
 * @brief Functions for managing the night light's states and transitions.
 *
 * @author Erwin Bauernschmitt
 * @date 3/12/2023
 *******************************************************************************
 */


#include "state_machine.h"
#include "globals.h"
#include "colour_control.h"


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
			if (event == AMBIENT_LIGHT_TURN_ON) {
				current_state = colour_mode;
			}
			break;

		case WHITE_LIGHT:
			handle_white_light(event);
			break;

		case RGB_LIGHT:
			handle_RGB_light(event);
			break;

		case POT_CALIBRATION:
			update_pot_cal_substate(event);
			break;

		case LED_CALIBRATION:
			update_led_cal_substate(event);
			break;

		case AMBIENT_LIGHT_CALIBRATION:
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
			break;
		case POT_1_BUTTON_HOLD:
			previous_state = STANDBY;
			current_state = POT_CALIBRATION;
			update_pot_cal_substate(event);
			break;

		case POT_2_BUTTON_HOLD:
			previous_state = STANDBY;
			current_state = LED_CALIBRATION;
			update_led_cal_substate(event);
			break;

		case POT_3_BUTTON_HOLD:
			previous_state = STANDBY;
			current_state = AMBIENT_LIGHT_CALIBRATION;
			update_light_cal_substate(event);
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
			break;

		case POT_1_BUTTON_HOLD:
			previous_state = WHITE_LIGHT;
			current_state = POT_CALIBRATION;
			update_pot_cal_substate(event);
			break;

		case POT_2_BUTTON_HOLD:
			previous_state = WHITE_LIGHT;
			current_state = LED_CALIBRATION;
			update_led_cal_substate(event);
			break;

		case POT_3_BUTTON_HOLD:
			previous_state = WHITE_LIGHT;
			current_state = AMBIENT_LIGHT_CALIBRATION;
			update_light_cal_substate(event);
			break;

		case AMBIENT_LIGHT_TURN_OFF:
			previous_state = WHITE_LIGHT;
			current_state = STANDBY;
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
		case POT_1_BUTTON_PRESS:
			/* Change Pot 2 to select from a white colour spectrum. */
			colour_mode = WHITE_LIGHT;
			current_state = colour_mode;
			break;

		case POT_1_BUTTON_HOLD:
			previous_state = RGB_LIGHT;
			current_state = POT_CALIBRATION;
			update_pot_cal_substate(event);
			break;

		case POT_2_BUTTON_HOLD:
			previous_state = RGB_LIGHT;
			current_state = LED_CALIBRATION;
			update_led_cal_substate(event);
			break;

		case POT_3_BUTTON_HOLD:
			previous_state = RGB_LIGHT;
			current_state = AMBIENT_LIGHT_CALIBRATION;
			update_light_cal_substate(event);
			break;

		case AMBIENT_LIGHT_TURN_OFF:
			previous_state = RGB_LIGHT;
			current_state = STANDBY;
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
			/* Flash white LEDs twice to indicate start of calibration. */
			double_pulse();

			/* Set all 16 LEDs to shine red for the first pot. */
			/* Brightness pot works with unscaled values. */
			pot_cal_substate = POT_1_LOWER;
			break;

		case POT_1_LOWER:
			if (event == POT_1_BUTTON_PRESS) {
				/* Save lower value in buffer */
				/* Flash white LEDs once to indicate capture. */
				single_pulse();
				pot_cal_substate = POT_1_UPPER;
			}
			break;

		case POT_1_UPPER:
			if (event == POT_1_BUTTON_PRESS) {
				/* Compare to lower value, save in buffer if greater. */
				/* If less or equal, save 0 for lower and 4095 for upper. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				/* Set all 16 LEDs to shine green for the second pot. */
				pot_cal_substate = POT_2_LOWER;
			}
			break;

		case POT_2_LOWER:
			if (event == POT_1_BUTTON_PRESS) {
				/* Save lower value in buffer */
				/* Flash white LEDs once to indicate capture. */
				single_pulse();
				pot_cal_substate = POT_2_UPPER;
			}
			break;

		case POT_2_UPPER:
			if (event == POT_1_BUTTON_PRESS) {
				/* Compare to lower value, save in buffer if greater. */
				/* If less or equal, save 0 for lower and 4095 for upper. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				/* Set all 16 LEDs to shine blue for the third pot. */
				pot_cal_substate = POT_3_LOWER;
			}
			break;

		case POT_3_LOWER:
			if (event == POT_1_BUTTON_PRESS) {
				/* Save lower value in buffer */
				/* Flash white LEDs once to indicate capture. */
				single_pulse();
				pot_cal_substate = POT_3_UPPER;
			}
			break;

		case POT_3_UPPER:
			if (event == POT_1_BUTTON_PRESS) {
				/* Compare to lower value, save in buffer if greater. */
				/* If less or equal, save 0 for lower and 4095 for upper. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				HAL_Delay(1000);
				/* Set all 16 LEDs to shine blue for the third pot. */
				/* Save the completed calibration buffer to flash memory. */
				/* Scale potentiometer readings with tbe new calibration values */
				/* Delay, then flash white LED colours for 1s. */
				long_pulse();
				/* Flash white LEDs twice to indicate end of calibration. */
				double_pulse();
				pot_cal_substate = POT_CALIBRATION_START;
				current_state = previous_state;
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
			/* Flash white LEDs twice to indicate start of calibration. */
			double_pulse();
			/* Each potentiometer now changes an RGB colour channel.  */
			/* Turn off all LEDs except for LED 1. */
			led_cal_substate = LED_1;
			break;

		case LED_1:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 1, turn on LED 2. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_2;
			}
			break;

		case LED_2:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 2, turn on LED 3. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_3;
			}
			break;

		case LED_3:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 3, turn on LED 4. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_4;
			}
			break;

		case LED_4:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 4, turn on LED 5. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_5;
			}
			break;

		case LED_5:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 5, turn on LED 6. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_6;
			}
			break;

		case LED_6:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 6, turn on LED 7. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_7;
			}
			break;

		case LED_7:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 7, turn on LED 8. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_8;
			}
			break;

		case LED_8:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 8, turn on LED 9. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_9;
			}
			break;

		case LED_9:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 9, turn on LED 10. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_10;
			}
			break;

		case LED_10:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 10, turn on LED 11. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_11;
			}
			break;

		case LED_11:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 11, turn on LED 12. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_12;
			}
			break;

		case LED_12:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 12, turn on LED 13. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_13;
			}
			break;

		case LED_13:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 13, turn on LED 14. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_14;
			}
			break;

		case LED_14:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 14, turn on LED 15. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_15;
			}
			break;

		case LED_15:
			if (event == POT_2_BUTTON_PRESS) {
				/* Save calibration values in buffer. */
				/* Turn off LED 15, turn on LED 16. */
				/* Flash White LEDs once to indicate capture. */
				single_pulse();
				led_cal_substate = LED_16;
			}
			break;

		case LED_16:
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
				current_state = previous_state;
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
			/* Flash white LEDs twice to indicate start of calibration. */
			double_pulse();
			light_cal_substate = LIT_ROOM;
			break;

		case LIT_ROOM:
			if (event == POT_3_BUTTON_PRESS) {
				/* Measure ambient light. */
				/* Measure contribution of LEDs from 0-100% brightness */
				/* Save measurements to buffer */
				/* Flash white LEDs once to indicate capture. */
				single_pulse();
				light_cal_substate = DARK_ROOM;
			}
			break;

		case DARK_ROOM:
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
				current_state = previous_state;
				break;
			}
			break;
	}
}
