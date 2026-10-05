/**
 * @file    lsm6dso.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_LSM6DSO_H_
#define PLAT_LSM6DSO_H_

/* Sensitivity factors based on selected scale:
 * Accel +/-2g -> 0.061 mg/LSB = 0.000061 g/LSB
 * Gyro +/-2000dps -> 70 mdps/LSB = 0.070 dps/LSB */
#define LSM6DSO_ACCEL_SENS_2G    0.000061f
#define LSM6DSO_GYRO_SENS_2000   0.070f

/* */
typedef struct {
    SPI_TypeDef*    spi;
    GPIO_Pin_t      cs_pin;
} PLAT_LSM6DSO_t;

/* */
bool PLAT_LSM6DSO_Init(void);
bool PLAT_LSM6DSO_Read();

#endif /* PLAT_LSM6DSO_H_ */
