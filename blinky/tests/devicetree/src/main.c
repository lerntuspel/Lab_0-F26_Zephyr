#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/ztest.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/devicetree.h>

/*
 * Compile-time checks: if any of these fail, the BUILD ITSELF fails
 * with a clear message, before any test even runs.
 */
BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(led5180)), "led5180 alias is missing from the devicetree");
BUILD_ASSERT(DT_NODE_EXISTS(DT_ALIAS(button1)), "button1 alias is missing from the devicetree");
BUILD_ASSERT(DT_NODE_EXISTS(DT_NODELABEL(bme280)), "bme280 node is missing from the devicetree");



ZTEST(devicetree_sanity, test_led_alias_okay)
{
	zassert_true(DT_NODE_HAS_STATUS(DT_ALIAS(led5180), okay),
		     "led5180 alias node is not status \"okay\"");
}



ZTEST(devicetree_sanity, test_button_alias_okay)
{
	zassert_true(DT_NODE_HAS_STATUS(DT_ALIAS(button1), okay),
		     "button1 alias node is not status \"okay\"");
}



ZTEST(devicetree_sanity, test_i2c1_bus_okay)
{
	zassert_true(DT_NODE_HAS_STATUS(DT_NODELABEL(i2c1), okay),
		     "i2c1 bus node is not status \"okay\"");
}



ZTEST(devicetree_sanity, test_bme280_node_okay)
{
	zassert_true(DT_NODE_HAS_STATUS(DT_NODELABEL(bme280), okay),
		     "bme280 node is not status \"okay\"");
}



ZTEST(devicetree_sanity, test_bme280_address)
{
	uint32_t addr = DT_REG_ADDR(DT_NODELABEL(bme280));

	zassert_equal(addr, 0x77, "Expected BME280 I2C address 0x77, got 0x%02x", addr);
}



ZTEST(devicetree_sanity, test_led_gpio_ready)
{
	static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led5180), gpios);

	zassert_true(gpio_is_ready_dt(&led), "LED GPIO controller is not ready");
}



ZTEST(devicetree_sanity, test_button_gpio_ready)
{
	static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(DT_ALIAS(button1), gpios);

	zassert_true(gpio_is_ready_dt(&button), "Button GPIO controller is not ready");
}



ZTEST(devicetree_sanity, test_i2c_bus_ready)
{
	static const struct i2c_dt_spec bme280 = I2C_DT_SPEC_GET(DT_NODELABEL(bme280));

	zassert_true(i2c_is_ready_dt(&bme280), "I2C bus for BME280 is not ready");
}



ZTEST_SUITE(devicetree_sanity, NULL, NULL, NULL, NULL, NULL);