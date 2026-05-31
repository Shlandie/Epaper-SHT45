#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"

#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "esp_lvgl_port.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_panel_ssd1306.h"

#include "common.h"
#include "portmacro.h"
#include "widgets/label/lv_label.h"
#include "display.h"
#include "sensor.h"

LV_IMG_DECLARE(cat);

static const char TAG[] = "display_task";

TaskHandle_t display_handle;
static lv_disp_t *display_lvgl_handle;

/*
* Initialize LVGL display styling etc. Return label to control rendered text
* @return pointer to the label
*/
static display_ui_t display_initialize_lvgl_styling()
{
	lv_obj_t *scr = NULL;
	display_ui_t ui = {0};
	
	if (lvgl_port_lock(0)) {
	    lv_disp_set_rotation(display_lvgl_handle, LV_DISPLAY_ROTATION_0);

	    scr = lv_disp_get_scr_act(display_lvgl_handle);
		ui.img = lv_img_create(scr);
	    ui.label = lv_label_create(scr);
		
		lv_obj_set_style_text_font(ui.label, &lv_font_montserrat_14, LV_PART_MAIN);
		lv_obj_set_style_text_align(ui.label, LV_TEXT_ALIGN_CENTER, 0);
	    lv_label_set_long_mode(ui.label, LV_LABEL_LONG_MODE_WRAP);
		lv_label_set_text(ui.label, "");
	    lv_obj_set_width(ui.label, lv_display_get_physical_horizontal_resolution(display_lvgl_handle));

		// Setting introductory image before showing any temperature and humidity data
		lv_img_set_src(ui.img, &cat);
	    lv_obj_align(ui.img, LV_ALIGN_CENTER, 0, 0);
		
	    lvgl_port_unlock();
	}
	return ui;
}

/*
* Initialize the ssd1306 display
*/
static void display_initialize_ssd1306()
{
	   ESP_LOGI(TAG, "Initialize I2C bus");
	    i2c_master_bus_handle_t i2c_bus = NULL;
	    i2c_master_bus_config_t i2c_bus_config = {
	        .clk_source = I2C_CLK_SRC_DEFAULT,
	        .glitch_ignore_cnt = 7,
	        .i2c_port = DISPLAY_I2C_CONTROLLER,
	        .sda_io_num = DISPLAY_PIN_SDA,
	        .scl_io_num = DISPLAY_PIN_SCL,
	        .flags.enable_internal_pullup = true,
	    };
	    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_bus_config, &i2c_bus));

	    ESP_LOGI(TAG, "Install panel IO");
	    esp_lcd_panel_io_handle_t io_handle = NULL;
	    esp_lcd_panel_io_i2c_config_t io_config = {
	        .dev_addr = DISPLAY_I2C_HW_ADDR,
	        .scl_speed_hz = DISPLAY_SCL_SPEED,
	        .control_phase_bytes = 1,               		// According to SSD1306 datasheet
	        .lcd_cmd_bits = DISPLAY_CMD_BITS,   			// According to SSD1306 datasheet
	        .lcd_param_bits = DISPLAY_DC_BIT_OFFSET, 		// According to SSD1306 datasheet
	        .dc_bit_offset = 6,                     		// According to SSD1306 datasheet
	    };
	    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

	    esp_lcd_panel_handle_t panel_handle = NULL;
	    esp_lcd_panel_dev_config_t panel_config = {
	        .bits_per_pixel = 1,
	        .reset_gpio_num = DISPLAY_PIN_RST,
		};
	    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
	        .height = DISPLAY_V_RES,
	    };
	    panel_config.vendor_config = &ssd1306_config;
	    ESP_LOGI(TAG, "Install SSD1306 panel driver");
	    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
		
		ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
		ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
		ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
		
		ESP_LOGI(TAG, "Initialize LVGL");
	    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
	    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

	    const lvgl_port_display_cfg_t disp_cfg = {
	        .io_handle = io_handle,
	        .panel_handle = panel_handle,
	        .buffer_size = DISPLAY_H_RES * DISPLAY_V_RES,
	        .double_buffer = true,
	        .hres = DISPLAY_H_RES,
	        .vres = DISPLAY_V_RES,
	        .monochrome = true,
	        .color_format = LV_COLOR_FORMAT_I1,
	        .rotation = {
	            .swap_xy = false,
	            .mirror_x = false,
	            .mirror_y = false,
	        },
	        .flags = {
	            .swap_bytes = false,
	            .sw_rotate = false,
	        }
		    };
	    display_lvgl_handle = lvgl_port_add_disp(&disp_cfg);
		assert(display_lvgl_handle != NULL);
}


static void display_task(void *pvParameters)
{
	display_initialize_ssd1306();
	display_ui_t ui = display_initialize_lvgl_styling();
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	if (lvgl_port_lock(0)) {
		lv_obj_del(ui.img);
		lvgl_port_unlock();
	}
	
	for(;;)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		char buf[64] = {0};
		snprintf(buf, sizeof(buf), "Temperature\n%.2f \xC2\xB0""C\nHumidity\n%.2f%%", sensor_temperature, sensor_humidity);
		if (lvgl_port_lock(0)) {
			lv_label_set_text(ui.label,buf);
			lvgl_port_unlock();
		}
	}
}


BaseType_t display_create_task()
{
	BaseType_t ret = xTaskCreatePinnedToCore(display_task, "display_task", DISPLAY_TASK_STACK_SIZE, NULL, DISPLAY_TASK_PRIORITY, &display_handle, DISPLAY_TASK_CORE_ID);
	return ret;
}







