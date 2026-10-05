/**
 * @file    i2c.c
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#include <stdint.h>
#include <stddef.h>
#include <stm32h562xx.h>

#include "platform/clock.h"
#include "platform/i2c.h"

/* TIMINGR = 0xB0422626 (100 kHz @ I2CCLK = 125 MHz, Standard-mode)  */
/* PRESC = 11 (div 12 -> t_presc = 96 ns)                            */
/* SCLDEL = 4, SDADEL = 2, SCLH = 0x26 (38), SCLL = 0x26 (38)        */
#define I2C_TIMINGR_100KHZ_125MHZ        \
    ((11U   << I2C_TIMINGR_PRESC_Pos)  | \
     (4U    << I2C_TIMINGR_SCLDEL_Pos) | \
     (2U    << I2C_TIMINGR_SDADEL_Pos) | \
     (0x26U << I2C_TIMINGR_SCLH_Pos)   | \
     (0x26U << I2C_TIMINGR_SCLL_Pos))

/* TIMINGR = 0x20310714 (400 kHz @ I2CCLK = 125 MHz, Fast-mode)      */
/* PRESC = 2 (div 3 -> t_presc = 24 ns)                              */
/* SCLDEL = 3, SDADEL = 1, SCLH = 0x07 (7), SCLL = 0x14 (20)         */
#define I2C_TIMINGR_400KHZ_125MHZ        \
    ((2U    << I2C_TIMINGR_PRESC_Pos)  | \
     (3U    << I2C_TIMINGR_SCLDEL_Pos) | \
     (1U    << I2C_TIMINGR_SDADEL_Pos) | \
     (0x07U << I2C_TIMINGR_SCLH_Pos)   | \
     (0x14U << I2C_TIMINGR_SCLL_Pos))

/* TIMINGR = 0x00300309 (1000 kHz @ I2CCLK = 125 MHz, Fast-mode Plus) */
/* PRESC = 0 (div 1 -> t_presc = 8 ns)                                */
/* SCLDEL = 3, SDADEL = 0, SCLH = 0x03 (3), SCLL = 0x09 (9)           */
#define I2C_TIMINGR_1000KHZ_125MHZ       \
    ((0U    << I2C_TIMINGR_PRESC_Pos)  | \
     (3U    << I2C_TIMINGR_SCLDEL_Pos) | \
     (0U    << I2C_TIMINGR_SDADEL_Pos) | \
     (0x03U << I2C_TIMINGR_SCLH_Pos)   | \
     (0x09U << I2C_TIMINGR_SCLL_Pos))

/* */
#define I2C_TIMEOUT_CYCLES   100000UL

/* */
static void I2C_Init(I2C_TypeDef *i2c, uint32_t timings)
{
    i2c->CR1  &= ~I2C_CR1_PE;
    i2c->TIMINGR = timings;
    i2c->CR1 |= I2C_CR1_PE;
}

/* */
void PLAT_I2C_Init(void)
{
    I2C_Init(I2C1, I2C_TIMINGR_100KHZ_125MHZ);
    I2C_Init(I2C2, I2C_TIMINGR_100KHZ_125MHZ);
    I2C_Init(I2C4, I2C_TIMINGR_100KHZ_125MHZ);

    __DSB();
    __ISB();
}

/* */
static I2C_Status_t I2C_PollStatus(I2C_TypeDef *i2c,
    uint32_t mask, uint32_t value, bool check_nack, I2C_Status_t err_status)
{
    uint32_t loop_cntr = 100000;

    do {
        uint32_t flags = i2c->ISR;

        if (flags & I2C_ISR_NACKF) {
            i2c->ICR = (I2C_ICR_NACKCF | I2C_ICR_STOPCF);
            return I2C_NACK_RECEIVED;
        }
        if (flags & I2C_ISR_BERR) {
            i2c->ICR = I2C_ICR_BERRCF;
            return I2C_BUS_ERR;
        }
        if (flags & I2C_ISR_ARLO) {
            i2c->ICR = I2C_ICR_ARLOCF;
            return I2C_ARBITRATION_LOST;
        }
        if (flags & I2C_ISR_OVR) {
            i2c->ICR = I2C_ICR_OVRCF;
            return I2C_OVERRUN;
        }
        if ((flags & mask) == value) {
            return I2C_OK;
        }
    }
    while(--loop_cntr > 0);

    return err_status;
}

