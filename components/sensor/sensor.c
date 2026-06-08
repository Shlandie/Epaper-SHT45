#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "common.h"
#include "sensor.h"
#include "display.h"
#include "dht.h"

static TaskHandle_t sensor_handle;

float sensor_humidity, sensor_temperature;


static void sensor_task(void *pvParameters)
{
	while(1)
	{
		if(sensor_humidity != 0 && sensor_temperature != 0)
		{
			xTaskNotifyGive(display_handle);
			break;
		}
		dht_read_float_data(DHT_TYPE_AM2301, SENSOR_GPIO, &sensor_humidity, &sensor_temperature);
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
	
	for(;;)
	{
		dht_read_float_data(DHT_TYPE_AM2301, GPIO_NUM_5, &sensor_humidity,&sensor_temperature);
		xTaskNotifyGive(display_handle);
		if (!after_deep_sleep) vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


BaseType_t sensor_create_task()
{
	BaseType_t ret = xTaskCreatePinnedToCore(sensor_task, "sensor_task", SENSOR_TASK_STACK_SIZE, NULL, SENSOR_TASK_PRIORITY, &sensor_handle, SENSOR_TASK_CORE_ID);
	return ret;
}







