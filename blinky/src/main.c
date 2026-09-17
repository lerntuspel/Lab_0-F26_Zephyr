/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#ifdef CONFIG_SUM_PRINT
	#include "sum_printk.h"
#elif defined(CONFIG_SUM_LOG)
	#include "sum_log.h"
#endif

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   2000
#define POLL_MS 50

/* The devicetree node identifier for the "led0" alias. */
#define LED1_NODE DT_ALIAS(led5180)
#define BUTTON1_NODE DT_ALIAS(button1)
/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON1_NODE, gpios);

int main(void)
{
	int ret;
	int last_state = 0;

	if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&button)) {
		return 0;
	}
	
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return 0;
	}

	while (1) {
		int state = gpio_pin_get_dt(&button);

		if (state < 0) {
			k_msleep(POLL_MS);
			continue;
		}

		if (state && !last_state) {
			gpio_pin_toggle_dt(&led);
			#ifdef CONFIG_SUM_PRINT
				int total = sum_printk(3, 4);
			#elif defined(CONFIG_SUM_LOG)
				int total = sum_log(3, 4);
			#endif
		}

		last_state = state;
		k_msleep(POLL_MS);
	}
	return 0;
}
