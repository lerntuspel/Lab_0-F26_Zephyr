#include <zephyr/drivers/i2c.h>
#include "bme280.h"
#ifdef CONFIG_SUM_LOG
	#include <zephyr/logging/log.h>
    LOG_MODULE_REGISTER(bme280, LOG_LEVEL_INF);
#endif

#define BME280_NODE DT_NODELABEL(bme280)

static const struct i2c_dt_spec bme280 = I2C_DT_SPEC_GET(BME280_NODE);

#define BME280_REG_CALIB00   0x88
#define BME280_REG_CTRL_HUM  0xF2
#define BME280_REG_CTRL_MEAS 0xF4
#define BME280_REG_TEMP_MSB  0xFA

static uint16_t dig_T1;
static int16_t  dig_T2;
static int16_t  dig_T3;
static int32_t  t_fine;

// I asked AI how I could calibrate the sensor since last time I used this we didn't and the temps recorded were close but not preceise to what was real
static int bme280_read_calibration(void) {
	uint8_t buffer[6];
	int ret = i2c_burst_read_dt(&bme280, BME280_REG_CALIB00, buffer, sizeof(buffer));
	if (ret < 0) {
		return ret;
	}

	dig_T1 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	dig_T2 = (int16_t)(buffer[3] << 8 | buffer[2]);
	dig_T3 = (int16_t)(buffer[5] << 8 | buffer[4]);

	return 0;
}

static int32_t bme280_compensate_temperature(int32_t adc_T)
// pages 24 and 25 of datasheet tell how to implement the compensation formulas
{
	int32_t var1, var2;

	var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * (int32_t)dig_T2) >> 11;
	var2 = (((((adc_T >> 4) - (int32_t)dig_T1) * ((adc_T >> 4) - (int32_t)dig_T1)) >> 12) *
		(int32_t)dig_T3) >> 14;

	t_fine = var1 + var2;

	return (t_fine * 5 + 128) >> 8;
}

static int bme280_configure(void) // turn sensor on
{
	uint8_t ctrl_hum[2]  = { BME280_REG_CTRL_HUM, 0x01 };
	uint8_t ctrl_meas[2] = { BME280_REG_CTRL_MEAS, 0x27 }; // non-sleep mode
	int ret;

	ret = i2c_write_dt(&bme280, ctrl_hum, sizeof(ctrl_hum));
	if (ret < 0) {
		return ret;
	}

	return i2c_write_dt(&bme280, ctrl_meas, sizeof(ctrl_meas));
}

int bme280_init(void)
{
	if (!i2c_is_ready_dt(&bme280)) {
		return -ENODEV;
	}

	int ret = bme280_read_calibration();

	if (ret < 0) {
		return ret;
	}

	return bme280_configure();
}

int bme280_read_temperature(int32_t *temp_out)
{
	uint8_t buffer[3];
	int ret = i2c_burst_read_dt(&bme280, BME280_REG_TEMP_MSB, buffer, sizeof(buffer));
	if (ret < 0) {
		return ret;
	}

	int32_t adc_T = ((int32_t)buffer[0] << 12) | ((int32_t)buffer[1] << 4) | ((int32_t)buffer[2] >> 4);

	*temp_out = bme280_compensate_temperature(adc_T);
	return 0;
}

#ifdef CONFIG_SUM_LOG
    int bme280_log_temperature(void)
    {
        int32_t temp;
        int ret = bme280_read_temperature(&temp);

        if (ret < 0) {
            LOG_ERR("Failed to read BME280 temperature (err %d)", ret);
            return ret;
        }

        LOG_INF("Temperature: %d.%02d C", temp / 100, temp % 100);
        return 0;
    }
#endif
