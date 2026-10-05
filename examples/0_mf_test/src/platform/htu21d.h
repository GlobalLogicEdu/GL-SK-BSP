/**
 * @file    htu21d.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_HTU21D_H_
#define PLAT_HTU21D_H_

/* */
typedef struct {
    I2C_TypeDef * i2c;
} PLAT_HTU21D_t;

/* */
bool PLAT_HTU21D_Init(void);
bool PLAT_HTU21D_ReadTempInt(int32_t *temp_x100);
bool PLAT_HTU21D_ReadHumInt(int32_t *hum_x100);

/*
int32_t temp_x100 = 0;
int32_t hum_x100 = 0;

if (PLAT_HTU21D_ReadTempInt(&temp_x100))
{
    int32_t temp_deg = temp_x100 / 100;                // Dec part (°C)
    int32_t temp_frac = temp_x100 % 100;               // Frac part
    if (temp_frac < 0) temp_frac = -temp_frac;         // Fix sign for fractional part

    sprintf(str, "Temp: %d.%02d C", temp_deg, temp_frac);
}
*/

#endif /* PLAT_HTU21D_H_ */
