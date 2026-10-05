/**
 * @file    board.c
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

#include "platform/gpio.h"
#include "platform/hd44780.h"
#include "platform/eeprom.h"
#include "platform/htu21d.h"
#include "platform/apds9250.h"
#include "platform/pwm_pca9685.h"
#include "platform/lsm6dso.h"
#include "platform/lis2mdl.h"
#include "platform/flash_nor.h"
#include "platform/gpio_keypad.h"

/* -------------------------------------------------------------------- */
const PLAT_GpioCfg_t g_pGpioList[] =
{
    /* --- Debug UART --- */
    GPIO_CFG(GPIOA_NS,  GPIO_BIT10, GPIO_AF3,       GPIO_IN | GPIO_PD),             // VCP_RX (LPUART1_RX)
    GPIO_CFG(GPIOA_NS,  GPIO_BIT9,  GPIO_AF3,       GPIO_OUT_PP_FAST),              // VCP_TX (LPUART1_TX)

    /* --- I2C --- */
    /* EEPROM */
    GPIO_CFG(GPIOB_NS, GPIO_BIT8,   GPIO_AF4,         GPIO_OUT_OD_MAX),             // I2C1_SCL
    GPIO_CFG(GPIOB_NS, GPIO_BIT9,   GPIO_AF4,         GPIO_OUT_OD_MAX),             // I2C1_SDA

    GPIO_CFG(GPIOF_NS, GPIO_BIT1,   GPIO_AF4,         GPIO_OUT_OD_MAX),             // I2C2_SCL
    GPIO_CFG(GPIOF_NS, GPIO_BIT0,   GPIO_AF4,         GPIO_OUT_OD_MAX),             // I2C2_SDA

    /* HTU21, APDS-9250, PCA9685BS */
    GPIO_CFG(GPIOD_NS, GPIO_BIT12,  GPIO_AF4,         GPIO_OUT_OD_MAX),             // I2C4_SCL
    GPIO_CFG(GPIOD_NS, GPIO_BIT13,  GPIO_AF4,         GPIO_OUT_OD_MAX),             // I2C4_SDA

    /* --- SPI --- */
    /* LIS2MDL, LSM6DSO */
    GPIO_CFG(GPIOB_NS, GPIO_BIT13,  GPIO_AF5,         GPIO_OUT_PP_MAX),             // SPI2_SCK
    GPIO_CFG(GPIOB_NS, GPIO_BIT15,  GPIO_AF5,         GPIO_OUT_PP_MAX),             // SPI2_MOSI (DIO)
    GPIO_CFG(GPIOB_NS, GPIO_BIT12,  GPIO_AF_NONE,     GPIO_OUT_PP_MAX | GPIO_PU),   // SPI2_CS#0
    GPIO_CFG(GPIOB_NS, GPIO_BIT10,  GPIO_AF_NONE,     GPIO_OUT_PP_MAX | GPIO_PU),   // SPI2_CS#1

    /* W25Q16JVSNIQ */
    GPIO_CFG(GPIOE_NS, GPIO_BIT2,   GPIO_AF5,         GPIO_OUT_PP_MAX),             // SPI4_SCK
    GPIO_CFG(GPIOE_NS, GPIO_BIT5,   GPIO_AF5,         GPIO_IN),                     // SPI4_MISO
    GPIO_CFG(GPIOE_NS, GPIO_BIT6,   GPIO_AF5,         GPIO_OUT_PP_MAX),             // SPI4_MOSI
    GPIO_CFG(GPIOE_NS, GPIO_BIT4,   GPIO_AF_NONE,     GPIO_OUT_PP_MAX | GPIO_PU),   // SPI4_CS#

    /* --- Keypad --- */
    GPIO_CFG(GPIOF_NS, GPIO_BIT13,  GPIO_AF_NONE,     GPIO_OUT_PP_MAX | GPIO_PU),   // COL1
    GPIO_CFG(GPIOB_NS, GPIO_BIT2,   GPIO_AF_NONE,     GPIO_OUT_PP_MAX | GPIO_PU),   // COL2
    GPIO_CFG(GPIOC_NS, GPIO_BIT4,   GPIO_AF_NONE,     GPIO_OUT_PP_MAX | GPIO_PU),   // COL3
    GPIO_CFG(GPIOF_NS, GPIO_BIT14,  GPIO_AF_NONE,     GPIO_IN),                     // ROW1
    GPIO_CFG(GPIOF_NS, GPIO_BIT15,  GPIO_AF_NONE,     GPIO_IN),                     // ROW2
    GPIO_CFG(GPIOF_NS, GPIO_BIT12,  GPIO_AF_NONE,     GPIO_IN),                     // ROW3
    GPIO_CFG(GPIOA_NS, GPIO_BIT0,   GPIO_AF_NONE,     GPIO_IN),                     // WAKEUP# (SW9)

    /* --- Gyro&Accel LEDs --- */
    GPIO_CFG(GPIOB_NS, GPIO_BIT6,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PU),  // LD7
    GPIO_CFG(GPIOB_NS, GPIO_BIT7,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PU),  // LD6
    GPIO_CFG(GPIOG_NS, GPIO_BIT12,  GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PU),  // LD8
    GPIO_CFG(GPIOG_NS, GPIO_BIT13,  GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PU),  // LD9

    /* --- Potentiometers --- */
    GPIO_CFG(GPIOC_NS, GPIO_BIT5,   GPIO_AF_NONE,     GPIO_IN_ANALOG),              // ADC1_IN8  (POT1)
    GPIO_CFG(GPIOA_NS, GPIO_BIT4,   GPIO_AF_NONE,     GPIO_IN_ANALOG),              // ADC1_IN18 (POT2)
    GPIO_CFG(GPIOA_NS, GPIO_BIT3,   GPIO_AF_NONE,     GPIO_IN_ANALOG),              // ADC1_IN15 (POT3)

    /* --- BUZZER --- */
    GPIO_CFG(GPIOG_NS, GPIO_BIT11,  GPIO_AF_NONE,     GPIO_OUT_PP_MAX | GPIO_PD),   // BZ1

    /* --- PWM --- */
    GPIO_CFG(GPIOG_NS, GPIO_BIT10,  GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PD),  // PWM_EN (PCA9685BS)
    GPIO_CFG(GPIOC_NS, GPIO_BIT7,   GPIO_AF3,         GPIO_OUT_PP_MAX),             // LCD_PWM (TIM8_CH2)
    GPIO_CFG(GPIOC_NS, GPIO_BIT6,   GPIO_AF3,         GPIO_OUT_PP_MAX),             // PWM16 (CN8)

    /* --- RGB_LED --- */
    GPIO_CFG(GPIOD_NS, GPIO_BIT15,  GPIO_AF2,         GPIO_OUT_PP_MAX | GPIO_PD),   // AB-HL5050RGBIC41SA (TIM4_CH4)

    /* --- LCD1602 --- */
    GPIO_CFG(GPIOG_NS, GPIO_BIT5,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW),            // D4
    GPIO_CFG(GPIOG_NS, GPIO_BIT4,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW),            // D5
    GPIO_CFG(GPIOG_NS, GPIO_BIT3,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW),            // D6
    GPIO_CFG(GPIOG_NS, GPIO_BIT2,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW),            // D7
    GPIO_CFG(GPIOG_NS, GPIO_BIT8,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PU),  // RS
    GPIO_CFG(GPIOG_NS, GPIO_BIT7,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PU),  // R/W
    GPIO_CFG(GPIOG_NS, GPIO_BIT6,   GPIO_AF_NONE,     GPIO_OUT_OD_SLOW | GPIO_PU),  // E

    /* --- Interrupts --- */
    GPIO_CFG_IRQ(GPIOE_NS, GPIO_BIT15, GPIO_AF_NONE, (GPIO_IN | GPIO_IRQ_FALLING_EDGE), 5), // APDS9250_INT
    GPIO_CFG_IRQ(GPIOD_NS, GPIO_BIT10, GPIO_AF_NONE, (GPIO_IN | GPIO_IRQ_FALLING_EDGE), 5), // LIS2MDL_INT
    GPIO_CFG_IRQ(GPIOD_NS, GPIO_BIT9,  GPIO_AF_NONE, (GPIO_IN | GPIO_IRQ_RISING_EDGE), 5),  // LSM6DSO_INT1
    GPIO_CFG_IRQ(GPIOD_NS, GPIO_BIT8,  GPIO_AF_NONE, (GPIO_IN | GPIO_IRQ_FALLING_EDGE), 5), // LSM6DSO_INT2

    /* End of list */
    GPIO_CFG(0, 0, 0, 0)
};

