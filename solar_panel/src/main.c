#include <avr/io.h>
#include "uart.h"

void adc_init(void)
{
	ADMUX |= (1 << REFS0);												// 0b01000000; // a0 = 0000, a1 = 0001, a2 = 0010, a3 = 0011....
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 0b10101111;  // ADSC 6bit spustí převod, 4bit konverze done, 125 kHz
}

uint16_t adc_read(void)
{
	ADCSRA |= (1 < ADSC); // start conversion

	while (ADCSRA & (1 << ADSC))
	{
		;
	}

	uint16_t result = ADCL;
	result |= (ADCH << 8);

	return result;
}

int main()
{

	adc_init();
	SREG |= (1 < SREG_I); // enable interrupts

	while (1)
	{
		uint16_t value = adc_read();
		char string[10];
		itoa(value, string, 10);
		uart_puts(string);
		uart_puts("\r\n");
	}

	return 0;
}
