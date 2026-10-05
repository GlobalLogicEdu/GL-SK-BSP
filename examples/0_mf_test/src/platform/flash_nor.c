/**
 * @file    flash_nor.c
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
#include <string.h>
#include <stm32h562xx.h>

#include "platform/spi.h"
#include "platform/gpio.h"
#include "platform/systick.h"
#include "platform/flash_nor.h"

/* */
#define W25Q_CMD_WRITE_ENABLE   0x06
#define W25Q_CMD_READ_STATUS1   0x05
#define W25Q_CMD_PAGE_PROGRAM   0x02
#define W25Q_CMD_READ_DATA      0x03
#define W25Q_CMD_SECTOR_ERASE   0x20   /* 4KB */
#define W25Q_CMD_JEDEC_ID       0x9F
#define W25Q_STATUS_BUSY        0x01
#define W25Q_STATUS_WEL         0x02

/* */
extern PLAT_NORFL_t g_pNORFL;

/* */
static uint8_t NORFL_ReadStatus(void)
{
    uint8_t tx[2] = { W25Q_CMD_READ_STATUS1, 0 };
    uint8_t rx[2];

    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 0);
    PLAT_SPI_FullXfer(g_pNORFL.spi, tx, rx, 2);
    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 1);

    return rx[1];
}

static void NORFL_WaitBusy(void)
{
    while (NORFL_ReadStatus() & W25Q_STATUS_BUSY);
}

static void NORFL_WriteEnable(void)
{
    uint8_t cmd = W25Q_CMD_WRITE_ENABLE;

    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 0);
    PLAT_SPI_FullXfer(g_pNORFL.spi, &cmd, NULL, 1);
    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 1);
}

/* */
void PLAT_NORFL_ReadJEDECID(uint8_t id[3])
{
    uint8_t tx[4] = { W25Q_CMD_JEDEC_ID, 0, 0, 0 };
    uint8_t rx[4];

    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 0);
    PLAT_SPI_FullXfer(g_pNORFL.spi, tx, rx, 4);
    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 1);

    memcpy(id, &rx[1], 3);
}

/* */
void PLAT_NORFL_ReadManufDeviceID(uint8_t* manuf, uint8_t* dev)
{
    uint8_t tx[6] = { 0x90, 0, 0, 0, 0, 0 }; /* cmd + 3 address bytes (0) + 2 dummy */
    uint8_t rx[6];

    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 0);
    PLAT_SPI_FullXfer(g_pNORFL.spi, tx, rx, 6);
    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 1);

    *manuf = rx[4];
    *dev = rx[5];
}

/* */
void PLAT_NORFL_Read(uint32_t addr, uint8_t* buf, uint32_t len)
{
    const uint8_t hdr[4] = {
        W25Q_CMD_READ_DATA,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)(addr)
    };

    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 0);
    PLAT_SPI_FullXfer(g_pNORFL.spi, hdr, NULL, 4); /* send command+address, discard dummy rx */
    PLAT_SPI_FullXfer(g_pNORFL.spi, NULL, buf, len); /* clock out len bytes, capture rx */
    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 1);
}

/* Programs up to one page (256 bytes) at a time. addr/len must not cross
 * a page boundary in a single call - see PLAT_NORFL_Write() below for that. */
static void NORFL_ProgramPage(uint32_t addr, const uint8_t* data, uint32_t len)
{
    const uint8_t hdr[4] = {
        W25Q_CMD_PAGE_PROGRAM,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)(addr)
    };

    NORFL_WriteEnable();

    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 0);
    PLAT_SPI_FullXfer(g_pNORFL.spi, hdr, NULL, 4);
    PLAT_SPI_FullXfer(g_pNORFL.spi, data, NULL, len);
    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 1);

    NORFL_WaitBusy();
}

/* General-purpose write: splits across page boundaries automatically.
 * Caller is responsible for having erased the target sectors first -
 * NOR flash can only clear bits to 0 on program; erase sets them back to 1. */
void PLAT_NORFL_Write(uint32_t addr, const uint8_t* data, uint32_t len)
{
    const uint32_t page_size = g_pNORFL.page_size;

    while (len) {
        uint32_t page_offset = addr % page_size;
        uint32_t chunk = page_size - page_offset;

        if (chunk > len) {
            chunk = len;
        }

        NORFL_ProgramPage(addr, data, chunk);

        addr += chunk;
        data += chunk;
        len -= chunk;
    }
}

void PLAT_NORFL_SectorErase(uint32_t addr)
{
    const uint8_t hdr[4] = {
        W25Q_CMD_SECTOR_ERASE,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)(addr)
    };

    NORFL_WriteEnable();

    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 0);
    PLAT_SPI_FullXfer(g_pNORFL.spi, hdr, NULL, 4);
    PLAT_GPIO_SetPinState(&g_pNORFL.cs_pin, 1);

    NORFL_WaitBusy(); /* sector erase can take up to ~400ms per datasheet */
}
