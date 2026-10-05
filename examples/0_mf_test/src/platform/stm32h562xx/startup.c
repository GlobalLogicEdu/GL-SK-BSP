/**
 * @file    startup.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stdint.h>
#include <string.h>
#include <stm32h562xx.h>

/* ------------------------------------------------------------------------- */
/* Mapped regions */
extern uint32_t _bss_start, _bss_end;
extern uint32_t _data_start, _data_end;
extern uint32_t _si_data; /* Start of data in FLASH */
extern uint32_t _estack;
extern void (* const g_pfnVectors[])(void);

/* ------------------------------------------------------------------------- */
__attribute__((naked, used))
    void PLAT_IRQ_Reset_Handler(void)
{
    __asm volatile (
        "ldr sp, =_estack   \n" /* set stack pointer */
        "ldr r0, =_sstack   \n"
        "msr MSPLIM, r0     \n" /* set stack pointer limit */
    );

    /* Copy the DATA segment initializers from Flash to SRAM */
    uint32_t *p_src = &_si_data;
    uint32_t *p_dst = &_data_start;

    while (p_dst < &_data_end) {
        *p_dst++ = *p_src++;
    }

    /* Zero-fill the BSS segment (uninitialized variables) */
    uint32_t *p_bss = &_bss_start;
    while (p_bss < &_bss_end) {
        *p_bss++ = 0;
    }

    /* --------------------------------------------------------------------- */
    /* FPU settings */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 20U) | (3UL << 22U));  /* set CP10 and CP11 Full Access */
#endif

    /* Enable UsageFault, BusFault, MemManage */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk |
                  SCB_SHCSR_BUSFAULTENA_Msk |
                  SCB_SHCSR_MEMFAULTENA_Msk;

    /* Disable all interrupts */
    RCC->CIER = 0U;

    /* Configure the Vector Table location add offset address */
    SCB->VTOR = (uint32_t)&g_pfnVectors;

    /* Enable ICache */
    if ((ICACHE->CR & ICACHE_CR_EN) == 0) {
        ICACHE->CR |= ICACHE_CR_EN;
        __DSB();
        __ISB();
    }

    /* --------------------------------------------------------------------- */
    __asm volatile(
        "BL core_init       \n"
        "BL main            \n"
        "loop:              \n"
        "B loop             \n"
    );
}
