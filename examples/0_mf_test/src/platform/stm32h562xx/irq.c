/**
 * @file    irq.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stdint.h>
#include <stdio.h>

#include "platform/irq.h"

/* */
extern uint32_t _estack;

/* ------------------------------------------------------------------------- */
__attribute__((weak)) void PLAT_IRQ_NonMaskable_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_HardFault_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_MPUFault_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_BusFault_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_UsageFault_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_SecureFault_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_SVC_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_DebugMon_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_PendSV_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}
__attribute__((weak)) void PLAT_IRQ_SysTick_Handler(void) {
    while(1) __asm volatile ("bkpt #0");
}

/* */
__attribute__((weak)) void PLAT_IRQ_Default_Handler(void) {
    __asm volatile ("bkpt #0");
}

/* ---------------------------------------------------------------------------- */
/* Cortex-M33 Exceptions Handlers */
__attribute__ ((section(".isr_vectors")))
void (* const g_pfnVectors[])(void) =
{
    /* The initial stack pointer (MSP), top of stack */
    (void (*)(void))&_estack,

    /*  ARM Cortex-M33 Specific Interrupt Numbers */
    PLAT_IRQ_Reset_Handler,             /* -15 Reset Vector, invoked on Power up and warm reset */
    PLAT_IRQ_NonMaskable_Handler,       /* -14 Non maskable Interrupt, cannot be stopped or preempted */
    PLAT_IRQ_HardFault_Handler,         /* -13 Hard Fault, all classes of Fault */
    PLAT_IRQ_MPUFault_Handler,          /* -12 Memory Management, MPU mismatch, including Access Violation and No Match */
    PLAT_IRQ_BusFault_Handler,          /* -11 Bus Fault, Pre-Fetch-, Memory Access Fault, other address/memory related Fault */
    PLAT_IRQ_UsageFault_Handler,        /* -10 Usage Fault, i.e. Undef Instruction, Illegal State Transition */
    PLAT_IRQ_SecureFault_Handler,       /* -9  Secure Fault */
    0,                                  /* -8  Reserved */
    0,                                  /* -7  Reserved */
    0,                                  /* -6  Reserved */
    PLAT_IRQ_SVC_Handler,               /* -5  System Service Call via SVC instruction */
    PLAT_IRQ_DebugMon_Handler,          /* -4  Debug Monitor */
    0,                                  /* -3  Reserved */
    PLAT_IRQ_PendSV_Handler,            /* -2  Pendable request for system service */
    PLAT_IRQ_SysTick_Handler,           /* -1  System Tick Timer */

    /* STM32H562xx Specific Interrupt Vectors */
    PLAT_IRQ_Default_Handler,           /* Window WatchDog interrupt */
    PLAT_IRQ_Default_Handler,           /* PVD/AVD through EXTI Line detection Interrupt */
    PLAT_IRQ_Default_Handler,           /* RTC non-secure interrupt */
    PLAT_IRQ_Default_Handler,           /* RTC secure interrupt */
    PLAT_IRQ_Default_Handler,           /* Tamper global interrupt */
    PLAT_IRQ_Default_Handler,           /* RAMCFG global interrupt */
    PLAT_IRQ_Default_Handler,           /* FLASH non-secure global interrupt */
    PLAT_IRQ_Default_Handler,           /* FLASH secure global interrupt */
    PLAT_IRQ_Default_Handler,           /* Global TrustZone Controller interrupt  */
    PLAT_IRQ_Default_Handler,           /* RCC non secure global interrupt   */
    PLAT_IRQ_Default_Handler,           /* RCC secure global interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line0 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line1 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line2 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line3 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line4 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line5 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line6 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line7 interrupt */
    PLAT_IRQ_EXTI8_Handler,             /* EXTI Line8 interrupt */
    PLAT_IRQ_EXTI9_Handler,             /* EXTI Line9 interrupt */
    PLAT_IRQ_EXTI10_Handler,            /* EXTI Line10 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line11 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line12 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line13 interrupt */
    PLAT_IRQ_Default_Handler,           /* EXTI Line14 interrupt */
    PLAT_IRQ_EXTI15_Handler,            /* EXTI Line15 interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 0 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 3 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 4 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 5 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 6 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA1 Channel 7 global interrupt */
    PLAT_IRQ_Default_Handler,           /* IWDG global interrupt */
    0,                                  /* Reserved */
    PLAT_IRQ_Default_Handler,           /* ADC1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* DAC1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* FDCAN1 interrupt 0 */
    PLAT_IRQ_Default_Handler,           /* FDCAN1 interrupt 1 */
    PLAT_IRQ_Default_Handler,           /* TIM1 Break interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM1 Update interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM1 Trigger and Commutation interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM1 Capture Compare interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM3 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM4 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM5 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM6 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM7 global interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C1 Event interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C1 Error interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C2 Event interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C2 Error interrupt */
    PLAT_IRQ_Default_Handler,           /* SPI1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* SPI2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* SPI3 global interrupt */
    PLAT_IRQ_Default_Handler,           /* USART1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* USART2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* USART3 global interrupt */
    PLAT_IRQ_Default_Handler,           /* UART4 global interrupt */
    PLAT_IRQ_Default_Handler,           /* UART5 global interrupt */
    PLAT_IRQ_Default_Handler,           /* LPUART1 global interrupt  */
    PLAT_IRQ_Default_Handler,           /* LPTIM1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM8 Break interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM8 Update interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM8 Trigger and Commutation interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM8 Capture Compare interrupt */
    PLAT_IRQ_Default_Handler,           /* ADC2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* LPTIM2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM15 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM16 global interrupt */
    PLAT_IRQ_TIM17_Handler,             /* TIM17 global interrupt */
    PLAT_IRQ_Default_Handler,           /* USB FS global interrupt */
    PLAT_IRQ_Default_Handler,           /* CRS global interrupt */
    PLAT_IRQ_Default_Handler,           /* UCPD1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* FMC global interrupt */
    PLAT_IRQ_Default_Handler,           /* OctoSPI1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* SDMMC1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C3 event interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C3 error interrupt */
    PLAT_IRQ_Default_Handler,           /* SPI4 global interrupt */
    PLAT_IRQ_Default_Handler,           /* SPI5 global interrupt */
    PLAT_IRQ_Default_Handler,           /* SPI6 global interrupt */
    PLAT_IRQ_Default_Handler,           /* USART6 global interrupt */
    PLAT_IRQ_Default_Handler,           /* USART10 global interrupt  */
    PLAT_IRQ_Default_Handler,           /* USART11 global interrupt  */
    PLAT_IRQ_Default_Handler,           /* Serial Audio Interface 1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* Serial Audio Interface 2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 0 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 1 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 2 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 3 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 4 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 5 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 6 global interrupt */
    PLAT_IRQ_Default_Handler,           /* GPDMA2 Channel 7 global interrupt */
    PLAT_IRQ_Default_Handler,           /* UART7 global interrupt */
    PLAT_IRQ_Default_Handler,           /* UART8 global interrupt */
    PLAT_IRQ_Default_Handler,           /* UART9 global interrupt */
    PLAT_IRQ_Default_Handler,           /* UART12 global interrupt */
    0,                                  /* Reserved */
    PLAT_IRQ_Default_Handler,           /* FPU global interrupt */
    PLAT_IRQ_Default_Handler,           /* Instruction cache global interrupt */
    PLAT_IRQ_Default_Handler,           /* Data cache global interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    PLAT_IRQ_Default_Handler,           /* DCMI/PSSI global interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    PLAT_IRQ_Default_Handler,           /* CORDIC global interrupt */
    PLAT_IRQ_Default_Handler,           /* FMAC global interrupt */
    PLAT_IRQ_Default_Handler,           /* DTS global interrupt */
    PLAT_IRQ_Default_Handler,           /* RNG global interrupt */
    0,                                  /* Reserved */
    0,                                  /* Reserved */
    PLAT_IRQ_Default_Handler,           /* HASH global interrupt */
    0,                                  /* Reserved */
    PLAT_IRQ_Default_Handler,           /* CEC-HDMI global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM12 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM13 global interrupt */
    PLAT_IRQ_Default_Handler,           /* TIM14 global interrupt */
    PLAT_IRQ_Default_Handler,           /* I3C1 event interrupt */
    PLAT_IRQ_Default_Handler,           /* I3C1 error interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C4 event interrupt */
    PLAT_IRQ_Default_Handler,           /* I2C4 error interrupt */
    PLAT_IRQ_Default_Handler,           /* LPTIM3 global interrupt */
    PLAT_IRQ_Default_Handler,           /* LPTIM4 global interrupt */
    PLAT_IRQ_Default_Handler,           /* LPTIM5 global interrupt */
    PLAT_IRQ_Default_Handler            /* LPTIM6 global interrupt */
};
