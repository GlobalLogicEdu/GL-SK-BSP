/**
 * @file    hd44780.c
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

#include "platform/systick.h"
#include "platform/gpio.h"
#include "platform/hd44780.h"

/* */
extern const PLAT_HD44780_t g_pHD44780;

/* */
static void HD44780_WriteNibble(const uint8_t bits)
{
    const GPIO_Pin_t *db_pins = g_pHD44780.db_pins;

    PLAT_GPIO_SetPinState(&db_pins[3], (bits >> 3) & 0x1);
    PLAT_GPIO_SetPinState(&db_pins[2], (bits >> 2) & 0x1);
    PLAT_GPIO_SetPinState(&db_pins[1], (bits >> 1) & 0x1);
    PLAT_GPIO_SetPinState(&db_pins[0], (bits >> 0) & 0x1);

    PLAT_GPIO_SetPinState(&g_pHD44780.en_pin, 1);
    PLAT_SYSTICK_DelayMs(1);
    PLAT_GPIO_SetPinState(&g_pHD44780.en_pin, 0);
    PLAT_SYSTICK_DelayMs(1);
}

static void HD44780_WriteByte(const uint8_t byte)
{
    HD44780_WriteNibble(byte >> 4);
    HD44780_WriteNibble(byte);

    PLAT_SYSTICK_DelayMs(2);
}

/* */
static void HD44780_SendCmd(uint8_t cmd)
{
    PLAT_GPIO_SetPinState(&g_pHD44780.rs_pin, 0);
    PLAT_SYSTICK_DelayMs(1);

    HD44780_WriteByte(cmd);
}

static void HD44780_SendData(uint8_t data)
{
    PLAT_GPIO_SetPinState(&g_pHD44780.rs_pin, 1);
    PLAT_SYSTICK_DelayMs(1);

    HD44780_WriteByte(data);
}

/* Text routines */
void PLAT_HD44780_PutChar(uint16_t col, uint16_t line, char ch)
{
    uint8_t cmd = 0xC0;

    if (line == 0) {
        cmd = 0x80;
    }

    HD44780_SendCmd(cmd | col);
    HD44780_SendData(ch);
}

int PLAT_HD44780_PutString(uint16_t col, uint16_t line, const char *str)
{
    char *buf = (char*)str;
    uint16_t pos = col;

    while (*buf != '\0') {
        PLAT_HD44780_PutChar(pos++, line, *buf++);
    }

    return (pos - col);
}

void PLAT_HD44780_Clear(void)
{
    HD44780_SendCmd(0x01);
}

/* */
void PLAT_HD44780_Init(void)
{
    /* Wait 50ms after power on */
    PLAT_SYSTICK_DelayMs(50);

    /* */
    PLAT_GPIO_SetPinState(&g_pHD44780.en_pin, 0);
    PLAT_GPIO_SetPinState(&g_pHD44780.rs_pin, 0);

    /* Set permanently R/W pin to W */
    if(g_pHD44780.rs_pin.io_base != NULL) {
        PLAT_GPIO_SetPinState(&g_pHD44780.rw_pin, 0);
    }
    PLAT_SYSTICK_DelayMs(20);

    /* */
    HD44780_WriteNibble(0x03); PLAT_SYSTICK_DelayMs(5);
    HD44780_WriteNibble(0x03);
    HD44780_WriteNibble(0x03);
    HD44780_WriteNibble(0x02); /* Set to 4-bit mode */

    /* */
    HD44780_SendCmd(0x28); /* 4-bit data, 2 line, 5x7 font */
    HD44780_SendCmd(0x0C); /* Display ON, cursor OFF */
    HD44780_SendCmd(0x06); /* Auto-increment cursor */
    HD44780_SendCmd(0x01); /* Clear display */

    PLAT_SYSTICK_DelayMs(5);
}
