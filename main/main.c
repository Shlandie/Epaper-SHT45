#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"

#include "esp_log.h"

#include "display.h"
#include "sensor.h"

static const char TAG[] = "app_main";

void app_main(void)
{
	ESP_LOGI(TAG, "Creating display task");
	assert(display_create_task() == pdTRUE);

	
	ESP_LOGI(TAG, "Creating sensor task");
	assert(sensor_create_task() == pdTRUE);
}
