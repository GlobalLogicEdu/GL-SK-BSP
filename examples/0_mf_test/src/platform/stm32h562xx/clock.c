/**
 * @file    clock.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stdint.h>
#include <stm32h562xx.h>

#include "platform/clock.h"

/* */
uint32_t SystemCoreClock = 4000000UL; /* Default: 4Mhz CSI RC initial clock */

/* Configure and Enable PLL1 for 250MHz */
static void CLK_ConfigPLL1(void)
{
    /* Turn off the PLL1 */
    RCC->CR &= ~RCC_CR_PLL1ON;
    while (RCC->CR & RCC_CR_PLL1RDY);

    /* */
    RCC->PLL1CFGR = (3U << RCC_PLL1CFGR_PLL1SRC_Pos) | /* 11 = HSE */
                    (2U << RCC_PLL1CFGR_PLL1RGE_Pos) | /* 4-8 MHz range */
                    (2U << RCC_PLL1CFGR_PLL1M_Pos) |   /* M = 2 */
                    RCC_PLL1CFGR_PLL1PEN |             /* enable P output -> feeds SYSCLK */
                    RCC_PLL1CFGR_PLL1QEN;              /* enable Q output -> feeds SPI2 */

    /* Integer mode, no fractional divider */
    RCC->PLL1FRACR = 0U;

    /* Mul/Div */
    RCC->PLL1DIVR = ((125U - 1U) << RCC_PLL1DIVR_PLL1N_Pos) | /* N = 125 -> VCO=4*125=500MHz */
                    ((2U - 1U) << RCC_PLL1DIVR_PLL1P_Pos) |   /* P = 2   -> PLL1P=500/2=250MHz */
                    ((4U - 1U) << RCC_PLL1DIVR_PLL1Q_Pos);    /* Q = 4   -> 125MHz */

    /* Turn on the PLL1 */
    RCC->CR |= RCC_CR_PLL1ON;
    while ((RCC->CR & RCC_CR_PLL1RDY) == 0);
}

/* Configure and Enable PLL2 */
static void CLK_ConfigPLL2(void)
{
    /* Turn off the PLL2 */
    RCC->CR &= ~RCC_CR_PLL2ON;
    while (RCC->CR & RCC_CR_PLL2RDY);
}

/* Configure and Enable PLL3 */
static void CLK_ConfigPLL3(void)
{
    /* Turn off the PLL3 */
    RCC->CR &= ~RCC_CR_PLL3ON;
    while ((RCC->CR & RCC_CR_PLL3RDY) != 0);
}

/* */
static void CLK_RoutePerifClocks(void)
{
    /* PLL1Q for SPI2 */
    RCC->CCIPR3 &= ~(RCC_CCIPR3_SPI2SEL);

    /* PCLK3 for LPUART1 */
    RCC->CCIPR3 &= ~(RCC_CCIPR3_LPUART1SEL);

    __DSB();
}

static void CLK_EnablePerifClocks(void)
{
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPDMA1EN);

    RCC->APB1LENR |= (RCC_APB1LENR_SPI2EN |
                      RCC_APB1LENR_I2C1EN | RCC_APB1LENR_I2C2EN |
                      RCC_APB1LENR_TIM4EN);

    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN |
                     RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIODEN |
                     RCC_AHB2ENR_GPIOEEN | RCC_AHB2ENR_GPIOFEN |
                     RCC_AHB2ENR_GPIOGEN |
                     RCC_AHB2ENR_ADCEN);

    RCC->APB2ENR |= (RCC_APB2ENR_SPI4EN |
                     RCC_APB2ENR_TIM8EN |
                     RCC_APB2ENR_TIM17EN);

    RCC->APB3ENR |= (RCC_APB3ENR_LPUART1EN |
                     RCC_APB3ENR_LPTIM1EN |
                     RCC_APB3ENR_I2C4EN);

    __DSB();
}

static void CLK_EnableLSE(void)
{
    /* Enable Write acess to backup domain */
    PWR->DBPCR |= PWR_DBPCR_DBP;
    __DSB();

    /* Turn on 32.768kHz LSE Crystal */
    RCC->BDCR |= RCC_BDCR_LSEON;
    while ((RCC->BDCR & RCC_BDCR_LSERDY) == 0);

    /* Disable Write acess to backup domain */
    PWR->DBPCR &= ~PWR_DBPCR_DBP;
    __DSB();
}

static void CLK_EnableHSE(void)
{
    /* Turn on 8MHz HSE Crystal */
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0);
}

/* */
void PLAT_CLK_Init(void)
{
    /* Raise the core voltage to VOS0 (required above 200 MHz, mandatory to reach 250 MHz) */
    PWR->VOSCR |= (3UL << PWR_VOSCR_VOS_Pos);   /* VOS0 = 0b11 */
    while ((PWR->VOSSR & PWR_VOSSR_VOSRDY) == 0U);

    /* Turn on 8MHz HSE Crystal */
    CLK_EnableHSE();

    /* Turn on 32.768kHz LSE Crystal */
    CLK_EnableLSE();

    /* Set Flash latency BEFORE raising SYSCLK,
     * 5 wait states is required for 250 MHz @ VOS0 (RM0481 flash table) */
    FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY_Msk) | FLASH_ACR_LATENCY_5WS;
    while ((FLASH->ACR & FLASH_ACR_LATENCY_Msk) != FLASH_ACR_LATENCY_5WS);

    /* Enable the Flash prefetch buffer */
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    /* SYSTEM PLL */
    CLK_ConfigPLL1();

    /* Bus prescalers (safe to set before switching SYSCLK) */
    RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_HPRE_Msk)  | (0U << RCC_CFGR2_HPRE_Pos);  /* AHB  /1 */
    RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PPRE1_Msk) | (4U << RCC_CFGR2_PPRE1_Pos); /* APB1 /2 */
    RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PPRE2_Msk) | (4U << RCC_CFGR2_PPRE2_Pos); /* APB2 /2 */
    RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PPRE3_Msk) | (4U << RCC_CFGR2_PPRE3_Pos); /* APB3 /2 */

    /* Switch SYSCLK to PLL1 and wait for confirmation */
    RCC->CFGR1 = (RCC->CFGR1 & ~RCC_CFGR1_SW) | (3U << RCC_CFGR1_SW_Pos);
    while ((RCC->CFGR1 & RCC_CFGR1_SWS) != (3U << RCC_CFGR1_SWS_Pos));

    SystemCoreClock = 250000000UL; /* 250MHz */

    /* PLL2 & PLL3 */
    CLK_ConfigPLL2();
    CLK_ConfigPLL3();

    /* Clock route for perfieria */
    CLK_RoutePerifClocks();
    CLK_EnablePerifClocks();
}
