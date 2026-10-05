/**
 * @file    pwm_pca9685.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_PWM_PCA9685_H_
#define PLAT_PWM_PCA9685_H_

/* */
typedef struct {
    GPIO_Pin_t      en_pin;
    I2C_TypeDef*    i2c;
    uint8_t         i2c_addr;
    uint16_t        pwm_freq_hz;
} PLAT_PCA9685_t;

/* */
bool PLAT_PCA9685_Init(void);

/* Sets arbitrary PWM turn-on and turn-off points
 * [channel]  Channel number (0..15)
 * [on_val]   Signal rise point (0..4095)
 * [off_val]  Signal fall point (0..4095)
*/
bool PLAT_PCA9685_SetPWM(uint8_t channel, uint16_t on_val, uint16_t off_val);

/* Simple duty cycle adjustment (0 .. 4096)
 * [duty_4096] Duty cycle value: 0 = 0%, 2048 = 50%, 4096 = 100%
*/
bool PLAT_PCA9685_SetDutyCycle(uint8_t channel, uint16_t duty_4096);

/* Setting the same signal for ALL 16 channels with a single command */
bool PLAT_PCA9685_SetAllPWM(uint16_t on_val, uint16_t off_val);

#endif /* PLAT_PWM_PCA9685_H_ */
