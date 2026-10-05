/**
 * @file    lis2mdl.c
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

#include "platform/spi.h"
#include "platform/gpio.h"
#include "platform/systick.h"
#include "platform/lis2mdl.h"

/* LIS2MDL Registers */
#define LIS2MDL_REG_OFFSET_X_REG_L    0x45
#define LIS2MDL_REG_WHO_AM_I          0x4F  // Expected response: 0x40
#define LIS2MDL_REG_CFG_REG_A         0x60  // ODR, Mode, Temperature Comp
#define LIS2MDL_REG_CFG_REG_B         0x61  // LPF, Offset cancellation
#define LIS2MDL_REG_CFG_REG_C         0x62  // SPI 3-wire/4-wire, DRDY pin
#define LIS2MDL_REG_STATUS_REG        0x67  // Data Ready Status
#define LIS2MDL_REG_OUTX_L_REG        0x68  // Mag Data Out (X_L)

/* LIS2MDL sensitivity is fixed: 1.5 mG/LSB = 0.0015 Gauss/LSB = 0.15 µT/LSB */
#define LIS2MDL_SENSITIVITY_MG_LSB    1.5f
#define LIS2MDL_SENSITIVITY_UT_LSB    0.15f

/* */
extern const PLAT_LIS2MDL_t g_pLIS2MDL;

/* CS stays low across the address phase and the data phase */
static void LIS2MDL_ReadRegs(uint8_t reg, uint8_t* buf, uint16_t n)
{
    uint8_t cmd = reg | 0x80U; /* bit7 = read */
    PLAT_GPIO_SetPinState(&g_pLIS2MDL.cs_pin, 0);
    PLAT_SPI_HalfWrite(g_pLIS2MDL.spi, &cmd, 1);
    PLAT_SPI_HalfRead(g_pLIS2MDL.spi, buf, n);
    PLAT_GPIO_SetPinState(&g_pLIS2MDL.cs_pin, 1);
}

static void LIS2MDL_WriteReg(uint8_t reg, uint8_t val)
{
    uint8_t reg_val[2] = { (uint8_t)(reg & 0x7FU), val };
    PLAT_GPIO_SetPinState(&g_pLIS2MDL.cs_pin, 0);
    PLAT_SPI_HalfWrite(g_pLIS2MDL.spi, reg_val, 2);
    PLAT_GPIO_SetPinState(&g_pLIS2MDL.cs_pin, 1);
}

/* */
bool PLAT_LIS2MDL_Init(void)
{
    /* Deassert CS pin */
    PLAT_GPIO_SetPinState(&g_pLIS2MDL.cs_pin, 1);
    PLAT_SYSTICK_DelayMs(10);

    /* Software Reset & Reboot */
    LIS2MDL_WriteReg(LIS2MDL_REG_CFG_REG_A, (1U << 5) | (1U << 6));
    PLAT_SYSTICK_DelayMs(20);

    /* CFG_REG_C (0x62) = 0x10 (BDU=1, I2C=0, 3-wire SPI) */
    LIS2MDL_WriteReg(LIS2MDL_REG_CFG_REG_C, (1U << 5) | (1U << 4));
    PLAT_SYSTICK_DelayMs(5);

    /* Read WHO_AM_I register */
    uint8_t id = 0;
    LIS2MDL_ReadRegs(LIS2MDL_REG_WHO_AM_I, &id, 1);
    if (id != 0x40) {
        return false;
    }

    /* Configure CFG_REG_A: */
    /* Bit 7: COMP_TEMP_EN = 1 (Temperature compensation enabled) */
    /* Bit 3 - 2: ODR = 10 (100 Hz ODR) */
    /* Bit 1 - 0: MD = 00 (Continuous mode) */
    /* Value = 0x80 | (0x02 << 2) | 0x00 = 0x8C  */
    LIS2MDL_WriteReg(LIS2MDL_REG_CFG_REG_A, 0x8C);
    PLAT_SYSTICK_DelayMs(15);

    /* Configure CFG_REG_B: */
    /* Bit 0: LPF = 1 (Enable Low Pass Filter) */
    LIS2MDL_WriteReg(LIS2MDL_REG_CFG_REG_B, 0x01);

    return true;
}

/* */
bool PLAT_LIS2MDL_Read(LIS2MDL_Data_t* data)
{
//    uint8_t status = 0;
//    LIS2MDL_ReadRegs(LIS2MDL_REG_STATUS_REG, &status, 1);

    uint8_t raw_buf[6];
    LIS2MDL_ReadRegs(LIS2MDL_REG_OUTX_L_REG, raw_buf, 6);

    /* Gathering 16-bits values (Little Endian) */
    data->mag_raw[0] = (int16_t)((raw_buf[1] << 8) | raw_buf[0]); // Axis X
    data->mag_raw[1] = (int16_t)((raw_buf[3] << 8) | raw_buf[2]); // Axis Y
    data->mag_raw[2] = (int16_t)((raw_buf[5] << 8) | raw_buf[4]); // Axis Z

    /* Convert raw values to physical units (milliGauss and microTesla) */
    data->mag_uT[0] = (float)data->mag_raw[0] * LIS2MDL_SENSITIVITY_UT_LSB;
    data->mag_uT[1] = (float)data->mag_raw[1] * LIS2MDL_SENSITIVITY_UT_LSB;
    data->mag_uT[2] = (float)data->mag_raw[2] * LIS2MDL_SENSITIVITY_UT_LSB;

    data->mag_mG[0] = (float)data->mag_raw[0] * LIS2MDL_SENSITIVITY_MG_LSB;
    data->mag_mG[1] = (float)data->mag_raw[1] * LIS2MDL_SENSITIVITY_MG_LSB;
    data->mag_mG[2] = (float)data->mag_raw[2] * LIS2MDL_SENSITIVITY_MG_LSB;

    return true;
}

/* LIS2MDL_INT# */
void PLAT_IRQ_EXTI10_Handler(void)
{
    EXTI_NS->FPR1 = (1UL << 10);
}
