/**
 * @file    pwm_tim.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_PWM_TIM_H_
#define PLAT_PWM_TIM_H_

/* */
void PLAT_PWM_TIM_Init(void);

/* Initialize TIM4 Channel 4 and GPDMA1 Channel 0 registers */
void PLAT_PWM_TIM4_DMA_Ch0_Config(void);
/* Start non-blocking transfer Buf->DMA->TIM4 */
void PLAT_PWM_TIM4_DMA_Ch0_Start(uint32_t buf_addr, size_t buf_bytes);
/* Check if GPDMA transfer is complete and ready for a new update */
bool PLAT_PWM_TIM4_DMA_Ch0_IsReady(void);

/* */
void PLAT_PWM_TIM8_Ch1Set(uint8_t percent);
void PLAT_PWM_TIM8_Ch2Set(uint8_t percent);

#endif /* PLAT_PWM_TIM_H_ */
