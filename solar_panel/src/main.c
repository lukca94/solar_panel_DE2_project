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
		uint8_t pin = 0;
		uint16_t value = adc_read(pin);
		char string[10];
		itoa(value, string, 10);

		uart_puts(string);
		uart_puts("\n");

	}
}
