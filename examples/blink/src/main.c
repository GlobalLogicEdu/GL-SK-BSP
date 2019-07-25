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
 * Usually only LEDs are used in blink examples. But in this one we'll
 * use LCD backlight instead of LED.
 *
 * LCD backlight connected to PE9. Interval of toggling light will be
 * managed by TICK timer.
 */

#include <stddef.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

// Variable that used for counting ms
static volatile uint32_t tick_ms_count;

// Basic sleep function that uses TICK and busy loop
void sleep_ms(uint32_t ms)
{
	uint32_t current_ms = tick_ms_count;
	while ((tick_ms_count - current_ms) < ms) {
		continue;
	}
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
		GPIO_PUPD_PULLUP,
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

	while (1) {
		// Toggle backlight
		gpio_toggle(GPIOE, GPIO9);

		sleep_ms(200);
	}

	return 0;
}
