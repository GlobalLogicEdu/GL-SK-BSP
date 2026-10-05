/**
 * @file    led_ws2812.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_WS2812_H_
#define PLAT_WS2812_H_

/* */
#define WS2812_LED_COUNT        16U
#define WS2812_BITS_PER_LED     24U
#define WS2812_RESET_SLOTS      50U  /* 50 * 1.25us = 62.5us (>50us RESET pulse) */

/* TIM4 PWM duty cycle values calculated for f_TIM4 = 250 MHz (ARR = 311) */
#define WS2812_PWM_LOGIC_0      100U /* ~0.40us High pulse */
#define WS2812_PWM_LOGIC_1      200U /* ~0.80us High pulse */

/* */
void PLAT_WS2812_Init(void);
/* Set RGB color for specific led */
void PLAT_WS2812_SetColor(uint16_t led_idx, uint8_t r, uint8_t g, uint8_t b);
/* Set RGB color for all pixels */
void PLAT_WS2812_SetAll(uint8_t r, uint8_t g, uint8_t b);
/* Trigger update */
void PLAT_WS2812_Update(void);

#endif /* PLAT_WS2812_H_ */
