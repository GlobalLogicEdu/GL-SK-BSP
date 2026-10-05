/**
 * @file    systick.c
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

/* */
volatile uint32_t s_ticks = 0;

void PLAT_IRQ_SysTick_Handler(void)
{
    s_ticks++;
}

/* */
void PLAT_SYSTICK_Init(void)
{
    uint32_t period = (SystemCoreClock / 1000UL) - 1UL;

    /* */
    SysTick->VAL = 0;
    SysTick->LOAD = (period & SysTick_LOAD_RELOAD_Msk);

    /* */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

    /* Lowest priority */
    NVIC_SetPriority(SysTick_IRQn, 255);
}

/* */
uint32_t PLAT_SYSTICK_getTimeMs(void)
{
    return s_ticks;
}

/* */
void PLAT_SYSTICK_DelayMs(uint32_t ms)
{
    uint32_t start = s_ticks;
    while ((s_ticks - start) < ms);
}
