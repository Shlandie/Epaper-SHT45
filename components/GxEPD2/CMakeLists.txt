idf_component_register(
    SRCS "GxEPD2_290_BS.cpp" "GxEPD2_EPD.cpp"        # adjust to actual source files
    INCLUDE_DIRS "."
    REQUIRES espressif__arduino-esp32 adafruit_gfx adafruit_busio
)