/**
 * @file    main.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stdint.h>
#include <stddef.h>
#include <stm32h562xx.h>

#include "platform/clock.h"
#include "platform/systick.h"
#include "platform/gpio.h"
#include "platform/i2c.h"
#include "platform/spi.h"
#include "platform/console.h"
#include "platform/pwm_tim.h"
#include "platform/hd44780.h"
#include "platform/led_ws2812.h"
#include "platform/gpio_keypad.h"
#include "platform/adc.h"

#include "tests.h"

/* */
void core_init(void)
{
    /* Config system clocks */
    PLAT_CLK_Init();
    PLAT_SYSTICK_Init();

    /* Core hardware components */
    PLAT_GPIO_Init();
    PLAT_CONSOLE_Init();
    PLAT_PWM_TIM_Init();
    PLAT_I2C_Init();
    PLAT_SPI_Init(SPI4, SPI_Mode0_FullDuplex, SPI_PclkDiv8 /* 125MHz /8 */);
    PLAT_SPI_Init(SPI2, SPI_Mode0_HalfDuplex, SPI_PclkDiv16 /* 125MHz /16 */);
    PLAT_ADC_Init();

    /* External hardware components */
    PLAT_HD44780_Init();
    PLAT_KEYPAD_Init();
    PLAT_WS2812_Init();
}

/* */
void main(void)
{
    PLAT_CONSOLE_puts("TEST FW v1.0\r\n");

    TESTS_Entry();
}
