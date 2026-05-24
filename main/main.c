#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "display.h"
#include "sensor.h"

void app_main(void)
{
	display_create_task();
	sensor_create_task();
}
