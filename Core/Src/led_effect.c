#include "main.h"
#include "portmacro.h"
#include "stm32f4xx_hal_gpio.h"

static void turn_on_all_leds(void);
static void turn_off_all_leds(void);
static void turn_on_RED_WHITE(void);
static void turn_on_GREEN_BLUE(void);

void led_effect_stop(void)
{
    for(uint8_t i=0; i<4; i++)
        xTimerStop(handle_led_timers[i], portMAX_DELAY);
}

void led_effect(uint8_t n)
{
    led_effect_stop();
    xTimerStart(handle_led_timers[n-1], portMAX_DELAY);
}

static void turn_off_all_leds(void)
{
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, RESET);
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, RESET);
    HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, RESET);
    HAL_GPIO_WritePin(WHITE_LED_GPIO_Port, WHITE_LED_Pin, RESET);
}

static void turn_on_all_leds(void)
{
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, SET);
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, SET);
    HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, SET);
    HAL_GPIO_WritePin(WHITE_LED_GPIO_Port, WHITE_LED_Pin, SET);
}

static void turn_on_RED_WHITE(void)
{
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, SET);
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, RESET);
    HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, RESET);
    HAL_GPIO_WritePin(WHITE_LED_GPIO_Port, WHITE_LED_Pin, SET);
}

static void turn_on_GREEN_BLUE(void)
{
    HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, RESET);
    HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, SET);
    HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, SET);
    HAL_GPIO_WritePin(WHITE_LED_GPIO_Port, WHITE_LED_Pin, RESET);
}

void led_effect_1(void)
{
    static uint8_t flag = 1;
    (flag ^= 1) ? turn_off_all_leds() : turn_on_all_leds();
}

void led_effect_2(void)
{
    static uint8_t flag = 1;
    (flag ^= 1) ? turn_on_GREEN_BLUE() : turn_on_RED_WHITE();  
}

void led_effect_3(void)
{
    static uint8_t current_led = 0;
    turn_off_all_leds();

    switch (current_led)
    {
    case 0:
        HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, SET);
        break;
    case 1:
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, SET);
        break;
    case 2:
        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, SET);
        break;
    case 3:
        HAL_GPIO_WritePin(WHITE_LED_GPIO_Port, WHITE_LED_Pin, SET);
        current_led = 0;
        break;
    }
    current_led += 1;
}

void led_effect_4(void)
{
    static uint8_t current_led = 3;
    turn_off_all_leds();

    switch (current_led)
    {
    case 0:
        HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, SET);
        current_led = 3;
        break;
    case 1:
        HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, SET);
        break;
    case 2:
        HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, SET);
        break;
    case 3:
        HAL_GPIO_WritePin(WHITE_LED_GPIO_Port, WHITE_LED_Pin, SET);
        break;
    }
    current_led -= 1;
}