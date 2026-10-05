/**
 * @file    adc.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stdint.h>
#include <stddef.h>
#include <stm32h562xx.h>

#include "platform/dma.h"
#include "platform/adc.h"

/* Addresses of calibration data in STM32H5 System Memory */
#define TEMPSENSOR_CAL1_ADDR  ((uint16_t*) (0x08FFF814U)) /* TS_CAL1 @ 30 °C */
#define TEMPSENSOR_CAL2_ADDR  ((uint16_t*) (0x08FFF818U)) /* TS_CAL2 @ 130 °C */
#define VREFINT_CAL_ADDR      ((uint16_t*) (0x08FFF810U)) /* VREFINT_CAL @ 3.0V */

/* External TL431 (U16) reference voltage source (2495 mV) */
#define EXT_VREF_MV           2495U

/* Global buffer storing raw ADC values (0..4095 for 12-bit)
 * Index 0: ADC1_IN8  (Potentiometer 1)
 * Index 1: ADC1_IN15 (Potentiometer 2)
 * Index 2: ADC1_IN18 (Potentiometer 3)
 * Index 3: ADC1_IN16 (V_SENSE - Temperature Sensor)
 * Index 4: ADC1_IN17 (V_REFINT - Internal Reference Voltage)
 */
static struct ADC_RawData_s {
    uint16_t pot1;
    uint16_t pot2;
    uint16_t pot3;
    uint16_t temp;
    uint16_t vref;
} s_ADC_RawData;

/* Linked List for DMA CH1 */
static GPDMA_LLI_t s_dma_lli __attribute__((aligned(32)));

