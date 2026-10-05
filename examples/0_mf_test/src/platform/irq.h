/**
 * @file    irq.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_IRQ_H_
#define PLAT_IRQ_H_

/** List of Cortex-M33 ISR handlers */
extern void PLAT_IRQ_NonMaskable_Handler(void);
extern void PLAT_IRQ_Reset_Handler(void);
extern void PLAT_IRQ_MPUFault_Handler(void);
extern void PLAT_IRQ_HardFault_Handler(void);
extern void PLAT_IRQ_SysTick_Handler(void);
extern void PLAT_IRQ_BusFault_Handler(void);
extern void PLAT_IRQ_UsageFault_Handler(void);
extern void PLAT_IRQ_SecureFault_Handler(void);
extern void PLAT_IRQ_SVC_Handler(void);
extern void PLAT_IRQ_DebugMon_Handler(void);
extern void PLAT_IRQ_PendSV_Handler(void);
extern void PLAT_IRQ_SysTick_Handler(void);

extern void PLAT_IRQ_EXTI0_Handler(void);
extern void PLAT_IRQ_EXTI1_Handler(void);
extern void PLAT_IRQ_EXTI2_Handler(void);
extern void PLAT_IRQ_EXTI3_Handler(void);
extern void PLAT_IRQ_EXTI4_Handler(void);
extern void PLAT_IRQ_EXTI5_Handler(void);
extern void PLAT_IRQ_EXTI6_Handler(void);
extern void PLAT_IRQ_EXTI7_Handler(void);
extern void PLAT_IRQ_EXTI8_Handler(void);
extern void PLAT_IRQ_EXTI9_Handler(void);
extern void PLAT_IRQ_EXTI10_Handler(void);
extern void PLAT_IRQ_EXTI11_Handler(void);
extern void PLAT_IRQ_EXTI12_Handler(void);
extern void PLAT_IRQ_EXTI13_Handler(void);
extern void PLAT_IRQ_EXTI14_Handler(void);
extern void PLAT_IRQ_EXTI15_Handler(void);

extern void PLAT_IRQ_TIM17_Handler(void);

#endif // PLAT_IRQ_H_
