/**
 * @file    systick.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_SYSTICK_H_
#define PLAT_SYSTICK_H_

/* */
void PLAT_SYSTICK_Init(void);

/* */
uint32_t PLAT_SYSTICK_getTimeMs(void);

/* */
void PLAT_SYSTICK_DelayMs(uint32_t ms);

#endif /* PLAT_SYSTICK_H_ */
