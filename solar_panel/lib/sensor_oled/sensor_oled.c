#include "sensor_oled.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "oled.h"
#include "timer.h"

void oled_setup(void)
{
	oled_init(OLED_DISP_ON);
	oled_clrscr();

	oled_gotoxy(4, 1);
	oled_charMode(NORMALSIZE);
	oled_puts("Solar system");
	oled_drawRect(100, 20, 20, 1, WHITE);

	oled_gotoxy(0, 3);
	oled_puts("Current:");
	oled_gotoxy(15, 3);
	oled_puts("mA");

	oled_gotoxy(0, 5);
	oled_puts("Power:");

	oled_gotoxy(15, 5);
	oled_puts("W/m2");

	oled_gotoxy(0, 7);
	oled_puts("Intensity");

	oled_gotoxy(15, 7);
	oled_puts("%");

	oled_display();
}

void timer1_init(void)
{
	TIM1_ovf_1sec();
	TIM1_ovf_enable();
}

void draw_image(const uint16_t *img_data, uint8_t start_x, uint8_t start_y)
{
	for (uint8_t y = 0; y < 10; y++)
	{
		uint16_t row_data = img_data[y];
		for (uint8_t x = 0; x < 10; x++)
		{
			if (row_data & (1 << (9 - x)))
			{
				oled_drawPixel(start_x + x, start_y + y, WHITE);
			}
			else
			{
				oled_drawPixel(start_x + x, start_y + y, BLACK);
			}
		}
	}
}

void update_oled(uint8_t *sensor_values, uint16_t photoresistor_value, uint8_t voltage)
{
	char oled_msg[10];
	oled_gotoxy(10, 3);
	oled_puts("    ");
	oled_gotoxy(10, 3);
	sprintf(oled_msg, "%u.%u", sensor_values[0], sensor_values[1]);
	oled_puts(oled_msg);
	oled_gotoxy(10, 5);
	oled_puts("    ");
	oled_gotoxy(10, 5);
	uint16_t current_mA = (sensor_values[0] << 8) | sensor_values[1];
	float power = (current_mA / 1000.0) * voltage;
	sprintf(oled_msg, "%.2f", power);
	oled_puts(oled_msg);
	oled_gotoxy(10, 7);
	sprintf(oled_msg, "%u", (photoresistor_value * 100) / 1023);
	oled_puts(oled_msg);
	oled_display();
}
