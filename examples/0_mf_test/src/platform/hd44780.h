/**
 * @file    hd44780.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef _HD44780_H_
#define _HD44780_H_

/* HD44780 control pins */
typedef struct {
    GPIO_Pin_t              rs_pin;
    GPIO_Pin_t              rw_pin;
    GPIO_Pin_t              en_pin;
    GPIO_Pin_t              db_pins[4];
} PLAT_HD44780_t;

/* */
void PLAT_HD44780_Init(void);
void PLAT_HD44780_Clear(void);
void PLAT_HD44780_PutChar(uint16_t col, uint16_t line, char ch);
int PLAT_HD44780_PutString(uint16_t col, uint16_t line, const char *str);

#endif /* _HD447801_H_ */
