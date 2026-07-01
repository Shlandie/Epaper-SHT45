#pragma once

#include <stdint.h>

// Display SPI config
#define DISPLAY_SPI_BITRATE							115200
#define DISPLAY_SPI_SPEED							400000

#define DISPLAY_SCK  								12 
#define DISPLAY_MOSI 								11
#define DISPLAY_CS   								10
#define DISPLAY_DC   								18
#define DISPLAY_RST  								9
#define DISPLAY_BUSY 								13

// Display render and styling
#define DISPLAY_WIDTH								296
#define DISPLAY_HEIGHT								128

#define DISPLAY_ROTATION							1
#define DISPLAY_FONT_SIZE							3

#define DISPLAY_TEMP_TEXT_START_X					30
#define DISPLAY_TEMP_TEXT_START_Y					45
#define DISPLAY_HUMID_TEXT_START_X					30
#define DISPLAY_HUMID_TEXT_START_Y					95
#define DISPLAY_RIGHT_MARGIN_PARTIAL				30

// A little bit extra width for safe partial refreshes
#define DISPLAY_TEMP_DATA_WIDTH						80
#define DISPLAY_HUMID_DATA_WIDTH					80		
#define DISPLAY_FONT_HEIGHT							15


#ifdef __cplusplus
extern "C" {
#endif


void display_init(void);

void display_styling_setup(void);

/*
* Render monochrome image C-style array
*/
void display_initial_splash(void);

/*
* Display initial temperature and humidity text and data on launch. Send back cursor location before printing data for partial screen updates on the number values later.
* @param temperature & humidity - current temperature and humidity from sensor data, temperature_cursor_XY & humidity_cursor_XY - store cursor location for future partial scren updates on the number values
*/
void display_initial_draw(float temperature, float humidity, int16_t temperature_cursor_XY[2], int16_t humidity_cursor_XY[2]);

void display_update_data(float temperature, float humidity);

void display_clear_data_ghosting(float temperature, float humidity, int16_t temperature_cursor_XY[2], int16_t humidity_cursor_XY[2]);

#ifdef __cplusplus
}
#endif

























