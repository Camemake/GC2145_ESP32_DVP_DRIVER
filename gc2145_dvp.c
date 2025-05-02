#include "gc2145_dvp.h"
#include "esp_camera.h"
#include "esp_log.h"

static const char *TAG = "gc2145_dvp";

esp_err_t gc2145_dvp_init(const gc2145_dvp_config_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg, ESP_ERR_INVALID_ARG, TAG, "null cfg");

    // 1. Initialize sensor over I²C (SCCB)
    ESP_ERROR_CHECK(gc2145_sensor_boot(cfg->i2c_port,
                                       cfg->cam_cfg.pin_sda,
                                       cfg->cam_cfg.pin_scl,
                                       cfg->pwdn_gpio,
                                       cfg->reset_gpio));

    // 2. Fill esp_camera sensor field to bypass probe (unknown sensor)
    sensor_t *s = esp_camera_sensor_get();
    if (s) {
        s->id.PID = 0x2145;
        // Further custom hooks could be added here
    }

    // 3. Start camera capture
    return esp_camera_init(&cfg->cam_cfg);
}
