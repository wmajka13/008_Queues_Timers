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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum {
  S_MAIN_MENU,
  S_LED_EFFECT,
  S_RTC_MENU,
  S_RTC_TIME_CONFIG,
  S_RTC_DATE_CONFIG,
  S_RTC_REPORT,
} state_t;

typedef struct {
  uint8_t payload[10];
  uint8_t len;
} command_t;


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
extern TaskHandle_t handle_task_menu, handle_task_led, handle_task_rtc, handle_task_print, handle_task_command_handling;
extern QueueHandle_t handle_queue_print, handle_queue_input_data;

extern volatile uint8_t user_data;
extern volatile state_t curr_state;

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

void menu_task( void *pvParameters );
void led_task( void *pvParameters );
void rtc_task( void *pvParameters );
void print_task( void *pvParameters );
void command_handling_task( void *pvParameters );

int extract_command(command_t *cmd);
void process_command(command_t *cmd);

void led_effect_stop(void);
void led_effect(uint8_t effect);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GREEN_LED_Pin GPIO_PIN_5
#define GREEN_LED_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_6
#define RED_LED_GPIO_Port GPIOA
#define BLUE_LED_Pin GPIO_PIN_7
#define BLUE_LED_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
