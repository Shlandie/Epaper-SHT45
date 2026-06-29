#pragma once

#include <stdint.h>


#define DISPLAY_SCK  12 
#define DISPLAY_MOSI 11
#define DISPLAY_CS   10
#define DISPLAY_DC   18
#define DISPLAY_RST  9
#define DISPLAY_BUSY 13


#ifdef __cplusplus
extern "C" {
#endif

void display_init(void);
void display_styling_setup(void);
void display_initial_draw(float temperature, float humidity, uint16_t temperature_cursor_XY[2], uint16_t humidity_cursor_XY[2]);
void display_draw_text1(void);
void display_draw_text2(void);

#ifdef __cplusplus
}
#endif

























