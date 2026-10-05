/**
 * @file    lis2mdl.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_LIS2MDL_H_
#define PLAT_LIS2MDL_H_

/* */
typedef struct {
    SPI_TypeDef*    spi;
    GPIO_Pin_t      cs_pin;
} PLAT_LIS2MDL_t;

/* */
typedef struct {
    int16_t mag_raw[3]; /* Raw ADC values */

    /* Converted physical values */
    float mag_mG[3]; /* Magnetic field in milliGauss (mG) */
    float mag_uT[3]; /* Magnetic field in microTesla (µT) */
} LIS2MDL_Data_t;

/* */
bool PLAT_LIS2MDL_Init(void);
bool PLAT_LIS2MDL_Read(LIS2MDL_Data_t* data);

#endif /* PLAT_LIS2MDL_H_ */
