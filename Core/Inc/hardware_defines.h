/**
 *******************************************************************************
 * @file hardware_defines.h
 * @brief Private definitions for pins and ports
 *
 * @author Erwin Bauernschmitt (22964301)
 * @date 19/11/2023
 *******************************************************************************
 */


#ifndef HARDWARE_DEFINES_H
#define HARDWARE_DEFINES_H


#include "stm32f3xx_hal.h"


#define BRIGHTNESS_VAL_Pin GPIO_PIN_3
#define BRIGHTNESS_VAL_GPIO_Port GPIOA
#define SENSITIVITY_VAL_Pin GPIO_PIN_4
#define SENSITIVITY_VAL_GPIO_Port GPIOA
#define COLOUR_VAL_Pin GPIO_PIN_5
#define COLOUR_VAL_GPIO_Port GPIOA
#define SOUT_R_Pin GPIO_PIN_6
#define SOUT_R_GPIO_Port GPIOA
#define XERR_R_Pin GPIO_PIN_7
#define XERR_R_GPIO_Port GPIOA
#define BLANK_R_Pin GPIO_PIN_0
#define BLANK_R_GPIO_Port GPIOB
#define SIN_R_Pin GPIO_PIN_1
#define SIN_R_GPIO_Port GPIOB
#define MODE_Pin GPIO_PIN_2
#define MODE_GPIO_Port GPIOB
#define SCLK_Pin GPIO_PIN_10
#define SCLK_GPIO_Port GPIOB
#define XLAT_Pin GPIO_PIN_11
#define XLAT_GPIO_Port GPIOB
#define SOUT_G_Pin GPIO_PIN_12
#define SOUT_G_GPIO_Port GPIOB
#define XERR_G_Pin GPIO_PIN_13
#define XERR_G_GPIO_Port GPIOB
#define XERR_G_EXTI_IRQn EXTI15_10_IRQn
#define BLANK_G_Pin GPIO_PIN_14
#define BLANK_G_GPIO_Port GPIOB
#define SIN_G_Pin GPIO_PIN_15
#define SIN_G_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_9
#define SCL_GPIO_Port GPIOA
#define SDA_Pin GPIO_PIN_10
#define SDA_GPIO_Port GPIOA
#define INT_Pin GPIO_PIN_11
#define INT_GPIO_Port GPIOA
#define INT_EXTI_IRQn EXTI15_10_IRQn
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define SIN_B_Pin GPIO_PIN_15
#define SIN_B_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define BLANK_B_Pin GPIO_PIN_4
#define BLANK_B_GPIO_Port GPIOB
#define XERR_B_Pin GPIO_PIN_5
#define XERR_B_GPIO_Port GPIOB
#define XERR_B_EXTI_IRQn EXTI9_5_IRQn
#define SOUT_B_Pin GPIO_PIN_6
#define SOUT_B_GPIO_Port GPIOB
#define BRIGHTNESS_BTN_Pin GPIO_PIN_7
#define BRIGHTNESS_BTN_GPIO_Port GPIOB
#define BRIGHTNESS_BTN_EXTI_IRQn EXTI9_5_IRQn
#define SENSITIVITY_BTN_Pin GPIO_PIN_8
#define SENSITIVITY_BTN_GPIO_Port GPIOB
#define SENSITIVITY_BTN_EXTI_IRQn EXTI9_5_IRQn
#define COLOUR_BTN_Pin GPIO_PIN_9
#define COLOUR_BTN_GPIO_Port GPIOB
#define COLOUR_BTN_EXTI_IRQn EXTI9_5_IRQn


#endif /* HARDWARE_DEFINES_H */
