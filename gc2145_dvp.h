/**
 * @file gc2145_dvp.h
 * @brief High level DVP camera driver for GC2145 on ESP32/S2/S3.
 *
 * Wraps esp32-camera capture with GC2145 sensor bootstrap.
 */
#pragma once
#include <esp_err.h>
#include "esp_camera.h"
#include "gc2145_sensor.h"

typedef struct {
    camera_config_t cam_cfg;   ///< esp32-camera configuration
    i2c_port_t      i2c_port;
    gpio_num_t      pwdn_gpio;
    gpio_num_t      reset_gpio;
} gc2145_dvp_config_t;

/**
 * @brief Initialize DVP capture and sensor.
 *
 * @param cfg configuration struct
 * @return esp_err_t
 */
esp_err_t gc2145_dvp_init(const gc2145_dvp_config_t *cfg);
