/**
 * @file    tests.c
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
#include <stdlib.h>
#include <stm32h562xx.h>

#include "platform/clock.h"
#include "platform/systick.h"
#include "platform/gpio.h"
#include "platform/adc.h"
#include "platform/i2c.h"
#include "platform/spi.h"
#include "platform/console.h"
#include "platform/pwm_tim.h"
#include "platform/eeprom.h"
#include "platform/hd44780.h"
#include "platform/htu21d.h"
#include "platform/apds9250.h"
#include "platform/pwm_pca9685.h"
#include "platform/led_ws2812.h"
#include "platform/lsm6dso.h"
#include "platform/lis2mdl.h"
#include "platform/flash_nor.h"
#include "platform/gpio_keypad.h"

#include "tests.h"

/* */
static void Test1_Entry(void);
static void Test2_Entry(void);
static void Test3_Entry(void);
static void Test4_Entry(void);

/* */
static const GPIO_KeyId_t s_KeyMap[GPIO_KEYB_ROWS][GPIO_KEYB_COLS] = {
    {KEY_SW3,  KEY_SW4,  KEY_SW5},
    {KEY_SW6,  KEY_SW7,  KEY_SW8},
    {KEY_SW10, KEY_SW11, KEY_SW12},
    {KEY_SW9,  KEY_SW9,  KEY_SW9},
};

static volatile bool s_KeyStatus[KEYS_TOTAL] __attribute__((aligned(4)));

/* */
void PLAT_KEYPAD_OnKeyPress(uint8_t row, uint8_t col)
{
    if (row >= GPIO_KEYB_ROWS || col >= GPIO_KEYB_COLS) {
        return;
    }
    const GPIO_KeyId_t keyId = s_KeyMap[row][col];

    if (keyId < KEYS_TOTAL) {
        s_KeyStatus[keyId] = true;
    }
}

void PLAT_KEYPAD_OnKeyRelease(uint8_t row, uint8_t col)
{
    if (row >= GPIO_KEYB_ROWS || col >= GPIO_KEYB_COLS) {
        return;
    }
    const GPIO_KeyId_t keyId = s_KeyMap[row][col];

    if (keyId < KEYS_TOTAL) {
        s_KeyStatus[keyId] = false;
    }
}

/* */
static void BuzzerToggle(void)
{
    PLAT_GPIO_SetBit(GPIOG_NS, GPIO_BIT11);
    PLAT_SYSTICK_DelayMs(100);
    PLAT_GPIO_ClrBit(GPIOG_NS, GPIO_BIT11);
}

