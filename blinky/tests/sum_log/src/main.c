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
	#include <zephyr/ztest.h>
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

#include <zephyr/ztest.h>
#include "sum_log.h"

ZTEST(sum_log_test_suite, test_sum_log_basic)
{
	int result = sum_log(2, 3);

	zassert_equal(result, 5, "Expected 2 + 3 to equal 5, got %d", result);
}




ZTEST(sum_log_test_suite, test_sum_log_negative)
{
	int result = sum_log(-4, -6);

	zassert_equal(result, -10, "Expected -4 + -6 to equal -10, got %d", result);
}




ZTEST(sum_log_test_suite, test_sum_log_zero)
{
	int result = sum_log(0, 0);

	zassert_equal(result, 0, "Expected 0 + 0 to equal 0, got %d", result);
}




ZTEST_SUITE(sum_log_test_suite, NULL, NULL, NULL, NULL, NULL);