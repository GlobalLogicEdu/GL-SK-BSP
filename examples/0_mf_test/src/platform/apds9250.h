/**
 * @file    apds9250.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_APDS9250_H_
#define PLAT_APDS9250_H_

/* Gain values for LS_GAIN */
typedef enum {
    APDS9250_GAIN_1 = 0,
    APDS9250_GAIN_3 = 1,
    APDS9250_GAIN_6 = 2,
    APDS9250_GAIN_9 = 3,
    APDS9250_GAIN_18 = 4
} APDS9250_Gain_t;

/* Resolution values for LS_MEAS_RATE */
typedef enum {
    APDS9250_RES_20BIT = 0x00, /* 400ms integration time */
    APDS9250_RES_19BIT = 0x10, /* 200ms */
    APDS9250_RES_18BIT = 0x20, /* 100ms (default) */
    APDS9250_RES_17BIT = 0x30, /* 50ms */
    APDS9250_RES_16BIT = 0x40, /* 25ms */
    APDS9250_RES_13BIT = 0x50  /* 3.125ms */
} APDS9250_Resolution_t;

/* Result RGB&IR */
typedef struct {
    uint32_t red;
    uint32_t green;
    uint32_t blue;
    uint32_t ir;
} APDS9250_RGB_t;

/* */
typedef struct {
    I2C_TypeDef *           i2c;
    APDS9250_Gain_t         gain;
    APDS9250_Resolution_t   resolution;
} PLAT_APDS9250_t;

/* */
typedef enum {
    APDS9250_OK = 0,
    APDS9250_IO_ERROR = -1,
    APDS9250_ID_ERROR = -2
} APDS9250_Status_t;

/* */
APDS9250_Status_t PLAT_APDS9250_Init(void);
APDS9250_Status_t PLAT_APDS9250_ReadRGB(APDS9250_RGB_t* rgb);
APDS9250_Status_t PLAT_APDS9250_ReadLuxInt(uint32_t* lux);

#endif /* PLAT_APDS9250_H_ */
