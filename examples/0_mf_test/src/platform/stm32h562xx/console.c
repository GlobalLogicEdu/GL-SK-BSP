/**
 * @file    cpnsole.c
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
#include "platform/console.h"

/* */
void PLAT_CONSOLE_Init(void)
{
    /* Defaults: 8-bit data (M0=0, M1=0), No Parity (PCE=0) */
    LPUART1->CR1 &= ~(USART_CR1_M1 | USART_CR1_M0 | USART_CR1_PCE | USART_CR1_UE);

    /* 1 Stop Bit */
    LPUART1->CR2 &= ~USART_CR2_STOP;

    /* Disable Hardware Flow Control */
    LPUART1->CR3 &= ~(USART_CR3_CTSE | USART_CR3_RTSE);

    /* USARTDIV for PCLK3=125MHz and baudrate 115200 */
    /* (256 * 125000000) / 115200 = 27777.77 */
    LPUART1->BRR = 277778U;

    /* Enable Transmitter and turn on the LPUART Peripheral */
    LPUART1->CR1 |= (USART_CR1_TE | USART_CR1_UE);
}

void PLAT_CONSOLE_putc(char ch)
{
    // Wait until the Transmit Data Register is empty (TXE bit inside ISR register)
    while ((LPUART1->ISR & USART_ISR_TXE) == 0);

    // Write the character to the Transmit Data Register (TDR)
    LPUART1->TDR = (uint32_t)ch;
}

void PLAT_CONSOLE_puts(const char *str)
{
    if (str == NULL) {
        return;
    }

    while (*str != 0) {
        if (*str == '\n') {
            PLAT_CONSOLE_putc('\r');
        }
        PLAT_CONSOLE_putc(*str++);
    }
}

void PLAT_putchar(char ch) {
    PLAT_CONSOLE_putc(ch);
}

void PLAT_puts(const char *str) {
    PLAT_CONSOLE_puts(str);
}
