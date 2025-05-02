/**
 * Example: capture one frame from GC2145 and print its size.
 * Supports ESP32, ESP32-S2, ESP32-S3 (DVP).
 */
#include "gc2145_dvp.h"
#include "esp_log.h"
#include "esp_system.h"

static const char *TAG = "example";

void app_main(void)
{
    gc2145_dvp_config_t cfg = {
        .i2c_port = I2C_NUM_0,
        .pwdn_gpio = GPIO_NUM_32,
        .reset_gpio = GPIO_NUM_33,
    };

    // Common camera pin map for AI-Thinker modules (adjust per board)
#if CONFIG_IDF_TARGET_ESP32
    cfg.cam_cfg = (camera_config_t) {
        .pin_pwdn  = GPIO_NUM_32,
        .pin_reset = GPIO_NUM_33,
        .pin_xclk  = GPIO_NUM_0,
        .pin_sda   = GPIO_NUM_26,
        .pin_scl   = GPIO_NUM_27,
        .pin_d7    = GPIO_NUM_35,
        .pin_d6    = GPIO_NUM_34,
        .pin_d5    = GPIO_NUM_39,
        .pin_d4    = GPIO_NUM_36,
        .pin_d3    = GPIO_NUM_21,
        .pin_d2    = GPIO_NUM_19,
        .pin_d1    = GPIO_NUM_18,
        .pin_d0    = GPIO_NUM_5,
        .pin_vsync = GPIO_NUM_25,
        .pin_href  = GPIO_NUM_23,
        .pin_pclk  = GPIO_NUM_22,
        .xclk_freq_hz = 20000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_SVGA,
        .fb_location = CAMERA_FB_IN_PSRAM,
    };
#elif CONFIG_IDF_TARGET_ESP32S2
    cfg.cam_cfg = (camera_config_t) {
        .pin_pwdn  = GPIO_NUM_32,
        .pin_reset = GPIO_NUM_33,
        .pin_xclk  = GPIO_NUM_42,
        .pin_sda   = GPIO_NUM_41,
        .pin_scl   = GPIO_NUM_40,
        .pin_d7    = GPIO_NUM_15,
        .pin_d6    = GPIO_NUM_16,
        .pin_d5    = GPIO_NUM_17,
        .pin_d4    = GPIO_NUM_18,
        .pin_d3    = GPIO_NUM_12,
        .pin_d2    = GPIO_NUM_11,
        .pin_d1    = GPIO_NUM_10,
        .pin_d0    = GPIO_NUM_9,
        .pin_vsync = GPIO_NUM_8,
        .pin_href  = GPIO_NUM_6,
        .pin_pclk  = GPIO_NUM_7,
        .xclk_freq_hz = 24000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_SVGA,
        .fb_location = CAMERA_FB_IN_PSRAM,
    };
#elif CONFIG_IDF_TARGET_ESP32S3
    cfg.cam_cfg = (camera_config_t) {
        .pin_pwdn  = GPIO_NUM_0,
        .pin_reset = GPIO_NUM_NC,
        .pin_xclk  = GPIO_NUM_15,
        .pin_sda   = GPIO_NUM_4,
        .pin_scl   = GPIO_NUM_5,
        .pin_d7    = GPIO_NUM_14,
        .pin_d6    = GPIO_NUM_16,
        .pin_d5    = GPIO_NUM_18,
        .pin_d4    = GPIO_NUM_12,
        .pin_d3    = GPIO_NUM_10,
        .pin_d2    = GPIO_NUM_8,
        .pin_d1    = GPIO_NUM_9,
        .pin_d0    = GPIO_NUM_11,
        .pin_vsync = GPIO_NUM_6,
        .pin_href  = GPIO_NUM_7,
        .pin_pclk  = GPIO_NUM_13,
        .xclk_freq_hz = 24000000,
        .ledc_timer = LEDC_TIMER_0,
        .ledc_channel = LEDC_CHANNEL_0,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_SVGA,
        .fb_location = CAMERA_FB_IN_PSRAM,
    };
#endif

    if (gc2145_dvp_init(&cfg) == ESP_OK) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (fb) {
            ESP_LOGI(TAG, "Captured %u bytes (%ux%u)", fb->len, fb->width, fb->height);
            esp_camera_fb_return(fb);
        } else {
            ESP_LOGE(TAG, "Capture failed");
        }
    } else {
        ESP_LOGE(TAG, "Driver init failed");
    }
}
