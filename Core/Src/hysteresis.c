/**
 *******************************************************************************
 * @file hysteresis.c
 * @brief Functions for automatic on/off and the hysteresis thresholds.
 *
 * @author Erwin Bauernschmitt
 * @date 27/12/2023
 *******************************************************************************
 */

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <globals.h>
#include "hysteresis.h"

#define MIN_LUX 1      		// Minimum perceived brightness (1 mlux)
#define MAX_LUX 100000		// Maximum perceived brightness (100 lux)

void check_for_on_off(uint32_t *hysteresis_thresholds) {
	if (mlux_reading < hysteresis_thresholds[0]) {
		if (current_state == STANDBY) {
			event_flag = AMBIENT_LIGHT_TURN_ON;
			printf("\nTURNING ON:\n");
			printf("Thresholds: %lu mlux, %lu mlux\n", hysteresis_thresholds[0],
					hysteresis_thresholds[1]);
			printf("Light reading: %lu mlux\n", mlux_reading);
		}
	} else if (mlux_reading > hysteresis_thresholds[1]) {
		if ((current_state == WHITE_LIGHT) || (current_state == RGB_LIGHT)) {
			event_flag = AMBIENT_LIGHT_TURN_OFF;
			printf("\nTURNING OFF:\n");
			printf("Thresholds: %lu mlux, %lu mlux\n", hysteresis_thresholds[0],
					hysteresis_thresholds[1]);
			printf("Light reading: %lu mlux\n", mlux_reading);
		}
	}
}

void update_hysteresis_thresholds(uint32_t *hysteresis_thresholds) {
	/* Define scale factor for the logarithmic mapping. */
	double scale_factor = (log(MAX_LUX) - log(MIN_LUX)) / log(ADC_RES - 1);

	/* Apply logarithmic mapping. */
	double threshold = exp(
			log(MIN_LUX) + scale_factor * log(pot3_moving_average));

	/* Determine hysteresis factor. */
	double hysteresis_factor = 0.2;

	/* Set the hysteresis thresholds. */
	hysteresis_thresholds[0] = (uint32_t) threshold;
	hysteresis_thresholds[1] = (uint32_t) (threshold * (1 + hysteresis_factor));
}
