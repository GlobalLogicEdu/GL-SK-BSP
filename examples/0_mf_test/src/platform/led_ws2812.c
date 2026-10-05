/**
 * @file    led_ws2812.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stddef.h>
#include <stdint.h>
#include <stm32h562xx.h>

#include "platform/pwm_tim.h"
#include "platform/led_ws2812.h"

/* */
typedef struct {
    uint8_t r, g, b;
} WS2812_Color_t;

/* Total DMA buffer size in 16-bit half-words */
#define WS2812_DMA_BUF_SIZE     ((WS2812_LED_COUNT * WS2812_BITS_PER_LED) + WS2812_RESET_SLOTS)

/* Pixel color buffer */
static WS2812_Color_t s_LED_colors[WS2812_LED_COUNT];

/* DMA buffer for TIMx CCRx values */
static uint16_t s_DMA_PWM_buf[WS2812_DMA_BUF_SIZE];

/* */
void PLAT_WS2812_Init(void)
{
    PLAT_PWM_TIM4_DMA_Ch0_Config();

    /* Clear LED(s) color(s) */
    PLAT_WS2812_SetAll(0, 0, 0);
}

/* */
void PLAT_WS2812_SetColor(uint16_t led_idx, uint8_t r, uint8_t g, uint8_t b)
{
    if (led_idx >= WS2812_LED_COUNT) {
        return;
    }

    s_LED_colors[led_idx].r = r;
    s_LED_colors[led_idx].g = g;
    s_LED_colors[led_idx].b = b;
}

/* */
void PLAT_WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b)
{
    for (uint16_t i = 0; i < WS2812_LED_COUNT; i++) {
        s_LED_colors[i].r = r;
        s_LED_colors[i].g = g;
        s_LED_colors[i].b = b;
    }
}

/* */
void PLAT_WS2812_Update(void)
{
    /* Wait until previous DMA transfer finishes */
    while (!PLAT_PWM_TIM4_DMA_Ch0_IsReady());

    /* */
    uint32_t buf_idx = 0U;

    /* Fill bit buffer in GRB order (MSB first) */
    for (uint16_t i = 0U; i < WS2812_LED_COUNT; i++)
    {
        uint32_t grb = ((uint32_t)s_LED_colors[i].g << 16U) |
                       ((uint32_t)s_LED_colors[i].r << 8U)  |
                        (uint32_t)s_LED_colors[i].b;

        for (int8_t bit = 23; bit >= 0; bit--)
        {
            if (grb & (1U << bit)) {
                s_DMA_PWM_buf[buf_idx++] = WS2812_PWM_LOGIC_1;
            } else {
                s_DMA_PWM_buf[buf_idx++] = WS2812_PWM_LOGIC_0;
            }
        }
    }

    /* Append low state slots for RESET pulse (> 50us) */
    while (buf_idx < WS2812_DMA_BUF_SIZE) {
        s_DMA_PWM_buf[buf_idx++] = 0U;
    }

    /* */
    PLAT_PWM_TIM4_DMA_Ch0_Start((uint32_t)s_DMA_PWM_buf, sizeof(s_DMA_PWM_buf));
}
