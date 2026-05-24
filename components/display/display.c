#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"

#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"

#include "common.h"
#include "display.h"

TaskHandle_t display_handle;


void display_task(void *pvParameters)
{
	for(;;)
	{
		
	}
}


void display_create_task()
{
	ESP_ERROR_CHECK(xTaskCreatePinnedToCore(display_task, "display_task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &display_handle, DISPLAY_TASK_CORE_ID));
}