/* */
void PLAT_ADC_Init(void)
{
    /* ---------------------------------
     * GPDMA1 Channel 1 Configuration
     * --------------------------------- */

    /* Disable GPDMA1 Channel 1 before configuration */
    GPDMA1_Channel1->CCR &= ~DMA_CCR_EN;
    while(GPDMA1_Channel1->CCR & DMA_CCR_EN);

    /* Configure Transfer Control Register 1 (CTR1) */
    s_dma_lli.CTR1 = (0UL << DMA_CTR1_SAP_Pos) |      /* Perif */
                     (1UL << DMA_CTR1_DAP_Pos) |      /* SRAM */
                     (1UL << DMA_CTR1_SDW_LOG2_Pos) | /* Source Data Width = 16-bit */
                     (1UL << DMA_CTR1_DDW_LOG2_Pos) | /* Destination Data Width = 16-bit */
                     (0UL << DMA_CTR1_SINC_Pos) |     /* Src fixed */
                     (1UL << DMA_CTR1_DINC_Pos);      /* Dst increment enabled */

    s_dma_lli.CTR2 = (0U << DMA_CTR2_REQSEL_Pos) |    /* adc1_dma */
                     DMA_CTR2_PFREQ;

    s_dma_lli.SAR = (uint32_t)&(ADC1->DR);            /* Set Source Address (ADC1 Data Register) */
    s_dma_lli.DAR = (uint32_t)&s_ADC_RawData;         /* Set Destination Address (Array in RAM) */
    s_dma_lli.CBR1 = sizeof(s_ADC_RawData) & DMA_CBR1_BNDT; /* Block Data Size in bytes */

    /* Link the loop back to the same list to create a continuous Circular Loop */
    s_dma_lli.CLLR = (((uint32_t)&s_dma_lli) & DMA_CLLR_LA_Msk) |
                   DMA_CLLR_UT1 | DMA_CLLR_UT2 | DMA_CLLR_UB1 |
                   DMA_CLLR_USA | DMA_CLLR_UDA | DMA_CLLR_ULL;

    /* Load configuration into the live GPDMA hardware registers */
    GPDMA1_Channel1->CTR1 = s_dma_lli.CTR1;
    GPDMA1_Channel1->CTR2 = s_dma_lli.CTR2;
    GPDMA1_Channel1->CBR1 = s_dma_lli.CBR1;
    GPDMA1_Channel1->CSAR = s_dma_lli.SAR;
    GPDMA1_Channel1->CDAR = s_dma_lli.DAR;
    GPDMA1_Channel1->CLLR = s_dma_lli.CLLR;
    GPDMA1_Channel1->CLBAR = ((uint32_t)&s_dma_lli) & 0xFFFF0000UL;

    /* */
    ADC12_COMMON->CCR |= ADC_CCR_TSEN; /* Enable Temperature Sensor path */
    // ADC12_COMMON->CCR |= ADC_CCR_VREFEN; /* VREFINT Buffer Enable */

    /* Prescaller HCLK/6 */
    ADC12_COMMON->CCR &= ~ADC_CCR_PRESC;
    ADC12_COMMON->CCR |= (3U << ADC_CCR_PRESC_Pos);

    /* ----------------------------------------
     * ADC1 Hardware Calibration & Power-On
     * ---------------------------------------- */

    /* Exit Deep-Power-Down mode and enable ADC Voltage Regulator */
    ADC1->CR &= ~ADC_CR_DEEPPWD;
    ADC1->CR |= ADC_CR_ADVREGEN;

    /* Wait for ADC Voltage Regulator startup time (~20 us) */
    for (volatile int i = 0; i < 1000; i++) { __NOP(); }

    /* Ensure Single-Ended Calibration Mode for Calibration (ADCALDIF = 0) */
    ADC1->CR &= ~ADC_CR_ADCALDIF;

    /* Start ADC Calibration */
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    /* ----------------------------------------
     * ADC1 Registers Configuration
     * ---------------------------------------- */

    /* Ensure ADC1 is disabled before altering configuration */
    if (ADC1->CR & ADC_CR_ADEN) {
        ADC1->CR |= ADC_CR_ADDIS;
        while (ADC1->CR & ADC_CR_ADEN);
    }

    /* CFGR1 Configuration: Continuous mode, Circular DMA mode, 12-bit resolution */
    ADC1->CFGR &= ~ADC_CFGR_RES;     /* 12-bit resolution (00) */
    ADC1->CFGR |= (ADC_CFGR_CONT |   /* Continuous Conversion Mode */
                   ADC_CFGR_DMACFG | /* DMA Circular Mode */
                   ADC_CFGR_DMAEN);

    /* Single-Ended for all used channels (8, 15, 16, 17 and 18) */
    ADC1->DIFSEL &= ~(ADC_DIFSEL_DIFSEL_8  | ADC_DIFSEL_DIFSEL_15 |
                      ADC_DIFSEL_DIFSEL_16 | ADC_DIFSEL_DIFSEL_17 |
                      ADC_DIFSEL_DIFSEL_18);

    /* Sampling Times Configuration:
     * Pots (IN8, IN15, IN18): 800.5 cycles (0b111)
     * Internal (IN16 V_SENSE, IN17 V_REFINT): 800.5 cycles (0b111)
     */
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP8_Msk);
    ADC1->SMPR2 &= ~(ADC_SMPR2_SMP15_Msk | ADC_SMPR2_SMP16_Msk |
                     ADC_SMPR2_SMP17_Msk | ADC_SMPR2_SMP18_Msk);

    ADC1->SMPR1 |= (7U << ADC_SMPR1_SMP8_Pos);  /* IN8 */
    ADC1->SMPR2 |= (7U << ADC_SMPR2_SMP15_Pos); /* IN15 */
    ADC1->SMPR2 |= (7U << ADC_SMPR2_SMP18_Pos); /* IN18 */
    ADC1->SMPR2 |= (7U << ADC_SMPR2_SMP16_Pos); /* IN16 - V_SENSE (Temp) */
    ADC1->SMPR2 |= (7U << ADC_SMPR2_SMP17_Pos); /* IN17 - V_REFINT */

    /* Sequence Length = 5 (L = 4 in SQR1) */
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= (4U << ADC_SQR1_L_Pos);

    /* Sequence Ranks Mapping: */
    ADC1->SQR1 &= ~ADC_SQR1_SQ1;
    ADC1->SQR1 |= (8U << ADC_SQR1_SQ1_Pos);  /* Rank 1: IN8  (Pot 1) */

    ADC1->SQR1 &= ~ADC_SQR1_SQ2;
    ADC1->SQR1 |= (18U << ADC_SQR1_SQ2_Pos); /* Rank 2: IN18 (Pot 2) */

    ADC1->SQR1 &= ~ADC_SQR1_SQ3;
    ADC1->SQR1 |= (15U << ADC_SQR1_SQ3_Pos); /* Rank 3: IN15 (Pot 3) */

    ADC1->SQR1 &= ~ADC_SQR1_SQ4;
    ADC1->SQR1 |= (16U << ADC_SQR1_SQ4_Pos); /* Rank 4: IN16 (V_SENSE Temp) */

    ADC1->SQR2 &= ~ADC_SQR2_SQ5;
    ADC1->SQR2 |= (17U << ADC_SQR2_SQ5_Pos); /* Rank 5: IN17 (V_REFINT) */

    /* ----------------------------------------
     * Enable & Start DMA & ADC1
     * ---------------------------------------- */

    /* Clear ADRDY bit by setting it */
    ADC1->ISR |= ADC_ISR_ADRDY;
    ADC1->CR |= ADC_CR_ADEN;

    /* Wait until ADC1 is ready */
    while (!(ADC1->ISR & ADC_ISR_ADRDY));

    /* Start DMA & ADC */
    GPDMA1_Channel1->CCR |= DMA_CCR_EN;
    ADC1->CR |= ADC_CR_ADSTART;
}

