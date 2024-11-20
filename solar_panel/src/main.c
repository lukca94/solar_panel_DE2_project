#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "adc.h"

int main()
{
	uart_init(UART_BAUD_SELECT(250000, F_CPU));
	adc_init();
	SREG |= (1 << SREG_I); // enable interrupts
	DDRC = DDRC & ~(1 << PC0);

	while (1)
	{
		uint8_t pin = 1;
		uint16_t value = adc_read(pin);
		char string[10];
		itoa(value, string, 10);
		uart_puts(string);
		uart_puts("\r");
	}

	return 0;
}
