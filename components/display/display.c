#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"

#include "common.h"
#include "system.h"
#include "display.h"
#include "sensor.h"

#include "display_wrapper.h"


TaskHandle_t g_display_task_handle;

RTC_DATA_ATTR static int16_t s_temperature_cursor_XY[2];
RTC_DATA_ATTR static int16_t s_humidity_cursor_XY[2];


static void display_task(void *pvParameters)
{
	// Initialize display (SPI)
	display_init();
	// Setup all display styling (Rotation, text font, color, etc.)
	display_styling_setup();
	
	if (!g_system_after_deep_sleep)
	{
		// Wait for initial data before first draw
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		display_initial_draw(g_sensor_data.temperature, g_sensor_data.humidity, s_temperature_cursor_XY, s_humidity_cursor_XY);
	}
	

	for(;;)
	{		
		vTaskDelay(pdMS_TO_TICKS(4000));
		//display_update_data(g_sensor_data.temperature, g_sensor_data.humidity);
		//display_clear_data_ghosting(temperature2, humidity2, s_temperature_cursor_XY, s_humidity_cursor_XY);

		/*
		after_deep_sleep = 1;
		ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(60 * 1000000ULL));
		esp_deep_sleep_start();
		*/
	}
}


BaseType_t display_create_task()
{
	BaseType_t ret = xTaskCreatePinnedToCore(display_task, "display_task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &g_display_task_handle, DISPLAY_TASK_CORE_ID);
	return ret;
}







