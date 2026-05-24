#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "common.h"
#include "sensor.h"

TaskHandle_t sensor_handle;


void sensor_task(void *pvParameters)
{
	for(;;)
	{
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


void sensor_create_task()
{
	ESP_ERROR_CHECK(xTaskCreatePinnedToCore(sensor_task, "sensor_task", SENSOR_TASK_STACK_SIZE, NULL, SENSOR_TASK_PRIORITY, &sensor_handle, SENSOR_TASK_CORE_ID));
}