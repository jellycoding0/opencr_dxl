/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f7xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SYS_USER_LED3_Pin GPIO_PIN_4
#define SYS_USER_LED3_GPIO_Port GPIOE
#define SYS_USER_LED2_Pin GPIO_PIN_5
#define SYS_USER_LED2_GPIO_Port GPIOE
#define SW_USER2_Pin GPIO_PIN_6
#define SW_USER2_GPIO_Port GPIOE
#define USB_SW_Pin GPIO_PIN_14
#define USB_SW_GPIO_Port GPIOC
#define ICM_SPI_INT_Pin GPIO_PIN_1
#define ICM_SPI_INT_GPIO_Port GPIOA
#define ICM_SPI_CS_Pin GPIO_PIN_4
#define ICM_SPI_CS_GPIO_Port GPIOA
#define ICM_SPI_CLK_Pin GPIO_PIN_5
#define ICM_SPI_CLK_GPIO_Port GPIOA
#define ICM_SPI_SDO_Pin GPIO_PIN_6
#define ICM_SPI_SDO_GPIO_Port GPIOA
#define DXL_PWR_EN_Pin GPIO_PIN_15
#define DXL_PWR_EN_GPIO_Port GPIOF
#define BUT_USER2_Pin GPIO_PIN_3
#define BUT_USER2_GPIO_Port GPIOG
#define DXL_DIR_Pin GPIO_PIN_9
#define DXL_DIR_GPIO_Port GPIOC
#define DXL_TX_Pin GPIO_PIN_10
#define DXL_TX_GPIO_Port GPIOC
#define DXL_RX_Pin GPIO_PIN_11
#define DXL_RX_GPIO_Port GPIOC
#define BUT_USER1_Pin GPIO_PIN_12
#define BUT_USER1_GPIO_Port GPIOC
#define SYS_STS_LED_Pin GPIO_PIN_9
#define SYS_STS_LED_GPIO_Port GPIOG
#define SYS_USER_LED4_Pin GPIO_PIN_10
#define SYS_USER_LED4_GPIO_Port GPIOG
#define SW_USER1_Pin GPIO_PIN_11
#define SW_USER1_GPIO_Port GPIOG
#define SYS_USER_LED1_Pin GPIO_PIN_12
#define SYS_USER_LED1_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
