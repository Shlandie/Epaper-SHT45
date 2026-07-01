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


//static const char TAG[] = "display_task";

TaskHandle_t g_display_task_handle;

RTC_DATA_ATTR static int16_t s_temperature_cursor_XY[2];
RTC_DATA_ATTR static int16_t s_humidity_cursor_XY[2];

float temperature = -25.5;
float humidity = 56.7;
float temperature2 = -20.1;
float humidity2 = -44.2;






static void display_task(void *pvParameters)
{
	// Initialize display (SPI)
	display_init();
	// Setup all display styling (Rotation, text font, color, etc.)
	display_styling_setup();
	
	if (!g_system_after_deep_sleep)
	{
		// Wait for initial data before first draw
		//ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		display_initial_draw(temperature, humidity, s_temperature_cursor_XY, s_humidity_cursor_XY);
	}
	

	for(;;)
	{		
		vTaskDelay(pdMS_TO_TICKS(4000));
		display_update_data(temperature2, humidity2);
		temperature2 += 1;
		humidity2 += 1;
		vTaskDelay(pdMS_TO_TICKS(4000));
		display_clear_data_ghosting(temperature2, humidity2, s_temperature_cursor_XY, s_humidity_cursor_XY);
		/*
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		char buf[64] = {0};
		snprintf(buf, sizeof(buf), "Temperature\n%.2f \xC2\xB0""C\nHumidity\n%.2f%%", sensor_temperature, sensor_humidity);
		if (!after_deep_sleep) vTaskDelay(pdMS_TO_TICKS(1000));
		
		after_deep_sleep = 1;
		snprintf(saved_humidity, sizeof(saved_humidity), "%.2f", sensor_humidity);
		snprintf(saved_temperature, sizeof(saved_temperature), "%.2f", sensor_temperature);
		ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(60 * 1000000ULL));
		esp_deep_sleep_start();
		*/
	}
}


BaseType_t display_create_task()
{
	BaseType_t ret = xTaskCreatePinnedToCore(display_task, "display_task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &display_task_handle, DISPLAY_TASK_CORE_ID);
	return ret;
}







