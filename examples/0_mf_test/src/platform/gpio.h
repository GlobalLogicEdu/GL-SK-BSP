/**
 * @file    gpio.h
 * @brief   Board Bring-Up & Educational sample code for GL-SKv2-STM32H562
 *
 * Copyright (c) 2026 Oleksii Gulchenko. All rights reserved.
 * Provided to GlobalLogic Inc. educational release.
 *
 * Author: Oleksii Gulchenko (https://github.com/oleksiig)
 * Released under the MIT License (see LICENSE file in repository root).
 */

#ifndef PLAT_GPIO_H_
#define PLAT_GPIO_H_

/* GPIO pins/bits */
#define GPIO_BIT_Shift          28UL
#define GPIO_BIT_MASK           0xf0000000
#define GPIO_BIT0               0
#define GPIO_BIT1               1
#define GPIO_BIT2               2
#define GPIO_BIT3               3
#define GPIO_BIT4               4
#define GPIO_BIT5               5
#define GPIO_BIT6               6
#define GPIO_BIT7               7
#define GPIO_BIT8               8
#define GPIO_BIT9               9
#define GPIO_BIT10              10
#define GPIO_BIT11              11
#define GPIO_BIT12              12
#define GPIO_BIT13              13
#define GPIO_BIT14              14
#define GPIO_BIT15              15

/* Alternate function numbers */
#define GPIO_AF_Shift           20UL
#define GPIO_AF_MASK            0x08f00000
#define GPIO_AF_NONE            0x08f00000
#define GPIO_AF0                0x00000000
#define GPIO_AF1                0x00100000
#define GPIO_AF2                0x00200000
#define GPIO_AF3                0x00300000
#define GPIO_AF4                0x00400000
#define GPIO_AF5                0x00500000
#define GPIO_AF6                0x00600000
#define GPIO_AF7                0x00700000
#define GPIO_AF8                0x00800000
#define GPIO_AF9                0x00900000
#define GPIO_AF10               0x00A00000
#define GPIO_AF11               0x00B00000
#define GPIO_AF12               0x00C00000
#define GPIO_AF13               0x00D00000
#define GPIO_AF14               0x00E00000
#define GPIO_AF15               0x00F00000

/** IRQ types for GPIO pins */
#define GPIO_IRQ_CFG_Shift      16UL
#define GPIO_IRQ_CFG_MASK       0x000f0000
#define GPIO_IRQ_FALLING_EDGE   0x00010000
#define GPIO_IRQ_RISING_EDGE    0x00020000
#define GPIO_IRQ_BOTH_EDGES     0x00030000

/* Configuration bits           0x0000ff00
   are reserved */

/* GPIO configuration bits */
#define GPIO_CFG_Shift          0UL
#define GPIO_CFG_MASK           0x000000ff
/* */
#define GPIO_PULL_MASK          0x00000300
#define GPIO_PU                 0x00000100  /* Pull UP */
#define GPIO_PD                 0x00000200  /* Pull Down */
/* GPIO input */
#define GPIO_IN_FLAG            0x00000080
#define GPIO_IN_MASK            0x00000081
#define GPIO_IN_ANALOG          0x00000080  /* Analog input mode */
#define GPIO_IN                 0x00000081  /* GPIO input mode */
/* GPIO output */
#define GPIO_OUT_MASK           0x0000000f
#define GPIO_OUT_PP             0x00000001
#define GPIO_OUT_OD             0x00000002
/* */
#define GPIO_OUT_FREQ_Shift     4UL
#define GPIO_OUT_FREQ_MASK      0x00000030
/* Combined definitions */
#define GPIO_OUT_PP_SLOW        0x00000001  /* Push pull, low speed */
#define GPIO_OUT_OD_SLOW        0x00000002  /* Open drain, low speed */
#define GPIO_OUT_PP_MED         0x00000011  /* Push pull, medium speed */
#define GPIO_OUT_OD_MED         0x00000012  /* Open drain, medium speed */
#define GPIO_OUT_PP_FAST        0x00000021  /* Push pull, fast speed */
#define GPIO_OUT_OD_FAST        0x00000022  /* Open drain, fast speed */
#define GPIO_OUT_PP_MAX         0x00000031  /* Push pull, fastest speed */
#define GPIO_OUT_OD_MAX         0x00000032  /* Open drain, fastest speed */

/* */
#define GPIO_CFG(gpio_base, bit, af, flags) \
    { ((uint32_t)gpio_base), (((bit) << GPIO_BIT_Shift) | (af) | (flags)), 0UL }

/* */
#define GPIO_CFG_IRQ(gpio_base, bit, af, flags, irq_priority) \
    { ((uint32_t)gpio_base), (((bit) << GPIO_BIT_Shift) | (af) | (flags)), (irq_priority) }

/* GPIO pin configuration descriptor */
typedef struct {
    uint32_t    io_base;
    uint32_t    cfg_bits;
    uint32_t    irq_priority;
} PLAT_GpioCfg_t;

/** GPIO pin desriptor */
typedef struct {
    GPIO_TypeDef*     io_base;
    volatile uint32_t bit;
} GPIO_Pin_t;

/* */
void PLAT_GPIO_Init(void);

/* Generic GPIO bit operations */
int PLAT_GPIO_GetState(GPIO_TypeDef* gpio, const uint32_t bit);

void PLAT_GPIO_SetPinState(const GPIO_Pin_t* pin, const int state);
int PLAT_GPIO_GetPinState(const GPIO_Pin_t* pin);

void PLAT_GPIO_SetBit(GPIO_TypeDef* gpio, const uint32_t bit);
void PLAT_GPIO_ClrBit(GPIO_TypeDef* gpio, const uint32_t bit);

#endif /* PLAT_GPIO_H_ */
