#include <zephyr/kernel.h>
#include "sum_printk.h"

int sum_printk(int a, int b)
{
	int result = a + b;

	printk("printing sum of %d and %d is %d\n", a, b, result);

	return result;
}