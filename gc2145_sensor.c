#include "gc2145_sensor.h"
#include "gc2145_regs.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define GC2145_ADDR 0x3C
static const char *TAG = "gc2145_sensor";
static i2c_port_t s_port;

static esp_err_t wr_reg(uint8_t reg, uint8_t val)
{
    return i2c_master_write_to_device(s_port, GC2145_ADDR, (uint8_t[]){reg,val}, 2,
                                      1000 / portTICK_PERIOD_MS);
}
static esp_err_t rd_reg(uint8_t reg, uint8_t *val)
{
    return i2c_master_write_read_device(s_port, GC2145_ADDR, &reg, 1, val, 1,
                                        1000 / portTICK_PERIOD_MS);
}
static esp_err_t load_regs(const sensor_reg_t *t)
{
    while(!(t->reg == 0xFF && t->val == 0xFF)) {
        ESP_ERROR_CHECK(wr_reg(t->reg, t->val));
        ++t;
    }
    return ESP_OK;
}

esp_err_t gc2145_sensor_boot(i2c_port_t port, gpio_num_t sda, gpio_num_t scl,
                             gpio_num_t pwdn, gpio_num_t reset)
{
    s_port = port;
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000
    };
    ESP_ERROR_CHECK(i2c_param_config(port, &cfg));
    ESP_ERROR_CHECK(i2c_driver_install(port, cfg.mode, 0, 0, 0));

    if (pwdn != GPIO_NUM_NC) {
        gpio_config_t g = { .pin_bit_mask = 1ULL<<pwdn, .mode = GPIO_MODE_OUTPUT };
        gpio_config(&g);
        gpio_set_level(pwdn, 0);
    }
    if (reset != GPIO_NUM_NC) {
        gpio_config_t g = { .pin_bit_mask = 1ULL<<reset, .mode = GPIO_MODE_OUTPUT };
        gpio_config(&g);
        gpio_set_level(reset, 0);
        vTaskDelay(pdMS_TO_TICKS(10));
        gpio_set_level(reset, 1);
    }

    // Verify ID
    uint8_t idh, idl;
    ESP_ERROR_CHECK(rd_reg(0xF0, &idh));
    ESP_ERROR_CHECK(rd_reg(0xF1, &idl));
    uint16_t id = (idh<<8)|idl;
    if (id != 0x2145) {
        ESP_LOGE(TAG, "Unexpected chip id 0x%04X", id);
        return ESP_ERR_NOT_FOUND;
    }
    ESP_LOGI(TAG, "GC2145 detected (id=0x%04X)", id);

    // Load bootstrap
    return load_regs(gc2145_init_regs);
}

esp_err_t gc2145_sensor_set_mirror(bool mirror)
{
    uint8_t v;
    ESP_ERROR_CHECK(rd_reg(0x17, &v));
    v = (v & ~0x01) | (mirror ? 1 : 0);
    return wr_reg(0x17, v);
}
esp_err_t gc2145_sensor_set_flip(bool flip)
{
    uint8_t v;
    ESP_ERROR_CHECK(rd_reg(0x17, &v));
    v = (v & ~0x02) | (flip ? 2 : 0);
    return wr_reg(0x17, v);
}
esp_err_t gc2145_sensor_set_framesize(uint16_t w, uint16_t h)
{
    (void)w; (void)h;
    // TODO: implement framesize tables
    ESP_LOGW(TAG, "Framesize change not implemented yet");
    return ESP_ERR_NOT_SUPPORTED;
}
esp_err_t gc2145_sensor_set_gain_db(int gain_db)
{
    if (gain_db < 0) gain_db = 0;
    if (gain_db > 24) gain_db = 24;
    uint8_t reg = 0x40 + gain_db * 3;
    return wr_reg(0xB0, reg);
}
