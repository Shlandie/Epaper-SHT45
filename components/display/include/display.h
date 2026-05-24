#pragma once

#include "freertos/FreeRTOS.h"


#define DISPLAY_I2C_CONTROLLER 								0

#define DISPLAY_PIN_SDA										10
#define DISPLAY_PIN_SCL										11
#define DISPLAY_PIN_RST										-1

#define DISPLAY_I2C_HW_ADDR									0x3C
#define DISPLAY_SCL_SPEED									(400 * 1000)
#define DISPLAY_CMD_BITS									8
#define DISPLAY_PARAM_BITS									8
#define DISPLAY_DC_BIT_OFFSET								6

#define DISPLAY_V_RES										64
#define DISPLAY_H_RES										128






























BaseType_t display_create_task(void);
