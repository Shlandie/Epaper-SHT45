#pragma once

#include "freertos/FreeRTOS.h"


extern TaskHandle_t g_display_task_handle;
extern RTC_DATA_ATTR bool g_after_deep_sleep;




























BaseType_t display_create_task(void);
