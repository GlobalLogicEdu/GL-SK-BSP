/**
 * @file    gpio.c
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

/* */
static void GPIO_ConfigEXTI(uint32_t io_base, uint16_t pin_num, uint32_t config, uint32_t priority)
{
    const IRQn_Type irq = (IRQn_Type)(EXTI0_IRQn + pin_num);
    const uint32_t exti_idx = (pin_num >> 2);
    const uint32_t bit_shift = (pin_num % 4) * 8;
    const uint32_t pin_bit_mask = (1 << pin_num);

    if (pin_num > GPIO_BIT15) {
        return;
    }

    uint8_t port_idx = 0;
    switch(io_base) {
        case GPIOA_BASE_NS: port_idx = 0; break;
        case GPIOB_BASE_NS: port_idx = 1; break;
        case GPIOC_BASE_NS: port_idx = 2; break;
        case GPIOD_BASE_NS: port_idx = 3; break;
        case GPIOE_BASE_NS: port_idx = 4; break;
        case GPIOF_BASE_NS: port_idx = 5; break;
        case GPIOG_BASE_NS: port_idx = 6; break;
        case GPIOH_BASE_NS: port_idx = 7; break;
        case GPIOI_BASE_NS: port_idx = 8; break;
    }

    EXTI_NS->EXTICR[exti_idx] &= ~(0xff << bit_shift);
    EXTI_NS->EXTICR[exti_idx] |=  (port_idx << bit_shift);

    /* Config Edge */
    switch (config & GPIO_IRQ_CFG_MASK) {
        case GPIO_IRQ_FALLING_EDGE: {
            EXTI_NS->RTSR1 &= ~pin_bit_mask; // Clear the Rising Trigger
            EXTI_NS->FTSR1 |= pin_bit_mask;  // Set the Falling Trigger
            break;
        }
        case GPIO_IRQ_RISING_EDGE: {
            EXTI_NS->RTSR1 |= pin_bit_mask; // Set the Rising Trigger
            EXTI_NS->FTSR1 &= ~pin_bit_mask; // Clear the Falling Trigger
            break;
        }
        case GPIO_IRQ_BOTH_EDGES: {
            EXTI_NS->RTSR1 |= pin_bit_mask; // Set the Rising Trigger
            EXTI_NS->FTSR1 |= pin_bit_mask;  // Set the Falling Trigger
        }
    }

    /* Unmask the Interrupt */
    EXTI_NS->IMR1 |= pin_bit_mask;

    /* NVIC */
    NVIC_SetPriority(irq, priority);
    NVIC_EnableIRQ(irq);
}

/* */
void PLAT_GPIO_Init(void)
{
    /* */
    extern const PLAT_GpioCfg_t g_pGpioList[];
    const PLAT_GpioCfg_t* pin = (PLAT_GpioCfg_t*)&g_pGpioList[0];

    /* */
    while (pin->io_base != 0) {
        GPIO_TypeDef* gpio = (GPIO_TypeDef*)pin->io_base;

        /* */
        const uint32_t config = pin->cfg_bits;
        const uint16_t pin_num = ((config & GPIO_BIT_MASK) >> GPIO_BIT_Shift);
        const uint16_t pin_af = ((config & GPIO_AF_MASK) >> GPIO_AF_Shift);
        const uint16_t pin_shift = (pin_num * 2);
        const uint16_t pin_bit_mask = (1 << pin_num);

        /* Turn off Pull Up/Down functionality by default */
        gpio->PUPDR &= ~(0x3 << pin_shift);

        /* Alternate functionality enable if configured */
        if ((config & GPIO_AF_MASK) != GPIO_AF_NONE) {
            gpio->MODER &= ~(0x3 << (pin_num * 2));
            gpio->MODER |=  (0x2 << (pin_num * 2));
            gpio->AFR[pin_num >> 3] |= (pin_af << ((pin_num & 0x07) * 4));
        }
        else {
            if((config & GPIO_IN_MASK) == GPIO_IN_ANALOG) {
            /* Analog mode */
                gpio->MODER |= (0x3 << pin_shift);
            }
            else if((config & GPIO_IN_MASK) == GPIO_IN) {
                /* GPIO input */
                gpio->MODER &= ~(0x3 << pin_shift);

                if (config & GPIO_IRQ_CFG_MASK) {
                    GPIO_ConfigEXTI(pin->io_base, pin_num, (config & GPIO_IRQ_CFG_MASK), pin->irq_priority);
                }
            } else {
                /* GPIO Output */
                gpio->MODER &= ~(0x3 << pin_shift);
                gpio->MODER |= (0x1 << pin_shift);

                /* Latch requested out level 1 or 0, if specified */
                if((config & GPIO_PULL_MASK) == GPIO_PU) {
                    PLAT_GPIO_SetBit(gpio, pin_num);
                }
                else if((config & GPIO_PULL_MASK) == GPIO_PD) {
                    PLAT_GPIO_ClrBit(gpio, pin_num);
                }
            }
        }

        if (config & GPIO_IN_FLAG) {
            if((config & GPIO_IN_MASK) == GPIO_IN) {
                /* Enable weak pull UP or DOWN, if specified */
                if((config & GPIO_PULL_MASK) == GPIO_PU) {
                    gpio->PUPDR |= (0x1 << pin_shift);
                }
                else if((config & GPIO_PULL_MASK) == GPIO_PD) {
                    gpio->PUPDR |= (0x2 << pin_shift);
                }
            }
        }
        else { /* Setup Output driving/pull mode */
            const uint32_t speed_mode = (config & GPIO_OUT_FREQ_MASK) >> GPIO_OUT_FREQ_Shift;
            gpio->OSPEEDR &= ~(0x3 << pin_shift);
            gpio->OSPEEDR |=  (speed_mode << pin_shift);

            /* Output push-pull (reset state) */
            gpio->OTYPER &= ~pin_bit_mask;

            if ((config & GPIO_OUT_MASK) == GPIO_OUT_OD) {
                gpio->OTYPER |= pin_bit_mask;
            }
        }

        /* Lock the port */
        gpio->LCKR |= pin_bit_mask;
        /* Next pin config from list */
        pin++;
    }
}

/* */
int PLAT_GPIO_GetState(GPIO_TypeDef* gpio, const uint32_t bit)
{
    return !!(gpio->IDR & (1U << bit));
}

/* */
int PLAT_GPIO_GetPinState(const GPIO_Pin_t* pin)
{
    return !!(pin->io_base->IDR & (1U << pin->bit));
}

/* */
void PLAT_GPIO_SetPinState(const GPIO_Pin_t* pin, const int state)
{
    if (state) {
        pin->io_base->BSRR = (1U << pin->bit) & 0x0000ffff;
    } else {
        pin->io_base->BSRR = ((1U << pin->bit) << 16) & 0xffff0000;
    }
}

/* */
int PLAT_GPIO_GetBit(GPIO_TypeDef* gpio, const uint32_t bit)
{
    return !!(gpio->IDR & (1U << bit));
}

void PLAT_GPIO_SetBit(GPIO_TypeDef* gpio, const uint32_t bit)
{
    gpio->BSRR = (1U << bit) & 0x0000ffff;
}

void PLAT_GPIO_ClrBit(GPIO_TypeDef* gpio, const uint32_t bit)
{
    gpio->BSRR = ((1U << bit) << 16) & 0xffff0000;
}
