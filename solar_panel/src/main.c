#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_CPU 16000000UL // Frekvence CPU (16 MHz pro Arduino Uno)
#define PWM_PORT_ANGLE PB1
#define PWM_PORT_TILT PB2

// Definice od Lukyho

#include "uart.h"
#include "adc.h"
#include "servo.h"

#define PHOTO_RES1 PC0
#define PHOTO_RES2 PC1
#define PHOTO_RES3 PC2
#define PHOTO_RES4 PC3

int main(void)
{
	ServoInit();

	uart_init(UART_BAUD_SELECT(250000, F_CPU));

	adc_init();
	SREG |= (1 << SREG_I); // enable interrupts probably

	DDRC = DDRC & ~(1 << PHOTO_RES1); // set input

	while (1)
	{
		uint8_t pin0 = 0;
		uint16_t value0 = adc_read(pin0);
		char string0[10];
		itoa(value0, string0, 10);

		uart_puts(string0);
		uart_puts(", ");

		uint8_t pin1 = 1;
		uint16_t value1 = adc_read(pin1);
		char string1[10];
		itoa(value1, string1, 10);

		uart_puts(string1);
		uart_puts(", ");

		uint8_t pin2 = 2;
		uint16_t value2 = adc_read(pin2);
		char string2[10];
		itoa(value2, string2, 10);

		uart_puts(string2);
		uart_puts(", ");

		uint8_t pin3 = 3;
		uint16_t value3 = adc_read(pin3);
		char string3[10];
		itoa(value3, string3, 10);

		uart_puts(string3);
		uart_puts("\r\n");
		_delay_ms(100);
	}
}
