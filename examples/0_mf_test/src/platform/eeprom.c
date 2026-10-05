/**
 * @file    eeprom.c
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
#include <memory.h>
#include <stm32h562xx.h>

#include "platform/i2c.h"
#include "platform/eeprom.h"

/* */
extern const PLAT_EEPROM_t g_pEEPROM;

/* */
int PLAT_EEPROM_Read(uint32_t addr, uint8_t *rd_buf, size_t to_read)
{
    uint8_t addr_a8_a11 = (addr >> 8) & 0x0f;
    uint8_t addr_a0_a7 = (addr & 0xff);

    /* Limit length by capacity */
    size_t avail = (g_pEEPROM.capacity - addr);

    if (to_read > avail) {
        to_read = avail;
    }

    if (PLAT_I2C_Xfer(g_pEEPROM.i2c, g_pEEPROM.i2c_addr | addr_a8_a11,
                        &addr_a0_a7, 1, rd_buf, to_read) != I2C_OK) {
        return -1;
    }

    return (int)to_read;
}

int PLAT_EEPROM_Write(uint32_t addr, uint8_t *wr_buf, const uint8_t *data, size_t data_size)
{
    uint8_t addr_a8_a11 = (addr >> 8) & 0x0f;
    uint8_t addr_a0_a7 = (addr & 0xff);

    /* Limit length by capacity */
    size_t avail = (g_pEEPROM.capacity - addr);

    if (data_size > avail) {
        data_size = avail;
    }

    wr_buf[0] = addr_a0_a7;
    memcpy(&wr_buf[1], data, data_size);

    if (PLAT_I2C_Xfer(g_pEEPROM.i2c, g_pEEPROM.i2c_addr | addr_a8_a11,
                        wr_buf, data_size + 1, NULL, 0) != I2C_OK) {
        return -1;
    }

    return (int)data_size;
}