/* -------------------------------------------------------------------- */
const PLAT_HD44780_t g_pHD44780 =
{
    .rs_pin  = {GPIOG_NS, GPIO_BIT8},
    .rw_pin  = {GPIOG_NS, GPIO_BIT7},
    .en_pin  = {GPIOG_NS, GPIO_BIT6},
    .db_pins = {
        {GPIOG_NS, GPIO_BIT5},
        {GPIOG_NS, GPIO_BIT4},
        {GPIOG_NS, GPIO_BIT3},
        {GPIOG_NS, GPIO_BIT2}
    }
};

/* -------------------------------------------------------------------- */
const PLAT_EEPROM_t g_pEEPROM =
{
    .i2c        = I2C1,
    .i2c_addr   = 0x50,
    .capacity   = 2048U  /* 16kBits AT24C16 */
};

/* -------------------------------------------------------------------- */
const PLAT_HTU21D_t g_pHTU21D = {
    .i2c        = I2C4
};

/* -------------------------------------------------------------------- */
const PLAT_APDS9250_t g_pAPDS9250 = {
    .i2c        = I2C4,
    .gain       = APDS9250_GAIN_3,
    .resolution = APDS9250_RES_18BIT
};

/* -------------------------------------------------------------------- */
const PLAT_PCA9685_t g_pPCA9685 = {
    .en_pin      = {GPIOG_NS, GPIO_BIT10},
    .i2c         = I2C4,
    .i2c_addr    = 0x44, /* Check for SB44,45,46,47,48 */
    .pwm_freq_hz = 1000  /* LEDs - 1000Hz, for servos - 50Hz */
};

