/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
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
#define XERR_R_EXTI_IRQn EXTI9_5_IRQn
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
#define SENSITIVITY_BTN_Pin GPIO_PIN_8
#define SENSITIVITY_BTN_GPIO_Port GPIOB
#define SENSITIVITY_BTN_EXTI_IRQn EXTI9_5_IRQn
#define COLOUR_BTN_Pin GPIO_PIN_9
#define COLOUR_BTN_GPIO_Port GPIOB
#define COLOUR_BTN_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