/* */
uint32_t PLAT_ADC_GetVref_mV(void)
{
    uint32_t vrefint_cal = (uint32_t)(*VREFINT_CAL_ADDR);

    if (s_ADC_RawData.vref == 0) {
        return EXT_VREF_MV;
    }

    /* Vref_mV = (3000 * EXT_VREF_MV) / VREFINT_RAW */
    return (EXT_VREF_MV * vrefint_cal) / s_ADC_RawData.vref;
}

/* Calculate chip temperature in hundredths of °C (0.01 °C steps) */
int32_t  PLAT_ADC_GetTemp_cC(void)
{
    int32_t raw_temp = (int32_t)s_ADC_RawData.temp; /* Index 3 = IN16 */
    int32_t ts_cal1 = (int32_t)(*TEMPSENSOR_CAL1_ADDR);
    int32_t ts_cal2 = (int32_t)(*TEMPSENSOR_CAL2_ADDR);

    int32_t cal_diff = ts_cal2 - ts_cal1;
    if (cal_diff == 0) {
        return 0; // Prevent division by zero
    }

    /* Scale raw value to 3.0V factory calibration level:
     * raw_temp_3v = (raw_temp * current_vref_mv) / 3000
     */
    int32_t raw_temp_3v = (raw_temp * EXT_VREF_MV) / 3000/*mV*/;

    /* Linear interpolation formula (0.01°C scale, offset +30°C = +3000):
     * Temp_cC = 3000 + ((raw_temp_3v - ts_cal1) * 10000) / (ts_cal2 - ts_cal1)
     */
    int32_t temp_cC = 3000 + (((raw_temp_3v - ts_cal1) * 10000) / cal_diff);

    return temp_cC;
}

/* */
uint16_t PLAT_ADC_GetRawPot1(void)
{
    return s_ADC_RawData.pot1;
}

uint16_t PLAT_ADC_GetRawPot2(void)
{
    return s_ADC_RawData.pot2;
}

uint16_t PLAT_ADC_GetRawPot3(void)
{
    return s_ADC_RawData.pot3;
}

/* */
uint32_t PLAT_ADC_GetPot1_mV(void)
{
    return ((uint32_t)s_ADC_RawData.pot1 * EXT_VREF_MV) / 4095U;
}

/* */
uint32_t PLAT_ADC_GetPot2_mV(void)
{
    return ((uint32_t)s_ADC_RawData.pot2 * EXT_VREF_MV) / 4095U;
}

/* */
uint32_t PLAT_ADC_GetPot3_mV(void)
{
    return ((uint32_t)s_ADC_RawData.pot3 * EXT_VREF_MV) / 4095U;
}
