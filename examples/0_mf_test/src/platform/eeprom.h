/**
 * @file    eeprom.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_EEPROM_H_
#define PLAT_EEPROM_H_

/* */
typedef struct {
    I2C_TypeDef *   i2c;
    uint8_t         i2c_addr;
    uint32_t        capacity;
} PLAT_EEPROM_t;

/* */
int PLAT_EEPROM_Read(uint32_t addr, uint8_t *rd_buf, size_t to_read);

/*
 [addr]         - Address of data to be written
 [wr_buf]       - Intermediate buffer, size must be [data_size] + 1
 [data]         - Data to be written
 [data_size]    - Bytes count in [data] to be written
*/
int PLAT_EEPROM_Write(uint32_t addr, uint8_t *wr_buf, const uint8_t *data, size_t data_size);

#endif /* PLAT_EEPROM_H_ */
