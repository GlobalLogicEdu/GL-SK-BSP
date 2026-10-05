/**
 * @file    htu21d.c
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
#include "platform/htu21d.h"

/* */
#define HTU21D_I2C_ADDR             0x40

#define HTU21D_CMD_TRIG_TEMP_NOHOLD 0xF3
#define HTU21D_CMD_TRIG_HUM_NOHOLD  0xF5
#define HTU21D_CMD_SOFT_RESET       0xFE

/* */
extern const PLAT_HTU21D_t g_pHTU21D;

/* */
static bool HTU21D_CheckCRC(uint16_t data, uint8_t checksum)
{
    uint32_t remainder = (uint32_t)data << 8;
    remainder |= checksum;

    uint32_t divisor = 0x988000U;

    for (int i = 0; i < 16; i++) {
        if (remainder & (1U << (23 - i))) {
            remainder ^= divisor;
        }
        divisor >>= 1;
    }

    return (remainder == 0);
}

/* */
bool PLAT_HTU21D_Init(void)
{
    uint8_t cmd = HTU21D_CMD_SOFT_RESET;

    if (PLAT_I2C_Xfer(g_pHTU21D.i2c, HTU21D_I2C_ADDR, &cmd, 1, NULL, 0) != I2C_OK) {
        return false;
    }

    PLAT_SYSTICK_DelayMs(15);
    return true;
}

bool PLAT_HTU21D_ReadTempInt(int32_t *temp_x100)
{
    uint8_t cmd = HTU21D_CMD_TRIG_TEMP_NOHOLD;
    uint8_t rx_buf[3] = {0};

    if (PLAT_I2C_Xfer(g_pHTU21D.i2c, HTU21D_I2C_ADDR, &cmd, 1, NULL, 0) != I2C_OK) {
        return false;
    }

    PLAT_SYSTICK_DelayMs(50);

    if (PLAT_I2C_Read(g_pHTU21D.i2c, HTU21D_I2C_ADDR, rx_buf, 3) != I2C_OK) {
        return false;
    }

    uint16_t raw_temp = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];

    if (!HTU21D_CheckCRC(raw_temp, rx_buf[2])) {
        return false;
    }

    raw_temp &= 0xFFFC; /* Status bit-mask */

    /* T_x100 = -4685 + (4393 * raw_temp) / 16384 */
    *temp_x100 = -4685 + (int32_t)(((uint32_t)raw_temp * 4393U) >> 14);

    return true;
}

/* */
bool PLAT_HTU21D_ReadHumInt(int32_t *hum_x100)
{
    uint8_t cmd = HTU21D_CMD_TRIG_HUM_NOHOLD;
    uint8_t rx_buf[3] = {0};

    if (PLAT_I2C_Xfer(g_pHTU21D.i2c, HTU21D_I2C_ADDR, &cmd, 1, NULL, 0) != I2C_OK) {
        return false;
    }

    PLAT_SYSTICK_DelayMs(16);

    if (PLAT_I2C_Read(g_pHTU21D.i2c, HTU21D_I2C_ADDR, rx_buf, 3) != I2C_OK) {
        return false;
    }

    uint16_t raw_hum = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];

    if (!HTU21D_CheckCRC(raw_hum, rx_buf[2])) {
        return false;
    }

    raw_hum &= 0xFFFC;

    /* RH_x100 = -600 + (3125 * raw_hum) / 16384 */
    int32_t rh = -600 + (int32_t)(((uint32_t)raw_hum * 3125U) >> 14);

    /* 0..10000 (0.00% .. 100.00%) */
    if (rh < 0) rh = 0;
    if (rh > 10000) rh = 10000;

    *hum_x100 = rh;

    return true;
}