/* ------------------------------------------------------------------------- */
static void Test1_Entry(void)
{
    uint16_t result = 0;

    PLAT_HD44780_Clear();
    PLAT_HD44780_PutString(0, 0, "TEST1:    KEYPAD");
    PLAT_HD44780_PutString(0, 1, "Press a Keys... ");

    /* Wait for SW9 key released */
    while (s_KeyStatus[KEY_SW9]);

    while(1) {
        if (result == 0x1ff) {
            if (s_KeyStatus[KEY_SW9]) {
                BuzzerToggle();
                Test2_Entry();
                break;
            }
            continue;
        }

        if (s_KeyStatus[KEY_SW3]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW3 Pressed     ");
            while (s_KeyStatus[KEY_SW3]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW3 OK          ");
            result |= (1 << 0);
        }

        if (s_KeyStatus[KEY_SW4]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW4 Pressed     ");
            while (s_KeyStatus[KEY_SW4]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW4 OK          ");
            result |= (1 << 1);
        }

        if (s_KeyStatus[KEY_SW5]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW5 Pressed     ");
            while (s_KeyStatus[KEY_SW5]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW5 OK          ");
            result |= (1 << 2);
        }

        if (s_KeyStatus[KEY_SW6]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW6 Pressed     ");
            while (s_KeyStatus[KEY_SW6]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW6 OK          ");
            result |= (1 << 3);
        }

        if (s_KeyStatus[KEY_SW7]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW7 Pressed     ");
            while (s_KeyStatus[KEY_SW7]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW7 OK          ");
            result |= (1 << 4);
        }

        if (s_KeyStatus[KEY_SW8]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW8 Pressed     ");
            while (s_KeyStatus[KEY_SW8]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW8 OK          ");
            result |= (1 << 5);
        }

        if (s_KeyStatus[KEY_SW10]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW10 Pressed     ");
            while (s_KeyStatus[KEY_SW10]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW10 OK          ");
            result |= (1 << 6);
        }

        if (s_KeyStatus[KEY_SW11]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW11 Pressed     ");
            while (s_KeyStatus[KEY_SW11]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW11 OK          ");
            result |= (1 << 7);
        }

        if (s_KeyStatus[KEY_SW12]) {
            BuzzerToggle();
            PLAT_HD44780_PutString(0, 1, "SW12 Pressed     ");
            while (s_KeyStatus[KEY_SW12]); /* Wait for KEY release */
            PLAT_HD44780_PutString(0, 1, "SW12 OK          ");
            result |= (1 << 8);
        }

        if (result == 0x1ff) {
            PLAT_HD44780_PutString(0, 0, "TEST1:    PASSED");
            PLAT_HD44780_PutString(0, 1, "Next test  > SW9");
            PLAT_SYSTICK_DelayMs(1000);
        }
    }
}

/* ------------------------------------------------------------------------- */
static void Test2_Entry(void)
{
    uint16_t result = 0;
    uint8_t bytes[8] = {};
    uint8_t jedec_id[3] = {0, 0, 0};

    PLAT_HD44780_Clear();
    PLAT_HD44780_PutString(0, 0, "TEST2: IC ACCESS");
    PLAT_HD44780_PutString(0, 1, "Press SW9 to RUN");

    /* Wait for SW9 key released */
    while (s_KeyStatus[KEY_SW9]);

    while(1) {
        if (result == 0x7f) {
            if (s_KeyStatus[KEY_SW9]) {
                BuzzerToggle();
                Test3_Entry();
                break;
            }
            continue;
        }

        /* */
        PLAT_HD44780_PutString(0, 0, "TEST2:   LSM6DSO");
        if (PLAT_LSM6DSO_Init()) {
            PLAT_HD44780_PutString(0, 1, "U8: init OK     ");
            PLAT_SYSTICK_DelayMs(2000);
            result |= (1U << 0);
        }
        else {
            PLAT_HD44780_PutString(0, 1, "U8:  test FAILED");
            while(1);
        }

        /* */
        PLAT_HD44780_PutString(0, 0, "TEST2:   LIS2MDL");
        if (PLAT_LIS2MDL_Init()) {
            PLAT_HD44780_PutString(0, 1, "U9: init OK     ");
            PLAT_SYSTICK_DelayMs(2000);
            result |= (1U << 1);
        }
        else {
            PLAT_HD44780_PutString(0, 1, "U9:  test FAILED");
            while(1);
        }

        /* */
        PLAT_HD44780_PutString(0, 0, "TEST2: APDS-9250");
        if (PLAT_APDS9250_Init() == APDS9250_OK) {
            PLAT_HD44780_PutString(0, 1, "U10: init OK    ");
            PLAT_SYSTICK_DelayMs(2000);
            result |= (1U << 2);
        }
        else {
            PLAT_HD44780_PutString(0, 1, "U10: test FAILED");
            while(1);
        }

        /* */
        PLAT_HD44780_PutString(0, 0, "TEST2:     HTU21");
        PLAT_HTU21D_Init();
        int32_t tempCc;
        if (PLAT_HTU21D_ReadTempInt(&tempCc)) {
            PLAT_HD44780_PutString(0, 1, "U11: read OK    ");
            PLAT_SYSTICK_DelayMs(2000);
            result |= (1U << 3);
        }
        else {
            PLAT_HD44780_PutString(0, 1, "U11: read FAILED");
            while(1);
        }

        /* */
        PLAT_HD44780_PutString(0, 0, "TEST2:    W25Q16");
        PLAT_NORFL_ReadJEDECID(jedec_id);

        if (jedec_id[0] == 0xEF && jedec_id[1] == 0x40 && jedec_id[2] == 0x15) {
            PLAT_HD44780_PutString(0, 1, "U12: JEDEC ID OK");
            PLAT_SYSTICK_DelayMs(2000);
            result |= (1U << 4);
        }
        else {
            PLAT_HD44780_PutString(0, 1, "U12: read FAILED");
            while(1);
        }

        /* */
        PLAT_HD44780_PutString(0, 0, "TEST2:  AT24C16C");
        if (PLAT_EEPROM_Read(0x100, bytes, sizeof(bytes))) {
            PLAT_HD44780_PutString(0, 1, "U13: read OK    ");
            PLAT_SYSTICK_DelayMs(2000);
            result |= (1U << 5);
        }
        else {
            PLAT_HD44780_PutString(0, 1, "U13: read FAILED");
            while(1);
        }

        /* */
        PLAT_HD44780_PutString(0, 0, "TEST2:   PCA9685");
        if (PLAT_PCA9685_Init()) {
            PLAT_HD44780_PutString(0, 1, "U14: init OK    ");
            /* */
            PLAT_PCA9685_SetAllPWM(0, 0);
            PLAT_PCA9685_SetPWM(0, 0, 4095);
            PLAT_PCA9685_SetPWM(4, 0, 4095);
            PLAT_PCA9685_SetPWM(8, 0, 4095);
            PLAT_PCA9685_SetPWM(12, 0, 4095);
            PLAT_HD44780_PutString(0, 1, "U14: RED        ");
            PLAT_SYSTICK_DelayMs(1000);
            /* */
            PLAT_PCA9685_SetAllPWM(0, 0);
            PLAT_PCA9685_SetPWM(1, 0, 4095);
            PLAT_PCA9685_SetPWM(5, 0, 4095);
            PLAT_PCA9685_SetPWM(9, 0, 4095);
            PLAT_PCA9685_SetPWM(13, 0, 4095);
            PLAT_HD44780_PutString(0, 1, "U14: GREEN      ");
            PLAT_SYSTICK_DelayMs(1000);
            /* */
            PLAT_PCA9685_SetAllPWM(0, 0);
            PLAT_PCA9685_SetPWM(2, 0, 4095);
            PLAT_PCA9685_SetPWM(6, 0, 4095);
            PLAT_PCA9685_SetPWM(10, 0, 4095);
            PLAT_PCA9685_SetPWM(14, 0, 4095);
            PLAT_HD44780_PutString(0, 1, "U14: YELLOW     ");
            PLAT_SYSTICK_DelayMs(1000);
            /* */
            PLAT_PCA9685_SetAllPWM(0, 0);
            PLAT_PCA9685_SetPWM(3, 0, 4095);
            PLAT_PCA9685_SetPWM(7, 0, 4095);
            PLAT_PCA9685_SetPWM(11, 0, 4095);
            PLAT_PCA9685_SetPWM(15, 0, 4095);
            PLAT_HD44780_PutString(0, 1, "U14: BLUE       ");
            PLAT_SYSTICK_DelayMs(1000);
            /* */
            PLAT_HD44780_PutString(0, 1, "U14: ALL OFF    ");
            PLAT_PCA9685_SetAllPWM(0, 0);
            result |= (1U << 6);
        }
        else {
            PLAT_HD44780_PutString(0, 1, "U14: init FAILED");
            while(1);
        }

        if (result == 0x7f) {
            PLAT_SYSTICK_DelayMs(1000);
            PLAT_HD44780_PutString(0, 0, "TEST2:    PASSED");
            PLAT_HD44780_PutString(0, 1, "Next test  > SW9");
        }
    }
}

/* ------------------------------------------------------------------------- */
static void Test3_Entry(void)
{
    bool done = false;

    PLAT_HD44780_Clear();
    PLAT_HD44780_PutString(0, 0, "TEST3:   RGB-LED");
    PLAT_HD44780_PutString(0, 1, "Press SW9 to RUN");

    /* Wait for SW9 key released */
    while (s_KeyStatus[KEY_SW9]);

    while(1) {
        if (done) {
            if (s_KeyStatus[KEY_SW9]) {
                BuzzerToggle();
                Test4_Entry();
                break;
            }
            continue;
        }

        PLAT_HD44780_PutString(0, 1, "LD26: RED       ");
        PLAT_WS2812_SetAll(100, 0, 0);
        PLAT_WS2812_Update();
        PLAT_SYSTICK_DelayMs(2000);

        PLAT_HD44780_PutString(0, 1, "LD26: GREEN     ");
        PLAT_WS2812_SetAll(0, 100, 0);
        PLAT_WS2812_Update();
        PLAT_SYSTICK_DelayMs(2000);

        PLAT_HD44780_PutString(0, 1, "LD26: BLUE      ");
        PLAT_WS2812_SetAll(0, 0, 100);
        PLAT_WS2812_Update();
        PLAT_SYSTICK_DelayMs(2000);

        PLAT_WS2812_SetAll(0, 0, 0);
        PLAT_WS2812_Update();

        PLAT_HD44780_PutString(0, 0, "TEST3:      DONE");
        PLAT_HD44780_PutString(0, 1, "Next test  > SW9");
        done = true;
    }
}

/* ------------------------------------------------------------------------- */
static void Test4_Entry(void)
{
    PLAT_HD44780_Clear();
    PLAT_HD44780_PutString(0, 0, "TEST4: POT 1,2,3");
    PLAT_HD44780_PutString(0, 1, "Press SW9 to RUN");

    /* Wait for SW9 key released */
    while (s_KeyStatus[KEY_SW9]);

    PLAT_HD44780_PutString(0, 1, "Rotate, exit SW9");

    while(1) {
        if (s_KeyStatus[KEY_SW9]) {
            BuzzerToggle();
            PLAT_WS2812_SetAll(0, 0, 0);
            PLAT_WS2812_Update();
            break;
        }

        PLAT_WS2812_SetAll(PLAT_ADC_GetRawPot1() >> 4,
                           PLAT_ADC_GetRawPot2() >> 4,
                           PLAT_ADC_GetRawPot3() >> 4);

        PLAT_WS2812_Update();
        PLAT_SYSTICK_DelayMs(30);
    }
}

/* */
void TESTS_Entry(void)
{
    PLAT_HD44780_Clear();
    PLAT_HD44780_PutString(0, 0, "GL-SK-v2-STM32H5");
    PLAT_HD44780_PutString(0, 1, "Press SW9 to RUN");

    /* Enable GYRO & ACCEL LEDs */
    PLAT_GPIO_ClrBit(GPIOB_NS, GPIO_BIT6); // LD7
    PLAT_GPIO_ClrBit(GPIOB_NS, GPIO_BIT7); // LD6
    PLAT_GPIO_ClrBit(GPIOG_NS, GPIO_BIT12); // LD8
    PLAT_GPIO_ClrBit(GPIOG_NS, GPIO_BIT13); // LD9

    while(1) {
        if (s_KeyStatus[KEY_SW9]) {

            /* Disable GYRO & ACCEL LEDs */
            PLAT_GPIO_SetBit(GPIOB_NS, GPIO_BIT6); // LD7
            PLAT_GPIO_SetBit(GPIOB_NS, GPIO_BIT7); // LD6
            PLAT_GPIO_SetBit(GPIOG_NS, GPIO_BIT12); // LD8
            PLAT_GPIO_SetBit(GPIOG_NS, GPIO_BIT13); // LD9

            BuzzerToggle();
            Test1_Entry();

            /* */
            PLAT_HD44780_PutString(0, 0, "GL-SK-v2-STM32H5");
            PLAT_HD44780_PutString(0, 1, "**TESTING DONE**");

            while(1) {
                __asm volatile ("wfi");
            }
        }
    }
}
