#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "servo.h"
#include "uart.h"

void ServoInit()
{
	// Nastavení časovače 1 pro režim Fast PWM s TOP v ICR1
	TCCR1A |= (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1); // Režim PWM a přepínání OC1A
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);	// Prescaler 8

	// Nastavení TOP hodnoty časovače (20 ms cyklus)
	ICR1 = 19999; // 16 MHz / (8 * 50 Hz) - 1 = 19999
}

void ServoPinSet(uint8_t outputPinAngle, uint8_t outputPinTilt)
{
	// Nastavení portu jako výstup pro PWM
	DDRB |= (1 << outputPinAngle) | (1 << outputPinTilt);
}

void ServoAngle(uint64_t position) // 0-180
{
	uint64_t value = (position * 3200 / 180) + 1000;

	// Nastavení nové hodnoty pro OCR1A
	OCR1A = value;

	char string0[10];
	itoa(value, string0, 10);

	uart_puts(string0);
	uart_puts("\n");
	// 4700 max 1500 min
	_delay_ms(100);
}

void ServoTilt(uint64_t position) // 0-180
{
	uint64_t value = (position * 2000 / 180) + 2000;

	// Nastavení nové hodnoty pro OCR1B
	OCR1B = value;

	// char string0[10];
	// itoa(value, string0, 10);

	// uart_puts(string0);
	// uart_puts("\n");
	// 4700 max 1500 min
	_delay_ms(100);

	// 2000 0
	//  3000 90
	//  4000 180
}