#include <stdio.h>
#include <avr/io.h>

#include "servo.h"

void ServoInit()
{
	// Nastavení časovače 1 pro režim Fast PWM s TOP v ICR1
	TCCR1A |= (1 << WGM11) | (1 << COM1A1);				 // Režim PWM a přepínání OC1A
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); // Prescaler 8

	// Nastavení TOP hodnoty časovače (20 ms cyklus)
	ICR1 = 19999; // 16 MHz / (8 * 50 Hz) - 1 = 19999
}

void ServoPinSet(outputPin)
{
	// Disable all outputs
	DDRB = 0;
	// Nastavení portu jako výstup pro PWM
	DDRB |= (1 << outputPin);
}

void Servo(position)
{
	// Nastavení nové hodnoty pro OCR1A
	OCR1A = position;
}