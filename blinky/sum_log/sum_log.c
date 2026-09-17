#include <zephyr/logging/log.h>
#include "sum_log.h"

LOG_MODULE_REGISTER(sum_log, LOG_LEVEL_INF);

int sum_log(int a, int b)
{
	int result = a + b;

	LOG_INF("Logging sum of %d and %d is %d", a, b, result);
	LOG_HEXDUMP_INF(&result, sizeof(result), "Sum result bytes");


	return result;
}