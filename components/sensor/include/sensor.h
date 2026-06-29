#pragma once

#include "freertos/FreeRTOS.h"

#define SENSOR_PIN_SDA							6
#define SENSOR_PIN_SCL							7

extern float sensor_humidity, sensor_temperature;







BaseType_t sensor_create_task(void);