/**
 * Copyright (C) 2019, Sergey Shcherbakov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Example description:
 *
 * Turn-on backlight on pressing SW2 button
 */

// Std headers
#include <stddef.h>

// libopencm3 headers
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

// Drivers
#include "../../libopencm3-drivers/include/hd44780.h"
#include "../../libopencm3-drivers/include/keys.h"

// Description of PINS that used as a bus for HD44780 display
static struct hd44780_bus lcd_bus = {
	.rs = {GPIOE, GPIO7},
	.e = {GPIOE, GPIO11},
	.rnw = {GPIOE, GPIO10},
	.db7 = {GPIOE, GPIO15},
	.db6 = {GPIOE, GPIO14},
	.db5 = {GPIOE, GPIO13},
	.db4 = {GPIOE, GPIO12},
};

// Descriptor of PINS that used as a keys
static struct keys_s keys[] = {
	// SW1
	{
		.port = GPIOC,
		.gpio = GPIO11,
		.pup = false,
		.nc = false,
	},
	// SW2
	{
		.port = GPIOA,
		.gpio = GPIO15,
		.pup = false,
		.nc = false,
	},
	// SW3
	{
		.port = GPIOC,
		.gpio = GPIO9,
		.pup = false,
		.nc = false,
	},
	// SW4
	{
		.port = GPIOC,
		.gpio = GPIO6,
		.pup = false,
		.nc = false,
	},
	// SW5
	{
		.port = GPIOC,
		.gpio = GPIO8,
		.pup = false,
		.nc = false,
	},
};

// Variable that used for counting ms
static volatile uint32_t tick_ms_count;

// Basic sleep function that uses TICK and busy loop
void sleep_ms(uint32_t ms)
{
	uint32_t current_ms = tick_ms_count;
	while ((tick_ms_count - current_ms) < ms);
}

void gpio_setup()
{
	// Enable GPIOE port clock
	rcc_periph_clock_enable(RCC_GPIOE);

	// Preconfigure backlight to off state
	gpio_clear(GPIOE, GPIO9);

	// Configure backlight GPIO
	gpio_mode_setup(GPIOE,
		GPIO_MODE_OUTPUT,
		GPIO_PUPD_NONE,
		GPIO9);
}

static void systick_setup(void)
{
	// 168MHz / 8 == 21000000 counts per second
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	// 21000000/1000 = 21000 overflows per second - every 1ms one interrupt
	// SysTick interrupt every N clock pulses: set reload to N-1
	systick_set_reload(21000 - 1);

	// Enable tick interrupts
	systick_interrupt_enable();

	// Start tick counter
	systick_counter_enable();
}

void sys_tick_handler(void)
{
	tick_ms_count++;
}

int main(void)
{
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
	gpio_setup();
	systick_setup();
	hd44780_init(&lcd_bus, 16, false, 2, false);
	keys_setup(keys, sizeof(keys)/sizeof(keys[0]));

	hd44780_printf("Globallogic\nEducation");
	sleep_ms(3000);

	hd44780_clear();
	hd44780_printf("GL.EDU\nPress key SW2 ->");

	while (1) {
		if (key_pressed(keys, 1)) {
			// Turn-on backlight
			gpio_set(GPIOE, GPIO9);
		} else {
			// Turn-off backlight
			gpio_clear(GPIOE, GPIO9);
		}
	}

	return 0;
}
