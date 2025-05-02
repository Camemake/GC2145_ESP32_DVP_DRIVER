idf_component_register(SRCS "src/gc2145_regs.c"
                           "src/gc2145_sensor.c"
                           "src/gc2145_dvp.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_camera)
