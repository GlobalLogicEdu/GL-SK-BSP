/**
 * @file    clock.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_CLOCK_H_
#define PLAT_CLOCK_H_

/* */
extern uint32_t SystemCoreClock;

/* **/
void PLAT_CLK_Init(void);

#endif /* PLAT_CLOCK_H_ */
