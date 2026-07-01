#include <stdio.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "esp_sleep.h"

#include "common.h"
#include "system.h"
#include "sensor.h"
#include "display.h"

#include "sht4x.h"


sht4x_data_t g_sensor_data;

static TaskHandle_t s_sensor_handle;
static sht4x_t s_sensor_dev;
RTC_DATA_ATTR static float s_temperature_before_sleep;
RTC_DATA_ATTR static float s_humidity_before_sleep; 
RTC_DATA_ATTR static uint8_t s_overhumid_sleep_cycles;



static void sensor_task(void *pvParameters)
{
	// Wait for sensor to stabilize on first boot
	if(!g_system_after_deep_sleep){
		esp_sleep_enable_timer_wakeup(pdMS_TO_TICKS(2000));
		esp_light_sleep_start();
	}
	
	// Initialize device descriptor
	s_sensor_dev.repeatability	= SHT4X_HIGH;	// Read takes longer but is more accurate
	sht4x_init_desc(&s_sensor_dev, 0, SENSOR_PIN_SDA, SENSOR_PIN_SCL);
	sht4x_init(&s_sensor_dev);

	// Measuring
	sht4x_measure(&s_sensor_dev, &g_sensor_data.temperature, &g_sensor_data.humidity);
	
	// After deep sleep checking
	if(g_system_after_deep_sleep)
	{
		// Checking if there is any temperature change by 1 digit after decimal for more efficient data rendering
		if((int16_t)(g_sensor_data.temperature * 10) == (int16_t)(s_temperature_before_sleep * 10))
		{
			g_sensor_data.temperature_not_change = true; 
		}
		
		// Checking if there is any humidity change by 1 digit after decimal for more efficient data rendering
		if((int16_t)(g_sensor_data.humidity * 10) == (int16_t)(s_humidity_before_sleep * 10))
		{
			g_sensor_data.humidity_not_change = true;
			
			// Checking if a very high humidity reading is stuck (from SENSOR_OVERHUMID_THRESHOLD to 100). Means the sensor has condensation on itself and the heater needs to be turnt on. Checks for 4 active/sleep cycles before turning on heater (6 min MINIMUM)
			if(g_sensor_data.humidity >= SENSOR_OVERHUMID_THRESHOLD)
			{
				for(uint8_t i = 1; i <= (100 - SENSOR_OVERHUMID_THRESHOLD); i++)
				{
					if(g_sensor_data.humidity == SENSOR_OVERHUMID_THRESHOLD + i)
					{
						++s_overhumid_sleep_cycles;
						if(s_overhumid_sleep_cycles == 4)
						{
							s_sensor_dev.heater	= SHT4X_HEATER_HIGH_LONG;	// High heat long enough to cook an egg
							sht4x_measure(&s_sensor_dev, &g_sensor_data.temperature, &g_sensor_data.humidity);	// Heater is triggered by calling measurement
							g_sensor_data.heater_on = true;
							s_overhumid_sleep_cycles = 0;
						}
						break;
					}
				} 
			}
		}
		else 
		{
			s_overhumid_sleep_cycles = 0;
		}
	}
	s_temperature_before_sleep = g_sensor_data.temperature;
	s_humidity_before_sleep = g_sensor_data.humidity;
}


BaseType_t sensor_create_task()
{
	BaseType_t ret = xTaskCreatePinnedToCore(sensor_task, "sensor_task", SENSOR_TASK_STACK_SIZE, NULL, SENSOR_TASK_PRIORITY, &s_sensor_handle, SENSOR_TASK_CORE_ID);
	return ret;
}







