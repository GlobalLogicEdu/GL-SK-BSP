/**
 * @file    lsm6dso.c
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
#include "platform/lsm6dso.h"

/* LSM6DSO Registers */
#define LSM6DSO_REG_FUNC_CFG_ACCESS    0x01
#define LSM6DSO_REG_WHO_AM_I           0x0F  /* Expected response: 0x6C */
#define LSM6DSO_REG_CTRL1_XL           0x10  /* Accelerometer Control */
#define LSM6DSO_REG_CTRL2_G            0x11  /* Gyroscope Control */
#define LSM6DSO_REG_CTRL3_C            0x12  /* Control Register 3 */
#define LSM6DSO_REG_CTRL4_C            0x13
#define LSM6DSO_REG_STATUS_REG         0x1E  /* Data Ready Status */
#define LSM6DSO_REG_OUTX_L_G           0x22  /* Gyro Data Out (X_L) */
#define LSM6DSO_REG_OUTX_L_A           0x28  /* Accel Data Out (X_L) */

/* */
extern const PLAT_LSM6DSO_t g_pLSM6DSO;

/* CS stays low across the address phase and the data phase */
static void LSM6DSO_ReadRegs(uint8_t reg, uint8_t* buf, uint16_t n)
{
    uint8_t cmd = reg | 0x80U; /* bit7 = read */
    PLAT_GPIO_SetPinState(&g_pLSM6DSO.cs_pin, 0);
    PLAT_SPI_HalfWrite(g_pLSM6DSO.spi, &cmd, 1);
    PLAT_SPI_HalfRead(g_pLSM6DSO.spi, buf, n);
    PLAT_GPIO_SetPinState(&g_pLSM6DSO.cs_pin, 1);
}

static void LSM6DSO_WriteReg(uint8_t reg, uint8_t val)
{
    uint8_t reg_val[2] = { (uint8_t)(reg & 0x7FU), val };
    PLAT_GPIO_SetPinState(&g_pLSM6DSO.cs_pin, 0);
    PLAT_SPI_HalfWrite(g_pLSM6DSO.spi, reg_val, 2);
    PLAT_GPIO_SetPinState(&g_pLSM6DSO.cs_pin, 1);
}

/* */
bool PLAT_LSM6DSO_Init(void)
{
    /* Deassert CS pin */
    PLAT_GPIO_SetPinState(&g_pLSM6DSO.cs_pin, 1);
    PLAT_SYSTICK_DelayMs(10);

    /* LSM6DSO: switch to 3-wire */
    LSM6DSO_WriteReg(LSM6DSO_REG_CTRL3_C, 0x4C); /* CTRL3_C: BDU=1, IF_INC=1, SIM=1 */
    LSM6DSO_WriteReg(LSM6DSO_REG_CTRL4_C, 0x04); /* CTRL4_C: I2C_disable */

    /* Read WHO_AM_I */
    uint8_t id = 0;
    LSM6DSO_ReadRegs(LSM6DSO_REG_WHO_AM_I, &id, 1);
    if (id != 0x6C) {
        return false;
    }

    /* Software Reset */
    LSM6DSO_WriteReg(LSM6DSO_REG_CTRL3_C, 0x01);
    PLAT_SYSTICK_DelayMs(10);

    /* Auto-increment configuration (IF_INC = 1) */
    LSM6DSO_WriteReg(LSM6DSO_REG_CTRL3_C, 0x04);

    /* Configure Accelerometer: 104 Hz ODR, +/-2g Scale */
    /* CTRL1_XL = 0x40 -> ODR_XL = 104Hz (0100b), FS_XL = 2g (00b) */
    LSM6DSO_WriteReg(LSM6DSO_REG_CTRL1_XL, 0x40);

    /* Configure Gyroscope: 104 Hz ODR, +/-2000 dps Scale */
    /* CTRL2_G = 0x4C -> ODR_G = 104Hz (0100b), FS_G = 2000dps (11b) */
    LSM6DSO_WriteReg(LSM6DSO_REG_CTRL2_G, 0x4C);

    return true;
}

/* LSM6DSO_INT2# */
void PLAT_IRQ_EXTI8_Handler(void)
{
    EXTI_NS->FPR1 = (1UL << 8);
}

/* LSM6DSO_INT1 */
void PLAT_IRQ_EXTI9_Handler(void)
{
    EXTI_NS->RPR1 = (1UL << 9);
}
