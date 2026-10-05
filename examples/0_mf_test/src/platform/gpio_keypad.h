/**
 * @file    gpio_keypad.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_KEYPAD_H_
#define PLAT_KEYPAD_H_

/* */
#define GPIO_KEYB_COLS  3
#define GPIO_KEYB_ROWS  4

typedef enum {
    KEY_SW3 = 0,
    KEY_SW4,
    KEY_SW5,
    KEY_SW6,
    KEY_SW7,
    KEY_SW8,
    KEY_SW9,
    KEY_SW10,
    KEY_SW11,
    KEY_SW12,
    KEYS_TOTAL
} GPIO_KeyId_t;

/* */
typedef struct {
    GPIO_Pin_t  col_pins[GPIO_KEYB_COLS];
    GPIO_Pin_t  row_pins[GPIO_KEYB_ROWS];
} PLAT_KEYPAD_t;

/* */
void PLAT_KEYPAD_Init(void);

#endif /* PLAT_KEYPAD_H_ */
