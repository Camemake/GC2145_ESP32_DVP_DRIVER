#pragma once
#include <stdint.h>

typedef struct {
    uint8_t reg;
    uint8_t val;
} sensor_reg_t;

extern const sensor_reg_t gc2145_init_regs[];
