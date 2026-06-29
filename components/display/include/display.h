#pragma once

#include "freertos/FreeRTOS.h"



#define DISPLAY_PIN_BUSY    								13
#define DISPLAY_PIN_RST     								9
#define DISPLAY_PIN_DC     									18
#define DISPLAY_PIN_CS      								10
#define DISPLAY_PIN_SCK     								12
#define DISPLAY_PIN_MOSI    								11

#define DISPLAY_V_RES										128
#define DISPLAY_H_RES										296
#define DISPLAY_ROTATION									1



extern TaskHandle_t display_task_handle;

extern bool after_deep_sleep;
































BaseType_t display_create_task(void);
