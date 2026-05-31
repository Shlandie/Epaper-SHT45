#pragma once

#include "freertos/FreeRTOS.h"

#define SENSOR_GPIO							5

extern float sensor_humidity, sensor_temperature;







BaseType_t sensor_create_task(void);