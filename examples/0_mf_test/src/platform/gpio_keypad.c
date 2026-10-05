/**
 * @file    gpio_keypad.c
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
#include <memory.h>
#include <stm32h562xx.h>

#include "platform/gpio.h"
#include "platform/systick.h"
#include "platform/gpio_keypad.h"

/* */
extern const PLAT_KEYPAD_t g_pKeyPad;
/* */
static bool s_keysState[GPIO_KEYB_ROWS][GPIO_KEYB_COLS];
static bool s_keysLastState[GPIO_KEYB_ROWS][GPIO_KEYB_COLS];

/* Default Weak Event Handlers */
__attribute__((weak)) void PLAT_KEYPAD_OnKeyPress(uint8_t row, uint8_t col) {
    /* Override in main application */
    (void)row;
    (void)col;
}

__attribute__((weak)) void PLAT_KEYPAD_OnKeyRelease(uint8_t row, uint8_t col) {
    /* Override in main application */
    (void)row;
    (void)col;
}

/* */
static void KEYPAD_Scan(void)
{
    for (uint16_t col = 0; col < GPIO_KEYB_COLS; col++) {
        /* Drive active column LOW */
        PLAT_GPIO_SetPinState(&g_pKeyPad.col_pins[col], 0);

        /* Short delay for signal stabilization */
        for (volatile uint32_t i = 0; i < 10; i++) { __NOP(); }

        /* Sample all row pins for the current column */
        for (uint16_t row = 0; row < GPIO_KEYB_ROWS; row++) {
            /* Active LOW: Button is pressed if pin reads 0 */
            s_keysState[row][col] = !PLAT_GPIO_GetPinState(&g_pKeyPad.row_pins[row]);
        }

        /* Return column back to HIGH (High-Z Open-Drain) */
        PLAT_GPIO_SetPinState(&g_pKeyPad.col_pins[col], 1);
    }
}

static void KEYPAD_ProcessEvents(void)
{
    for (uint16_t row = 0; row < GPIO_KEYB_ROWS; row++) {
        for (uint16_t col = 0; col < GPIO_KEYB_COLS; col++) {
            uint8_t curr = s_keysState[row][col];
            uint8_t prev = s_keysLastState[row][col];

            /* Detect Key Press (0 -> 1 transition) */
            if (curr && !prev) {
                PLAT_KEYPAD_OnKeyPress(row, col);
            }
            /* Detect Key Release (1 -> 0 transition) */
            else if (!curr && prev) {
                PLAT_KEYPAD_OnKeyRelease(row, col);
            }

            s_keysLastState[row][col] = curr;
        }
    }
}

/* */
static void TIM17_Init(void)
{
    /* Assuming SystemCoreClock is set to 250MHz (APB clock = 250MHz) */
    /* Prescaler: 250MHz / 25000 = 10 kHz timer clock (0.1 ms tick) */
    TIM17->PSC = 25000 - 1;

    /* Auto-reload value: 100 ticks @ 10 kHz = 10 ms interrupt period */
    TIM17->ARR = 100 - 1;

    /* Clear update flag and enable Update Interrupt */
    TIM17->SR &= ~TIM_SR_UIF;
    TIM17->DIER |= TIM_DIER_UIE;

    /* Enable NVIC IRQ for TIM17 */
    NVIC_SetPriority(TIM17_IRQn, 5);
    NVIC_EnableIRQ(TIM17_IRQn);

    /* Start TIM17 Counter */
    TIM17->CR1 |= TIM_CR1_CEN;
}

/* */
void PLAT_IRQ_TIM17_Handler(void)
{
    /* Check if Update Interrupt Flag is set */
    if (TIM17->SR & TIM_SR_UIF) {
        /* Clear Update Interrupt Flag */
        TIM17->SR &= ~TIM_SR_UIF;

        /* Execute non-blocking matrix scan and process events */
        KEYPAD_Scan();
        KEYPAD_ProcessEvents();
    }
}

/* */
void PLAT_KEYPAD_Init(void)
{
    memset(s_keysState, 0, sizeof(s_keysState));
    memset(s_keysLastState, 0, sizeof(s_keysLastState));

    TIM17_Init();
}

/* */
bool PLAT_KEYPAD_SW9_Pressed(void)
{
    if (PLAT_GPIO_GetState(GPIOA_NS, GPIO_BIT0)) {
        return false;
    }

    PLAT_SYSTICK_DelayMs(10); /* Debounce delay */

    if (PLAT_GPIO_GetState(GPIOA_NS, GPIO_BIT0)) {
        return false;
    }
    return true;
}

bool PLAT_KEYPAD_SW9_Released(void)
{
    if (!PLAT_GPIO_GetState(GPIOA_NS, GPIO_BIT0)) {
        return false;
    }

    PLAT_SYSTICK_DelayMs(10); /* Debounce delay */

    if (!PLAT_GPIO_GetState(GPIOA_NS, GPIO_BIT0)) {
        return false;
    }
    return true;
}
