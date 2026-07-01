#pragma once

#include "freertos/FreeRTOS.h"

#define SENSOR_PIN_SDA							6
#define SENSOR_PIN_SCL							7

#define SENSOR_OVERHUMID_THRESHOLD				95


typedef struct sht4x_data{
	float temperature;
	float humidity;
	bool temperature_not_change;	// Did it change after deep sleep
	bool humidity_not_change;
	bool heater_on;
}sht4x_data_t;





BaseType_t sensor_create_task(void);