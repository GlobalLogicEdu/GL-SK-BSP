/**
 * @file    spi.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_SPI_H_
#define PLAT_SPI_H_

/* */
typedef enum {
    SPI_Mode0_FullDuplex = 0, /* CPOL = 0, CPHA = 0, 4-wire */
    SPI_Mode0_HalfDuplex = 1, /* CPOL = 0, CPHA = 0, 3-wire */
} SPI_Mode_t;

/* */
typedef enum {
    SPI_PclkDiv2 = 0,   /* SPI master clock/2 */
    SPI_PclkDiv4 = 1,   /* /4 */
    SPI_PclkDiv8 = 2,   /* /8 */
    SPI_PclkDiv16 = 3,  /* /16 */
    SPI_PclkDiv32 = 4,  /* /32 */
    SPI_PclkDiv64 = 5,  /* /64 */
    SPI_PclkDiv128 = 6, /* /128 */
    SPI_PclkDiv256 = 7, /* /256 */
} SPI_PclkDiv_t;

/* */
void PLAT_SPI_Init(SPI_TypeDef* spi, SPI_Mode_t mode, SPI_PclkDiv_t pclk_div);
void PLAT_SPI_FullXfer(SPI_TypeDef* spi, const uint8_t* tx, uint8_t* rx, uint16_t len);
void PLAT_SPI_HalfWrite(SPI_TypeDef* spi, const uint8_t* tx, uint16_t len);
void PLAT_SPI_HalfRead(SPI_TypeDef* spi, uint8_t* rx, uint16_t len);

#endif /* PLAT_SPI_H_ */
