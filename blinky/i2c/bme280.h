#ifndef BME280_H
#define BME280_H

#include <stdint.h>

/**
 * Verify the I2C bus is ready, read the sensor's factory calibration
 * data, and put it into normal measurement mode.
 *
 * Returns 0 on success, negative errno on failure.
 */
int bme280_init(void);

/**
 * Read and compensate the current temperature.
 *
 * temp_out is written with the temperature in units of 0.01 degC
 * (e.g. 2534 means 25.34 C).
 *
 * Returns 0 on success, negative errno on failure.
 */
int bme280_read_temperature(int32_t *temp_out);
#ifdef CONFIG_SUM_LOG
	int bme280_log_temperature(void);
#endif


#endif /* BME280_H */