/* -------------------------------------------------------------------- */
const PLAT_LSM6DSO_t g_pLSM6DSO = {
    .spi         = SPI2,
    .cs_pin      = {GPIOB_NS, GPIO_BIT10},
};

/* -------------------------------------------------------------------- */
const PLAT_LIS2MDL_t g_pLIS2MDL = {
    .spi         = SPI2,
    .cs_pin      = {GPIOB_NS, GPIO_BIT12},
};

/* -------------------------------------------------------------------- */
const PLAT_NORFL_t g_pNORFL = {
    .spi         = SPI4,
    .cs_pin      = {GPIOE_NS, GPIO_BIT4},
    .page_size   = 256,
};

/* -------------------------------------------------------------------- */
/* Define navigation and numerical keymaps */
const PLAT_KEYPAD_t g_pKeyPad = {
    .col_pins = {
        {GPIOF_NS, GPIO_BIT13}, /* COL0 */
        {GPIOB_NS, GPIO_BIT2},  /* COL1 */
        {GPIOC_NS, GPIO_BIT4}   /* COL2 */
     },
     .row_pins = {
        {GPIOF_NS, GPIO_BIT14}, /* ROW0 */
        {GPIOF_NS, GPIO_BIT15}, /* ROW1 */
        {GPIOF_NS, GPIO_BIT12}, /* ROW2 */
        {GPIOA_NS, GPIO_BIT0},  /* ROW3 SW9 */
     }
};
