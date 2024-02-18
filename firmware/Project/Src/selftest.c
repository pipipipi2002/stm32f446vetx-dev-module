#include "stm32f4xx_hal.h"
#include "main.h"
#include "selftest.h"

void test_gpioBlink(void) {
    HAL_GPIO_TogglePin(GP_LED_GPIO_Port, GP_LED_Pin);
}