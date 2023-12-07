/**
 *******************************************************************************
 * @file external_interrupts.h
 * @brief Declarations for external_interrupts.c
 *
 * @author Erwin Bauernschmitt
 * @date 5/12/2023
 *******************************************************************************
 */


#include <stdint.h>
#include "hardware_defines.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
