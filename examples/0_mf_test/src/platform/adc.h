/**
 * @file    adc.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_ADC_H_
#define PLAT_ADC_H_

/* */
void PLAT_ADC_Init(void);

/* Returns actual VREF+ in mV */
uint32_t PLAT_ADC_GetVref_mV(void);

/* Returns temperature in 0.01 °C */
int32_t  PLAT_ADC_GetTemp_cC(void);

/* Returns voltage measured on POT1 in mV */
uint32_t PLAT_ADC_GetPot1_mV(void);

/* Returns voltage measured on POT2 in mV */
uint32_t PLAT_ADC_GetPot2_mV(void);

/* Returns voltage measured on POT3 in mV */
uint32_t PLAT_ADC_GetPot3_mV(void);

/* */
uint16_t PLAT_ADC_GetRawPot1(void);
uint16_t PLAT_ADC_GetRawPot2(void);
uint16_t PLAT_ADC_GetRawPot3(void);

#endif /* PLAT_ADC_H_ */
