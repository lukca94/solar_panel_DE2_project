#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "uart.h"
#include "adc.h"

#define PHOTO_RES1 PC0
#define PHOTO_RES2 PC1
#define PHOTO_RES3 PC2
#define PHOTO_RES4 PC3

// #define PWM_OUT1 PB1

int main()
{

	uart_init(UART_BAUD_SELECT(250000, F_CPU));

	adc_init();
	SREG |= (1 << SREG_I); // enable interrupts probably

	DDRC = DDRC & ~(1 << PHOTO_RES1);//set input
	// DDRB = DDRB |= (1 << PWM_OUT1);

	// TCCR1A |= (1 << WGM10); //| (1 << COM1A1) | (1 << COM1B1);
	// TCCR1B |= (1 << CS12);	//| (1 << WGM12);

	// TCNT1 = 1249;

	// OCR1A = 230;

	while (1)
	{
		// uint8_t pin = 0;
		// uint16_t value = adc_read(pin);
		// char string[10];
		// itoa(value, string, 10);

		// uart_puts(string);
		// uart_puts("\n");

	}

	return 0;
}
