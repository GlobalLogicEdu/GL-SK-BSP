/**
 * @file    pwm_pca9685.c
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
#include "platform/gpio.h"
#include "platform/systick.h"
#include "platform/pwm_pca9685.h"

/* PCA9685 Registers */
#define PCA9685_REG_MODE1           0x00
#define PCA9685_REG_MODE2           0x01
#define PCA9685_REG_SUBADR1         0x02
#define PCA9685_REG_PRESCALE        0xFE

/* First Channel Base Registers */
#define PCA9685_REG_LED0_ON_L       0x06
#define PCA9685_REG_ALL_LED_ON_L    0xFA

/* MODE1 Bit definitions */
#define PCA9685_MODE1_RESTART       (1U << 7)
#define PCA9685_MODE1_EXTCLK        (1U << 6)
#define PCA9685_MODE1_AI            (1U << 5) // Auto-Increment enable
#define PCA9685_MODE1_SLEEP         (1U << 4) // Low power mode (needed to set PRE_SCALE)

/* MODE2 Bit definitions */
#define PCA9685_MODE2_OUTDRV        (1U << 2) // 1 = Totem pole output, 0 = Open-drain
#define PCA9685_MODE2_INVRT         (1U << 4) // Output logic state: 1 = inverted, 0 = NOT inverted

/* */
extern const PLAT_PCA9685_t g_pPCA9685;

/* */
static I2C_Status_t PCA9685_WriteReg(uint8_t reg, uint8_t val)
{
    const uint8_t buf[2] = { reg, val };
    return PLAT_I2C_Xfer(g_pPCA9685.i2c, g_pPCA9685.i2c_addr, buf, 2, NULL, 0);
}

/* */
static I2C_Status_t PCA9685_ReadReg(uint8_t reg, uint8_t* val)
{
    return PLAT_I2C_Xfer(g_pPCA9685.i2c, g_pPCA9685.i2c_addr, &reg, 1, val, 1);
}

/* */
bool PLAT_PCA9685_Init(void)
{
    /* Enable Auto-Increment (AI) for sequential registers write */
    if (PCA9685_WriteReg(PCA9685_REG_MODE1, PCA9685_MODE1_AI) != I2C_OK) {
        return false;
    }

    /* Set Mode2 (Totem Pole, Inverted) */
    if (PCA9685_WriteReg(PCA9685_REG_MODE2, PCA9685_MODE2_OUTDRV | PCA9685_MODE2_INVRT) != I2C_OK) {
        return false;
    }

    /* Calculating PRE_SCALE for required frequency without float */
    /* Prescale = (25,000,000 / (4096 * freq)) - 1 */
    /* Rounding to keep precision */
    uint32_t prescale_val = 0;
    uint32_t denominator = 4096U * (uint32_t)g_pPCA9685.pwm_freq_hz;

    if (denominator > 0) {
        prescale_val = ((25000000U + (denominator / 2U)) / denominator) - 1U;
    }

    if (prescale_val < 3) prescale_val = 3;       // Max freq (~1526 Ãö)
    if (prescale_val > 255) prescale_val = 255;   // Min freq (~24 Ãö)

    /* For PRE_SCALE modification, PCA9685 should be in SLEEP mode */
    uint8_t old_mode = 0;
    PCA9685_ReadReg(PCA9685_REG_MODE1, &old_mode);
    uint8_t sleep_mode = (old_mode & ~PCA9685_MODE1_RESTART) | PCA9685_MODE1_SLEEP | PCA9685_MODE1_AI;

    PCA9685_WriteReg(PCA9685_REG_MODE1, sleep_mode);                  /* Sleep */
    PCA9685_WriteReg(PCA9685_REG_PRESCALE, (uint8_t)prescale_val);
    PCA9685_WriteReg(PCA9685_REG_MODE1, old_mode | PCA9685_MODE1_AI); /* WakeUp */
    PLAT_SYSTICK_DelayMs(5);

    /* Restart */
    PCA9685_WriteReg(PCA9685_REG_MODE1, old_mode | PCA9685_MODE1_RESTART | PCA9685_MODE1_AI);
    PLAT_SYSTICK_DelayMs(5);

    /* EN */
    //PLAT_GPIO_SetPinState(&g_pPCA9685.en_pin, 0);
    return true;
}

/* */
bool PLAT_PCA9685_SetPWM(uint8_t channel, uint16_t on_val, uint16_t off_val)
{
    if (channel > 15) channel = 15;

    /* 0x06 + 4 * channel */
    uint8_t base_reg = PCA9685_REG_LED0_ON_L + (4U * channel);

    uint8_t buf[5];
    buf[0] = base_reg;
    buf[1] = (uint8_t)(on_val & 0xFF);
    buf[2] = (uint8_t)((on_val >> 8) & 0x0F);
    buf[3] = (uint8_t)(off_val & 0xFF);
    buf[4] = (uint8_t)((off_val >> 8) & 0x0F);

    I2C_Status_t status = PLAT_I2C_Xfer(g_pPCA9685.i2c, g_pPCA9685.i2c_addr, buf, 5, NULL, 0);
    PLAT_SYSTICK_DelayMs(1);

    return (status == I2C_OK);
}

/* */
bool PLAT_PCA9685_SetAllPWM(uint16_t on_val, uint16_t off_val)
{
    uint8_t buf[5];
    buf[0] = PCA9685_REG_ALL_LED_ON_L;
    buf[1] = (uint8_t)(on_val & 0xFF);
    buf[2] = (uint8_t)((on_val >> 8) & 0x0F);
    buf[3] = (uint8_t)(off_val & 0xFF);
    buf[4] = (uint8_t)((off_val >> 8) & 0x0F);

    I2C_Status_t status = PLAT_I2C_Xfer(g_pPCA9685.i2c, g_pPCA9685.i2c_addr, buf, 5, NULL, 0);
    PLAT_SYSTICK_DelayMs(1);

    return (status == I2C_OK);
}

/* */
bool PLAT_PCA9685_SetDutyCycle(uint8_t channel, uint16_t duty_4096)
{
    if (duty_4096 >= 4096) { /* 100% */
        return PLAT_PCA9685_SetPWM(channel, 4096, 0);
    } else if (duty_4096 == 0) { /* 0% */
        return PLAT_PCA9685_SetPWM(channel, 0, 4096);
    }
    return PLAT_PCA9685_SetPWM(channel, 0, duty_4096);
}
