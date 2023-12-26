/**
 *******************************************************************************
 * @file kelvin_to_rgb.c
 * @brief Functions for converting kelvin values to RGB values.
 *
 * @author Erwin Bauernschmitt
 * @date 21/12/2023
 *******************************************************************************
 */

#include "kelvin_to_rgb.h"
#include "globals.h"
#include <stdint.h>
#include <stdio.h>
#include <math.h>

//KelvinToRGB kelvin_table[KELVIN_TABLE_LENGTH] = {
//    {2000, 255, 147, 41},   // Warm White
//    {2700, 255, 197, 143},  // Incandescent
//    {3000, 255, 214, 170},  // Soft White
//    {3500, 255, 224, 189},  // Cool White
//    {4000, 255, 233, 209},  // Neutral White
//    {4500, 255, 242, 229},  // Daylight
//    {5000, 255, 251, 248},  // Horizon Daylight
//    {5500, 247, 255, 255},  // Vertical Daylight
//    {6500, 255, 255, 255}   // Overcast Daylight
//};

//KelvinToRGB kelvin_table[KELVIN_TABLE_LENGTH] = {
//		{1700,252,121,0},
//		{1850,252,129,2},
//		{2400,252,156,62},
//		{2550,252,162,74},
//		{2700,252,168,85},
//		{3000,252,179,106},
//		{3200,253,186,117},
//		{3350,253,191,126},
//		{5000,254,228,204},
//		{5500,254,240,232},
//		{6200,255,245,246},
//		{6500,255,249,255}
//};

KelvinToRGB kelvin_table[] = { { 1000, 255, 56, 0 }, { 1100, 255, 71, 0 }, {
		1200, 255, 83, 0 }, { 1300, 255, 93, 0 }, { 1400, 255, 101, 0 }, { 1500,
		255, 109, 0 }, { 1600, 255, 115, 0 }, { 1700, 255, 121, 0 }, { 1800,
		255, 126, 0 }, { 1900, 255, 131, 0 }, { 2000, 255, 138, 18 }, { 2100,
		255, 142, 33 }, { 2200, 255, 147, 44 }, { 2300, 255, 152, 54 }, { 2400,
		255, 157, 63 }, { 2500, 255, 161, 72 }, { 2600, 255, 165, 79 }, { 2700,
		255, 169, 87 }, { 2800, 255, 173, 94 }, { 2900, 255, 177, 101 }, { 3000,
		255, 180, 107 }, { 3100, 255, 184, 114 }, { 3200, 255, 187, 120 }, {
		3300, 255, 190, 126 }, { 3400, 255, 193, 132 }, { 3500, 255, 196, 137 },
		{ 3600, 255, 199, 143 }, { 3700, 255, 201, 148 },
		{ 3800, 255, 204, 153 }, { 3900, 255, 206, 159 },
		{ 4000, 255, 209, 163 }, { 4100, 255, 211, 168 },
		{ 4200, 255, 213, 173 }, { 4300, 255, 215, 177 },
		{ 4400, 255, 217, 182 }, { 4500, 255, 219, 186 },
		{ 4600, 255, 221, 190 }, { 4700, 255, 223, 194 },
		{ 4800, 255, 225, 198 }, { 4900, 255, 227, 202 },
		{ 5000, 255, 228, 206 }, { 5100, 255, 230, 210 },
		{ 5200, 255, 232, 213 }, { 5300, 255, 233, 217 },
		{ 5400, 255, 235, 220 }, { 5500, 255, 236, 224 },
		{ 5600, 255, 238, 227 }, { 5700, 255, 239, 230 },
		{ 5800, 255, 240, 233 }, { 5900, 255, 242, 236 },
		{ 6000, 255, 243, 239 }, { 6100, 255, 244, 242 },
		{ 6200, 255, 245, 245 }, { 6300, 255, 246, 247 },
		{ 6400, 255, 248, 251 }, { 6500, 255, 249, 253 },
		{ 6600, 254, 249, 255 }, { 6700, 252, 247, 255 },
		{ 6800, 249, 246, 255 }, { 6900, 247, 245, 255 },
		{ 7000, 245, 243, 255 }, { 7100, 243, 242, 255 },
		{ 7200, 240, 241, 255 }, { 7300, 239, 240, 255 },
		{ 7400, 237, 239, 255 }, { 7500, 235, 238, 255 },
		{ 7600, 233, 237, 255 }, { 7700, 231, 236, 255 },
		{ 7800, 230, 235, 255 }, { 7900, 228, 234, 255 },
		{ 8000, 227, 233, 255 }, };

void search_rgb_to_kelvin(uint32_t kelvin, KelvinToRGB *lower,
		KelvinToRGB *higher) {
	int i;
	for (i = 0; i < KELVIN_TABLE_LENGTH; i++) {
		if (kelvin <= kelvin_table[i].kelvin) {
			break;
		}
	}

	// Assign the lower and higher KelvinToRGB structs
	if (kelvin == kelvin_table[i].kelvin || i == 0) {
		// If exact match or less than first element, return same entry twice.
		*lower = kelvin_table[i];
		*higher = kelvin_table[i];
	} else if (kelvin > kelvin_table[KELVIN_TABLE_LENGTH - 1].kelvin) {
		// If more than last element, return last enry twice.
		*lower = kelvin_table[KELVIN_TABLE_LENGTH - 1];
		*higher = kelvin_table[KELVIN_TABLE_LENGTH - 1];
	} else {
		// If between two values, return adjacent entries
		*lower = kelvin_table[i - 1];
		*higher = kelvin_table[i];
	}
}

void pulse_for_kelvin(uint32_t kelvin, KelvinToRGB *lower, KelvinToRGB *higher,
		uint16_t *pulse_values) {
	uint32_t rgb_values[3];

	/* Interpolate RGB values. */
	if (kelvin != lower->kelvin) {
		rgb_values[0] = round(
				lower->r
						+ (double) (kelvin - lower->kelvin)
								/ (higher->kelvin - lower->kelvin)
								* (higher->r - lower->r));
		rgb_values[1] = round(
				lower->g
						+ (double) (kelvin - lower->kelvin)
								/ (higher->kelvin - lower->kelvin)
								* (higher->g - lower->g));
		rgb_values[2] = round(
				lower->b
						+ (double) (kelvin - lower->kelvin)
								/ (higher->kelvin - lower->kelvin)
								* (higher->b - lower->b));
	} else {
		rgb_values[0] = lower->r;
		rgb_values[1] = lower->g;
		rgb_values[2] = lower->b;
	}

	/* Convert RGB values to pulse values. */
	pulse_values[0] = COUNTER_PERIOD - rgb_values[0] * COUNTER_PERIOD / 256;
	pulse_values[1] = COUNTER_PERIOD - rgb_values[1] * COUNTER_PERIOD / 256;
	pulse_values[2] = COUNTER_PERIOD - rgb_values[2] * COUNTER_PERIOD / 256;
}
