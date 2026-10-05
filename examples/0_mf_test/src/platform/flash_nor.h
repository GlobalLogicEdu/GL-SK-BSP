/**
 * @file    flash_nor.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_NOR_FLASH_H_
#define PLAT_NOR_FLASH_H_

/* */
typedef struct {
    SPI_TypeDef*    spi;
    GPIO_Pin_t      cs_pin;
    uint32_t        page_size;
} PLAT_NORFL_t;

/* */
void PLAT_NORFL_ReadJEDECID(uint8_t id[3]);
void PLAT_NORFL_ReadManufDeviceID(uint8_t* manuf, uint8_t* dev);
void PLAT_NORFL_Read(uint32_t addr, uint8_t* buf, uint32_t len);
void PLAT_NORFL_Write(uint32_t addr, const uint8_t* data, uint32_t len);
void PLAT_NORFL_SectorErase(uint32_t addr);

#endif /* PLAT_NOR_FLASH_H_ */
