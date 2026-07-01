#include <stdio.h>

#include <SPI.h>

#include "display_wrapper.h"

#include "GxEPD2_BW.h"
#include "GxEPD2_290_BS.h"

#include "Fonts/TomThumb.h"
#include "bitmaps/splash.h"


//////////////////////////////////////////////////////////
//					GLOBAL VARIABLES					//
//////////////////////////////////////////////////////////

SPIClass hspi(HSPI);

GxEPD2_BW<GxEPD2_290_BS, GxEPD2_290_BS::HEIGHT> display(
    GxEPD2_290_BS(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST, DISPLAY_BUSY)
);

//////////////////////////////////////////////////////////
//				STATIC FUNCTION DECLARATIONS			//
//////////////////////////////////////////////////////////


static int16_t display_lift_y_axis(int16_t starting_y_position, int16_t lift_height);


//////////////////////////////////////////////////////////
//					GLOBAL FUNCTIONS					//
//////////////////////////////////////////////////////////


void display_init(void)
{	
	hspi.begin(DISPLAY_SCK, -1, DISPLAY_MOSI, DISPLAY_CS); // Re-map HSPI pins
	display.epd2.selectSPI(hspi, SPISettings(DISPLAY_SPI_SPEED, MSBFIRST, SPI_MODE0));
	
	display.init(DISPLAY_SPI_BITRATE);
}


void display_styling_setup(void)
{
	display.setRotation(DISPLAY_ROTATION);

	display.setTextColor(GxEPD_BLACK);
	display.setFont(&TomThumb); 																																	/* Try exprimeting with Kconfig for the abillity to choose different fonts on the go */   
	display.setTextSize(DISPLAY_FONT_SIZE);         
	display.setTextWrap(false);	// Avoids weird auto-wrap on small partial windows
}


void display_initial_splash(void)
{
	display.firstPage();
	do {
		display.drawBitmap(0, 0, splash_bitmap, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);
	} while (display.nextPage());
}


void display_initial_draw(float temperature, float humidity, int16_t temperature_cursor_XY[2], int16_t humidity_cursor_XY[2])
{
	display.setFullWindow(); // Function for full refresh
	
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


void display_update_data(float temperature, float humidity)
{
	display.setPartialWindow(DISPLAY_TEMP_TEXT_START_X,display_lift_y_axis(DISPLAY_TEMP_TEXT_START_Y, DISPLAY_FONT_HEIGHT) , DISPLAY_WIDTH - DISPLAY_TEMP_TEXT_START_X - DISPLAY_RIGHT_MARGIN_PARTIAL /* Shave pixels extra from the right */, DISPLAY_HUMID_TEXT_START_Y - (DISPLAY_TEMP_TEXT_START_Y - DISPLAY_FONT_HEIGHT) /* Total height from top of display to bottom of content - block to content start*/); // Function for partial refresh. (Shave off some unused area for some energy savings)
	
	display.firstPage();
	do {
		// Temperature render
	    display.fillScreen(GxEPD_WHITE);
		
	    display.setCursor(DISPLAY_TEMP_TEXT_START_X, DISPLAY_TEMP_TEXT_START_Y);
	    display.print("Temperature: ");
	    display.print(temperature, 1);
		
		// Draw degree symbol 
		int16_t dotX = display.getCursorX();
		int16_t dotY = display.getCursorY() - (5 * 3); 
		display.fillRect(dotX, dotY, 3, 3, GxEPD_BLACK); 
		
		// Push cursor X-axis so C and degree symbol aligns perfectly
		display.setCursor(display.getCursorX() + 4, display.getCursorY());
		display.print("C");

		// Humidity render
	    display.setCursor(DISPLAY_HUMID_TEXT_START_X, DISPLAY_HUMID_TEXT_START_Y);
	    display.print("Humidity: ");
	    display.print(humidity, 1);
		display.print((char)0x25); // Prints %
		
	} while (display.nextPage());
	
}


void display_clear_data_ghosting(float temperature, float humidity, int16_t temperature_cursor_XY[2], int16_t humidity_cursor_XY[2])
{
	// Because display/controller doesn't support full partial window refreshes on Its own, a DIY version has to be used that flashes the screen black then white
	// Refresh temperature data
	display.setPartialWindow(temperature_cursor_XY[0], display_lift_y_axis(temperature_cursor_XY[1],  DISPLAY_FONT_HEIGHT) , DISPLAY_TEMP_DATA_WIDTH, DISPLAY_FONT_HEIGHT);

	display.firstPage();
	do {
	    display.fillScreen(GxEPD_BLACK);
	} while (display.nextPage());
	
	display.firstPage();
	do {
	    display.fillScreen(GxEPD_WHITE);
	} while (display.nextPage());
	
	display.firstPage();
	do {
	    display.fillScreen(GxEPD_WHITE);
		display.setCursor(temperature_cursor_XY[0], temperature_cursor_XY[1]);
		display.print(temperature, 1);
				
		// Draw degree symbol 
		int16_t dotX = display.getCursorX();
		int16_t dotY = display.getCursorY() - (5 * 3); 
		display.fillRect(dotX, dotY, 3, 3, GxEPD_BLACK); 
		
		// Push cursor X-axis so C and degree symbol aligns perfectly
		display.setCursor(display.getCursorX() + 4, display.getCursorY());
		display.print("C");
	} while (display.nextPage());
	
	// Refresh humidity data
	display.setPartialWindow(humidity_cursor_XY[0], display_lift_y_axis(humidity_cursor_XY[1],  DISPLAY_FONT_HEIGHT), DISPLAY_HUMID_DATA_WIDTH, DISPLAY_FONT_HEIGHT);

	display.firstPage();
	do {
	    display.fillScreen(GxEPD_BLACK);
	} while (display.nextPage());

	display.firstPage();
	do {
	    display.fillScreen(GxEPD_WHITE);
	} while (display.nextPage());
	
	display.firstPage();
	do {
		display.fillScreen(GxEPD_WHITE);
		display.setCursor(humidity_cursor_XY[0], humidity_cursor_XY[1]);
		display.print(humidity, 1);
		display.print((char)0x25); // Prints %
	} while (display.nextPage());
}


//////////////////////////////////////////////////////////
//					STATIC FUNCTIONS					//
//////////////////////////////////////////////////////////


/*
* When using member function setPartialWindow() it starts setting the partial windows from top to bottom so you need the highest y coordinate of the window which which setCursor doesn't do
* @param starting_y_position - y position to lift, lift_height - amount to lift
* @return int16_t the lifted y coordinate
*/
static int16_t display_lift_y_axis(int16_t starting_y_position, int16_t lift_height)
{
	return (starting_y_position - lift_height);
}