/**
 * @file    spi.c
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
#include "platform/spi.h"

/* */
void PLAT_SPI_Init(SPI_TypeDef* spi, SPI_Mode_t mode, SPI_PclkDiv_t pclk_div)
{
    /* disable SPI, clear config, before reconfiguring */
    spi->CR1 = SPI_CR1_SSI; /* internal SS high - required with SSM=1 to avoid MODF */

    /* */
    spi->CFG1 = (7U << SPI_CFG1_DSIZE_Pos) | /* 8-bit frame  */
                (pclk_div << SPI_CFG1_MBR_Pos);

    /* CFG2: master, motorola mode, full duplex, CPOL=0/CPHA=0 (SPI mode 0) */
    spi->CFG2 = SPI_CFG2_MASTER |
                SPI_CFG2_SSM; /* CS is software GPIO */

    if (mode == SPI_Mode0_HalfDuplex) {
        spi->CFG2 |= (3U << SPI_CFG2_COMM_Pos);
    }

    /* enable the peripheral (transfers still need CSTART) */
    spi->CR1 |= SPI_CR1_SPE;
}

/* */
void PLAT_SPI_FullXfer(SPI_TypeDef* spi, const uint8_t* tx, uint8_t* rx, uint16_t len)
{
    if (len == 0) {
        return;
    }

    spi->CR2 = len;             /* TSIZE = number of frames in this transaction */
    spi->CR1 |= SPI_CR1_CSTART; /* master: transfer only starts once CSTART is set */

    for (uint16_t i = 0; i < len; i++) {
        uint8_t txbyte = tx ? tx[i] : 0xFFU;

        while (!(spi->SR & SPI_SR_TXP)); /* wait for TX room */
        *(volatile uint8_t *)&spi->TXDR = txbyte;

        while (!(spi->SR & SPI_SR_RXP)); /* wait for RX data */
        volatile uint8_t rxbyte = *(volatile uint8_t *)&spi->RXDR;

        if (rx) {
            rx[i] = rxbyte;
        }
    }

    while (!(spi->SR & SPI_SR_EOT)); /* wait for end-of-transfer */

    /* clear EOT / TXTF flags */
    spi->IFCR = SPI_IFCR_EOTC |
                SPI_IFCR_TXTFC;
}

/* */
void PLAT_SPI_HalfWrite(SPI_TypeDef* spi, const uint8_t* tx, uint16_t len)
{
    spi->CR1 |= SPI_CR1_HDDIR; /* transmit direction */
    spi->CR2 = len;
    spi->CR1 |= SPI_CR1_CSTART;

    for (uint16_t i = 0; i < len; i++) {
        while (!(spi->SR & SPI_SR_TXP));
        *(volatile uint8_t *)&spi->TXDR = tx[i];
    }

    while (!(spi->SR & SPI_SR_EOT));

    /* clear EOT / TXTF flags */
    spi->IFCR = SPI_IFCR_EOTC |
                SPI_IFCR_TXTFC;
}

void PLAT_SPI_HalfRead(SPI_TypeDef* spi, uint8_t* rx, uint16_t len)
{
    spi->CR1 &= ~SPI_CR1_HDDIR; /* receive direction */
    spi->CR2 = len;
    spi->CR1 |= SPI_CR1_CSTART;

    for (uint16_t i = 0; i < len; i++) {
        while (!(spi->SR & SPI_SR_RXP));
        rx[i] = *(volatile uint8_t *)&spi->RXDR;
    }

    while (!(spi->SR & SPI_SR_EOT));

    /* clear EOT / TXTF flags */
    spi->IFCR = SPI_IFCR_EOTC |
                SPI_IFCR_TXTFC;
}
