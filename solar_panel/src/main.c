#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/////////////////////////////////////////////////////////
#include <twi.h>   // I2C/TWI library for AVR-GCC
#include <stdio.h> // C library for `sprintf`
#include "timer.h"
#include "oled.h"

// const uint16_t image_data[10] = {
// 	// Binary example of picture for OLED display
// 	0x40, 0x00, 0x48, 0x00, 0x00, 0x00, 0xc4, 0x00, 0x44, 0x00, 0x28, 0x00, 0x18, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x00, 0x00};

// const uint16_t image_data[10] = {
// 	// Binary example of picture for OLED display
// 	0x40, 0x00, 0x48, 0x00, 0x00, 0x00, 0xc4, 0x00, 0x44, 0x00, 0x28, 0x00, 0x18, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x00, 0x00};

// -- Function definitions -------------------------------------------
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
	oled_puts("Intensity:");

	oled_gotoxy(15, 5);
	oled_puts("%");
	// Copy buffer to display RAM
	oled_display();
}

void draw_image(const uint16_t *img_data, uint8_t start_x, uint8_t start_y)
{
	// Procházíme řádky obrázku (10 řádků)
	for (uint8_t y = 0; y < 10; y++)
	{
		// Načteme data pro aktuální řádek (10 bitů)
		uint16_t row_data = img_data[y];

		// Procházíme sloupce obrázku (10 pixelů v řádku)
		for (uint8_t x = 0; x < 10; x++)
		{
			// Zjistíme, zda je aktuální pixel bílý (1) nebo černý (0)
			if (row_data & (1 << (9 - x)))
			{													 // Posuneme bit na správnou pozici
				oled_drawPixel(start_x + x, start_y + y, WHITE); // Vykreslíme bílý pixel
			}
			else
			{
				oled_drawPixel(start_x + x, start_y + y, BLACK); // Vykreslíme černý pixel
			}
		}
	}
}
/////////////////////////////////////////////////////////

#define F_CPU 16000000UL // Frekvence CPU (16 MHz pro Arduino Uno)
#define PWM_PORT_ANGLE PB1
#define PWM_PORT_TILT PB2

// Definice od Lukyho

#include "uart.h"
#include "adc.h"
#include "servo.h"
#include "move.h"

#define PHOTO_RES1 PC0
#define PHOTO_RES2 PC1
#define PHOTO_RES3 PC2
#define PHOTO_RES4 PC3

const uint16_t image_data[20] = {
	// Binary example of picture for OLED display
	0x40, 0x00, 0x48, 0x00, 0x00, 0x00, 0xc4, 0x00, 0x44, 0x00, 0x28, 0x00, 0x18, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x00, 0x00};

int main(void)
{ //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// char oled_msg[10];
	twi_init();
	oled_setup();

	draw_image(image_data, 5, 0); // Draw at position (10, 5)

	draw_image(image_data, 109, 0); // Draw at position (10, 5)

	////////////////////////////////////////////////////////////////////////////////////////////////////

	uart_init(UART_BAUD_SELECT(250000, F_CPU));

	ServoInit();
	ServoPinSet(PWM_PORT_ANGLE, PWM_PORT_TILT);

	adc_init();
	SREG |= (1 << SREG_I); // enable interrupts probably

	DDRC = DDRC & ~(1 << PHOTO_RES1) & ~(1 << PHOTO_RES2) & ~(1 << PHOTO_RES3) & ~(1 << PHOTO_RES4); // set input

	// OCR1B = 2000;
	ServoAngle(90);
	ServoTilt(90);

	Rotate(PHOTO_RES1, PHOTO_RES2, PHOTO_RES3, PHOTO_RES4, 70);
	Tilt(PHOTO_RES1, PHOTO_RES2, PHOTO_RES3, PHOTO_RES4, 70);

	while (1)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		uint8_t pin = 6;
		uint16_t value = adc_read(pin);
		char string[10];

		value = 1000 * (-565 + value) * (1 / 27.8); // Vale by chěla předělat do floatu hádám

		itoa(value, string, 10);
		uart_puts(string);
		uart_puts("\n");
		_delay_ms(100);

		oled_gotoxy(9, 3);
		oled_puts(string);
		oled_puts("  ");
		oled_display();

		uint16_t intensity = (adc_read(PHOTO_RES1) + adc_read(PHOTO_RES1) + adc_read(PHOTO_RES1) + adc_read(PHOTO_RES1)) / 4;
		intensity = 100 * (intensity / 760);
		char string2[10];
		itoa(intensity, string2, 10);
		oled_gotoxy(9, 5);
		oled_puts(string2);
		oled_puts("  ");
		oled_display();

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// uint8_t pin0 = 0;
		// uint16_t value0 = adc_read(pin0);
		// char string0[10];
		// itoa(value0, string0, 10);

		// uart_puts(string0);
		// uart_puts(", ");

		// uint8_t pin1 = 1;
		// uint16_t value1 = adc_read(pin1);
		// char string1[10];
		// itoa(value1, string1, 10);

		// uart_puts(string1);
		// uart_puts(", ");

		// uint8_t pin2 = 2;
		// uint16_t value2 = adc_read(pin2);
		// char string2[10];
		// itoa(value2, string2, 10);

		// uart_puts(string2);
		// uart_puts(", ");

		// uint8_t pin3 = 3;
		// uint16_t value3 = adc_read(pin3);
		// char string3[10];
		// itoa(value3, string3, 10);

		// uart_puts(string3);
		// uart_puts("\r\n");
		// _delay_ms(100);
	}
}
