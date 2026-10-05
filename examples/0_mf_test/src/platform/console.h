/**
 * @file    console.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_CONSOLE_H_
#define PLAT_CONSOLE_H_

void PLAT_CONSOLE_Init(void);
void PLAT_CONSOLE_putc(char ch);
void PLAT_CONSOLE_puts(const char *str);

#endif /* PLAT_CONSOLE_H_ */
