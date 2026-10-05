/**
 * @file    apds9250.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stddef.h>
#include <stdint.h>
#include <stm32h562xx.h>

#include "platform/i2c.h"
#include "platform/systick.h"
#include "platform/apds9250.h"

/* */
#define APDS9250_I2C_ADDR           0x52

/* APDS-9250 Registers */
#define APDS9250_REG_MAIN_CTRL      0x00
#define APDS9250_REG_LS_MEAS_RATE   0x04
#define APDS9250_REG_LS_GAIN        0x05
#define APDS9250_REG_PART_ID        0x06
#define APDS9250_REG_MAIN_STATUS    0x07

/* Data Registers (24-bit: LSB, MSB, HSB) */
#define APDS9250_REG_DATA_IR_0      0x0A
#define APDS9250_REG_DATA_GREEN_0   0x0D
#define APDS9250_REG_DATA_RED_0     0x10
#define APDS9250_REG_DATA_BLUE_0    0x13

/* Configuration values */
#define APDS9250_PART_ID_VAL        0xB2

// Modes for MAIN_CTRL (Reg 0x00)
#define APDS9250_CTRL_LS_EN         (1U << 1)
#define APDS9250_CTRL_RGB_MODE      (1U << 2) /* 1: RGB+IR mode, 0: ALS+IR mode */

/* */
extern const PLAT_APDS9250_t g_pAPDS9250;

/* */
static I2C_Status_t APDS9250_WriteReg(uint8_t reg, uint8_t val)
{
    const uint8_t buf[2] = {reg, val};
    return PLAT_I2C_Xfer(g_pAPDS9250.i2c, APDS9250_I2C_ADDR, buf, 2, NULL, 0);
}

/* */
static I2C_Status_t APDS9250_ReadReg(uint8_t reg, uint8_t *data, uint8_t len)
{
    return PLAT_I2C_Xfer(g_pAPDS9250.i2c, APDS9250_I2C_ADDR, &reg, 1, data, len);
}

/* */
APDS9250_Status_t PLAT_APDS9250_Init(void)
{
    uint8_t part_id = 0;

    /* Check Part ID */
    if (APDS9250_ReadReg(APDS9250_REG_PART_ID, &part_id, 1) != I2C_OK) {
        return APDS9250_IO_ERROR;
    }

    if ((part_id & 0xF0) != (APDS9250_PART_ID_VAL & 0xF0)) {
        return APDS9250_ID_ERROR; /* Unknown part */
    }

    /* */
    if (APDS9250_WriteReg(APDS9250_REG_LS_GAIN, (uint8_t)g_pAPDS9250.gain) != I2C_OK) {
        return APDS9250_IO_ERROR;
    }

    /* 0x05 - (100ms measurement rate) */
    if (APDS9250_WriteReg(APDS9250_REG_LS_MEAS_RATE, (uint8_t)g_pAPDS9250.resolution | 0x05) != I2C_OK) {
        return APDS9250_IO_ERROR;
    }

    /* Enable Light Sensor in a mode RGB+IR (Reg 0x00 = LS_EN | RGB_MODE) */
    if (APDS9250_WriteReg(APDS9250_REG_MAIN_CTRL, APDS9250_CTRL_LS_EN | APDS9250_CTRL_RGB_MODE) != I2C_OK) {
        return APDS9250_IO_ERROR;
    }

    PLAT_SYSTICK_DelayMs(10);
    return APDS9250_OK;
}

/* */
APDS9250_Status_t PLAT_APDS9250_ReadRGB(APDS9250_RGB_t* rgb)
{
    uint8_t buf[12] = {0};

    /* Read 12 bytes beginning from register DATA_IR_0 (0x0A) */
    if (APDS9250_ReadReg(APDS9250_REG_DATA_IR_0, buf, 12) != I2C_OK) {
        return APDS9250_IO_ERROR;
    }

    /* */
    rgb->ir    = (uint32_t)buf[0]  | ((uint32_t)buf[1]  << 8) | ((uint32_t)buf[2]  << 16);
    rgb->green = (uint32_t)buf[3]  | ((uint32_t)buf[4]  << 8) | ((uint32_t)buf[5]  << 16);
    rgb->red   = (uint32_t)buf[6]  | ((uint32_t)buf[7]  << 8) | ((uint32_t)buf[8]  << 16);
    rgb->blue  = (uint32_t)buf[9]  | ((uint32_t)buf[10] << 8) | ((uint32_t)buf[11] << 16);

    return APDS9250_OK;
}

/* */
APDS9250_Status_t PLAT_APDS9250_ReadLuxInt(uint32_t* lux)
{
    APDS9250_RGB_t rgb;
    if (PLAT_APDS9250_ReadRGB(&rgb) != APDS9250_OK) {
        return APDS9250_IO_ERROR;
    }

    /* For Gain=3x and Res=18-bit coeficient is avr. 46 counts/lux */
    /* Lux = Green_Count / 46 */
    *lux = rgb.green / 46U;

    return APDS9250_OK;
}

/* APDS9250_INT# */
void PLAT_IRQ_EXTI15_Handler(void)
{
    EXTI_NS->FPR1 = (1UL << 15);
}