/* */
I2C_Status_t PLAT_I2C_Xfer(I2C_TypeDef *i2c, uint8_t slave_addr,
    const uint8_t* wr_buf, uint8_t to_write,
    uint8_t* rd_buf, uint8_t to_read)
{
    if (i2c == NULL || (to_write == 0 && to_read == 0)) {
        return I2C_ERR_PARAM;
    }

    /* Waiting for bus IDLE */
    I2C_Status_t status = I2C_PollStatus(i2c, I2C_ISR_BUSY, 0, false, I2C_BUS_BUSY);
    if (status != I2C_OK) {
        return status;
    }

    /* Transmitt 1 byte of slave "address + to_write" */
    uint32_t cr2 = ((uint32_t)slave_addr << (I2C_CR2_SADD_Pos + 1)) |
                   ((uint32_t)to_write << I2C_CR2_NBYTES_Pos) |
                   I2C_CR2_START;

    if (to_read == 0) {
        cr2 |= I2C_CR2_AUTOEND;
    }

    i2c->CR2 = cr2;

    /* Serially sending an array of data (to_write bytes) */
    for (uint16_t i = 0; i < to_write; i++) {
        status = I2C_PollStatus(i2c, I2C_ISR_TXIS, I2C_ISR_TXIS, true, I2C_BUS_ERR);
        if (status != I2C_OK) {
            return status;
        }
        i2c->TXDR = wr_buf[i];
    }

    if (to_read > 0) {
        /* Waiting for Transfer Complete */
        status = I2C_PollStatus(i2c, I2C_ISR_TC, I2C_ISR_TC, true, I2C_BUS_ERR);
        if (status != I2C_OK) {
            return status;
        }

        /* Re-START and receive to_read bytes with AUTOEND */
        i2c->CR2 = ((uint32_t)slave_addr << (I2C_CR2_SADD_Pos + 1)) |
                   ((uint32_t)to_read << I2C_CR2_NBYTES_Pos) |
                   I2C_CR2_RD_WRN | I2C_CR2_AUTOEND | I2C_CR2_START;

        /* */
        for (uint16_t i = 0; i < to_read; i++) {
            status = I2C_PollStatus(i2c, I2C_ISR_RXNE, I2C_ISR_RXNE, true, I2C_BUS_ERR);
            if (status != I2C_OK) {
                return status;
            }
            rd_buf[i] = (uint8_t)(i2c->RXDR & I2C_RXDR_RXDATA);
        }
    }

    /* */
    status = I2C_PollStatus(i2c, I2C_ISR_STOPF, I2C_ISR_STOPF, false, I2C_BUS_ERR);
    if (status != I2C_OK) {
        return status;
    }
    i2c->ICR = I2C_ICR_STOPCF;

    return I2C_OK;
}

I2C_Status_t PLAT_I2C_Read(I2C_TypeDef *i2c, uint8_t slave_addr,
    uint8_t* rd_buf, uint8_t to_read)
{
    if (i2c == NULL || rd_buf == NULL || to_read == 0) {
        return I2C_ERR_PARAM;
    }

    /* Waiting for bus IDLE */
    I2C_Status_t status = I2C_PollStatus(i2c, I2C_ISR_BUSY, 0, false, I2C_BUS_BUSY);
    if (status != I2C_OK) {
        return status;
    }

    /* */
    i2c->CR2 = ((uint32_t)slave_addr << (I2C_CR2_SADD_Pos + 1)) |
               ((uint32_t)to_read << I2C_CR2_NBYTES_Pos) |
               I2C_CR2_RD_WRN | I2C_CR2_AUTOEND | I2C_CR2_START;

    /* */
    for (uint16_t i = 0; i < to_read; i++) {
        status = I2C_PollStatus(i2c, I2C_ISR_RXNE, I2C_ISR_RXNE, true, I2C_BUS_ERR);
        if (status != I2C_OK) {
            return status;
        }
        rd_buf[i] = (uint8_t)(i2c->RXDR & I2C_RXDR_RXDATA);
    }

    /* */
    status = I2C_PollStatus(i2c, I2C_ISR_STOPF, I2C_ISR_STOPF, false, I2C_BUS_ERR);
    if (status != I2C_OK) {
        return status;
    }
    i2c->ICR = I2C_ICR_STOPCF;

    return I2C_OK;
}
