/**
 * @file    pwm_tim.c
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

#include "platform/pwm_tim.h"

/* ------------------------------------------------------------------------- */
/* TIM4_CH4 - PWM (DMA) */
void PLAT_PWM_TIM4_DMA_Ch0_Config()
{
    /* Configure TIM4 for 800 kHz PWM generation */
    TIM4->PSC = 0U;   /* Prescaler = 1 (Timer clock = 250 MHz) */
    TIM4->ARR = 311U; /* Auto-reload value: T = 312 ticks = 1.248us (~800 kHz) */

    /* Configure Channel 4: PWM Mode 1, Preload Enable */
    TIM4->CCMR2 &= ~(TIM_CCMR2_OC4M_Msk | TIM_CCMR2_CC4S_Msk);
    TIM4->CCMR2 |= (6U << TIM_CCMR2_OC4M_Pos) | TIM_CCMR2_OC4PE;

    /* Enable Channel 4 output */
    TIM4->CCER |= TIM_CCER_CC4E;

    /* Enable DMA request on Compare 4 event */
    TIM4->DIER |= TIM_DIER_CC4DE;

    /* Enable Auto-reload Preload register */
    TIM4->CR1 |= TIM_CR1_ARPE;

    /* Initial GPDMA1 Channel 0 setup */
    GPDMA1_Channel0->CCR &= ~DMA_CCR_EN; /* Disable channel for configuration */
    while ((GPDMA1_Channel0->CSR & DMA_CSR_IDLEF) == 0);

    /* Clear all event/error flags */
    GPDMA1_Channel0->CFCR = 0x3FUL;

    /* Configure Control Register 1:
     * SINC = 1 (Source memory increments)
     * SBL_1 = 16-bit source data width
     * DINC = 0 (Destination address fixed to CCR4)
     * DBL_1 = 16-bit destination data width */
    GPDMA1_Channel0->CTR1 = DMA_CTR1_SINC |
                            (1U << DMA_CTR1_SDW_LOG2_Pos) |
                            (1U << DMA_CTR1_DDW_LOG2_Pos);

    /* Configure Control Register 2: Hardware request from TIM4_CH4 */
    GPDMA1_Channel0->CTR2 = (86U << DMA_CTR2_REQSEL_Pos); /* tim4_cc4_dma */

    /* Set Source and Destination addresses */
    GPDMA1_Channel0->CDAR = (uint32_t)&TIM4->CCR4;
}

void PLAT_PWM_TIM4_DMA_Ch0_Start(uint32_t buf_addr, size_t buf_bytes)
{
    /* Reset TIM4 counter register */
    TIM4->CNT = 0U;

    /* */
    GPDMA1_Channel0->CSAR = buf_addr;
    GPDMA1_Channel0->CBR1 = (buf_bytes & DMA_CBR1_BNDT);

    /* Clear GPDMA flags and enable channel */
    GPDMA1_Channel0->CFCR = 0x3FUL;
    GPDMA1_Channel0->CCR |= DMA_CCR_EN;

    /* Start TIM4 counter */
    TIM4->CR1 |= TIM_CR1_CEN;
}

bool PLAT_PWM_TIM4_DMA_Ch0_IsReady(void)
{
    /* Return true if channel is disabled or Transfer Complete flag is set */
    if ((GPDMA1_Channel0->CCR & DMA_CCR_EN) == 0U) {
        return true;
    }

    if (GPDMA1_Channel0->CSR & DMA_CSR_TCF) {
        /* Clear Transfer Complete Flag */
        GPDMA1_Channel0->CFCR = DMA_CFCR_TCF;
        /* Disable DMA channel */
        GPDMA1_Channel0->CCR &= ~DMA_CCR_EN;
        /* Disable Timer counter */
        TIM4->CR1 &= ~TIM_CR1_CEN;
        return true;
    }
    return false;
}

/* ------------------------------------------------------------------------- */
/* TIM8_CH1/TIM8_CH2 - PWM */
static void TIM8_Config(void)
{
    /* TIM8 PWM */
    TIM8->PSC = 249;    /* Prescaler = 250 (Timer clock = 1 MHz) */
    TIM8->ARR = 99;     /* Auto-reload value 10Khz */
    TIM8->CCR1 = 10;    /* 10% */
    TIM8->CCR2 = 50;    /* 50% */

    /* Configure Channel 1 and Channel 2 for PWM Mode */
    TIM8->CCMR1 &= ~(TIM_CCMR1_CC1S_Msk | TIM_CCMR1_OC1M_Msk |
                     TIM_CCMR1_CC2S_Msk | TIM_CCMR1_OC2M_Msk);

    TIM8->CCMR1 |= (0x6U << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE |
                   (0x6U << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE;

    /* */
    TIM8->BDTR |= TIM_BDTR_MOE;

    /* Enable Channel 1 and Channel 2 Output and Start Timer */
    TIM8->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM8->CR1 |= TIM_CR1_ARPE;  /* Enable Auto-reload preload (Synchronous ARR updates) */
    TIM8->CR1 |= TIM_CR1_CEN;
    __DSB();
}

void PLAT_PWM_TIM8_Ch1Set(uint8_t percent)
{
    if (percent > 100) percent = 100;
    TIM8->CCR1 = percent;
}

void PLAT_PWM_TIM8_Ch2Set(uint8_t percent)
{
    if (percent > 100) percent = 100;
    TIM8->CCR2 = percent;
}

/* */
void PLAT_PWM_TIM_Init(void)
{
    TIM8_Config();
}
