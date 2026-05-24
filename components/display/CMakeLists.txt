idf_component_register(SRCS "display.c" "cat.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "common" "esp_driver_i2c" "lvgl" "esp_lcd" "esp_lvgl_port")

