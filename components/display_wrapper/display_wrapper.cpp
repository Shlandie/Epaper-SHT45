#include <stdio.h>
#include "display_wrapper.h"

#include <SPI.h>

#include "GxEPD2_BW.h"
#include "GxEPD2_290_BS.h"

#include "Fonts/TomThumb.h"


SPIClass hspi(HSPI);

GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(
    GxEPD2_290_BS(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST, DISPLAY_BUSY)
);


void display_init(void)
{	
	hspi.begin(DISPLAY_SCK, -1, DISPLAY_MOSI, DISPLAY_CS); // Re-map HSPI pins
	display.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));
	
	display.init(115200);
}


void display_styling_setup(void)
{
	display.setRotation(1);

	display.setTextColor(GxEPD_BLACK);
	display.setFont(&TomThumb);      // optional — default font works but is small/blocky
	display.setTextSize(3);          // scales default font; ignored if using a custom GFXfont
	display.setTextWrap(false);      // avoids weird auto-wrap on small partial windows
}



/*
* Display initial temperature and humidity text and data on launch. Send back cursor location before printing data for partial screen updates on the number values later.
* @param temperature & humidity - current temperature and humidity from sensor data, temperature_cursor_XY & humidity_cursor_XY - store cursor location for future partial scren updates on the number values
*/
void display_initial_draw(float temperature, float humidity, uint16_t temperature_cursor_XY[2], uint16_t humidity_cursor_XY[2])
{
	display.firstPage();
	do {
		// Temperature render
	    display.fillScreen(GxEPD_WHITE);
	    display.setCursor(30, 45);
	    display.print("Temperature: ");
		temperature_cursor_XY[0] = display.getCursorX();
		temperature_cursor_XY[1] = display.getCursorY();
	    display.print(temperature, 1);
		
		// Draw degree symbol 
		int16_t dotX = display.getCursorX();
		int16_t dotY = display.getCursorY() - (5 * 3); 
		display.fillRect(dotX, dotY, 3, 3, GxEPD_BLACK); 
		
		// Push cursor X-axis so C and degree symbol aligns perfectly
		display.setCursor(display.getCursorX() + 4, display.getCursorY());
		display.print("C");

		// Humidity render
	    display.setCursor(30, 95);
	    display.print("Humidity: ");
		humidity_cursor_XY[0] = display.getCursorX();
		humidity_cursor_XY[1] = display.getCursorY();
	    display.print(humidity, 1);
		display.print((char)0x25); // Prints %
	} while (display.nextPage());
	
}


void display_draw_text1(void)
{
	int16_t x1, y1;
	uint16_t w, h;
	
	display.getTextBounds("2222222222", 30, 30, &x1, &y1, &w, &h);
	display.setPartialWindow(x1, y1, w, h);
	
	display.firstPage();
	do {
	    display.fillScreen(GxEPD_WHITE);
	    display.setCursor(30, 30);
	    display.print("2222222222");
	} while (display.nextPage());
}

void display_draw_text2(void)
{
	int16_t x1, y1;
	uint16_t w, h;
	
	display.getTextBounds("2222222222", 30, 30, &x1, &y1, &w, &h);
	display.setPartialWindow(x1, y1, w, h);
	
	display.firstPage();
	do {
	    display.fillScreen(GxEPD_WHITE);
	    display.setCursor(30, 30);
	    display.print("3333333333");
	} while (display.nextPage());
}