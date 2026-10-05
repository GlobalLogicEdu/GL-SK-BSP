/**
 * @file    lcd.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_LCD_H_
#define PLAT_LCD_H_

/* */
#define LCD_WIDTH       320
#define LCD_HEIGHT      240

/* */
void PLAT_LCD_Init(void (*pfnCmd)(uint8_t), void (*pfnData)(uint8_t));
void PLAT_LCD_StartWriteWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

#endif /* PLAT_LCD_H_ */
