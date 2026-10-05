/**
 * @file    i2c.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_I2C_H_
#define PLAT_I2C_H_

/* */
typedef enum {
    I2C_SPEED_100KHZ,
    I2C_SPEED_400KHZ
} I2C_Speed_t;

/* */
typedef enum {
    I2C_OK = 0,
    I2C_ERR_PARAM,          /*!< Bad function param(s) */
    I2C_BUS_ERR,            /*!< Bus error */
    I2C_BUS_BUSY,           /*!< Bus busy */
    I2C_NACK_RECEIVED,      /*!< NACK received */
    I2C_ARBITRATION_LOST,   /*!< Arbitration lost */
    I2C_OVERRUN             /*!< Overrun/Underrun */
} I2C_Status_t;

/* */
void PLAT_I2C_Init(void);

I2C_Status_t PLAT_I2C_Xfer(I2C_TypeDef *i2c, uint8_t slave_addr,
    const uint8_t* wr_buf, uint8_t to_write,
    uint8_t* rd_buf, uint8_t to_read);

I2C_Status_t PLAT_I2C_Read(I2C_TypeDef *i2c, uint8_t slave_addr,
    uint8_t* rd_buf, uint8_t to_read);

#endif /* PLAT_I2C_H_ */
