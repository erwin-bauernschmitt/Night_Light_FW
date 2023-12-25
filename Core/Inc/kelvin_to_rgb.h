/**
 *******************************************************************************
 * @file kelvin_to_rgb.h
 * @brief Declarations for kelvin_to_rgb.c
 *
 * @author Erwin Bauernschmitt
 * @date 21/12/2023
 *******************************************************************************
 */

#ifndef KELVIN_TO_RGB_H
#define KELVIN_TO_RGB_H

#define KELVIN_TABLE_LENGTH 71	///< Number of entries in kelvin_table.

#include <stdint.h>

typedef struct {
    uint32_t kelvin;
    int32_t r;
    int32_t g;
    int32_t b;
} KelvinToRGB;

extern KelvinToRGB kelvin_table[];

void search_rgb_to_kelvin(uint32_t kelvin, KelvinToRGB *lower, KelvinToRGB *higher);
void pulse_for_kelvin(uint32_t kelvin, KelvinToRGB *lower, KelvinToRGB *higher, uint16_t *pulse_values);

#endif /* KELVIN_TO_RGB_H */
