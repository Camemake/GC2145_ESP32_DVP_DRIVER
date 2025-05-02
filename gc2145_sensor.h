/**
 * @file gc2145_sensor.h
 * @brief GC2145 sensor register driver (SCCB / I2C) for ESP32 series.
 *
 * Provides bootstrap and runtime controls (flip, mirror, gain, framesize).
 */
#pragma once
#include <esp_err.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t gc2145_sensor_boot(i2c_port_t port, gpio_num_t sda, gpio_num_t scl,
                             gpio_num_t pwdn, gpio_num_t reset);

esp_err_t gc2145_sensor_set_framesize(uint16_t width, uint16_t height);
esp_err_t gc2145_sensor_set_mirror(bool mirror);
esp_err_t gc2145_sensor_set_flip(bool flip);
esp_err_t gc2145_sensor_set_gain_db(int gain_db);

#ifdef __cplusplus
}
#endif
