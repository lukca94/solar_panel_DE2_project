#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

#include "adc.h"
#include "servo.h"
#include "uart.h"

uint8_t FindHighest(uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin)
{

	uint16_t firstValue = adc_read(0);
	uint16_t secondValue = adc_read(1);
	uint16_t thirdValue = adc_read(2);
	uint16_t fourthValue = adc_read(3);

	// char string0[10];
	// itoa(firstValue, string0, 10);

	// uart_puts(string0);
	// uart_puts(", ");
	// itoa(secondValue, string0, 10);

	// uart_puts(string0);
	// uart_puts(", ");
	// itoa(thirdValue, string0, 10);

	// uart_puts(string0);
	// uart_puts(", ");
	// itoa(fourthValue, string0, 10);

	// uart_puts(string0);
	// uart_puts("\n");
	uint16_t array[] = {firstValue, secondValue, thirdValue, fourthValue};
	uint8_t n = sizeof(array) / sizeof(array[0]);

	uint16_t result = array[0];
	uint8_t highest = 0;

	for (int i = 1; i < n; i++)
	{
		if (array[i] > result)
		{
			result = array[i]; // Update max if a larger number is found
			highest = i;
		}
	}
	return highest;
}
bool IsInRange(uint8_t pin1, uint8_t pin2, uint8_t range)
{
	uint16_t value1 = adc_read(pin1);
	uint16_t value2 = adc_read(pin2);

	char string0[10];
	itoa(value1, string0, 10);
	uart_puts(string0);
	uart_puts(", ");

	itoa(value2, string0, 10);
	uart_puts(string0);
	uart_puts("\n ");

	if (abs(value1 - value2) > range)
		return true;
	else
		return false;
}

void Rotate(uint8_t PWM_PIN, uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin, uint8_t rangeTolerance)
{
	ServoPinSet(PWM_PIN);
	// default rotation
	uint16_t position = 90;
	Servo(position);

	uint8_t highestPin = FindHighest(firstPin, secondPin, thirdPin, fourthPin);

	// group A = pin0, pin1 1
	// group B = pin2, pin3 2

	char string0[10];
	itoa(highestPin, string0, 10);

	uart_puts(string0);
	uart_puts("\n");

	uint8_t currentGroup;

	if ((highestPin == 0) | (highestPin == 1))
	{
		currentGroup = 1;
	}
	else
	{
		currentGroup = 2;
	}

	itoa(currentGroup, string0, 10);

	uart_puts(string0);
	uart_puts("\n");

	if (currentGroup == 1)
	{
		while (IsInRange(0, 1, rangeTolerance))
		{
			if (highestPin == 0) // rotate left
			{
				position++;
				Servo(position);

				itoa(position, string0, 10);

				uart_puts(string0);
				uart_puts("\n");
			}
			else // rotate right
			{
				position--;
				Servo(position);

				itoa(position, string0, 10);
				uart_puts("position:");
				uart_puts(string0);
				uart_puts("\n");
			}
		}
	}
	else
	{
		while (IsInRange(2, 3, rangeTolerance))
		{
			if (highestPin == 2) // rotate left
			{
				position--;
				Servo(position);
			}
			else // rotate right
			{
				position++;
				Servo(position);
			}
		}
	}
}

void Tilt(uint8_t PWM_PIN, uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin, uint8_t rangeTolerance)
{
	ServoPinSet(PWM_PIN);
	// default rotation
	uint16_t position = 90;
	Servo(position);

	uint8_t highestPin = FindHighest(firstPin, secondPin, thirdPin, fourthPin);

	// group A = pin0, pin1 1
	// group B = pin2, pin3 2

	uint8_t highestGroup;
	if ((highestPin == 0) | (highestPin == 1))
	{
		highestGroup = 1;
	}
	else
	{
		highestGroup = 2;
	}

	if (highestGroup == 1) // tilt down
	{

		while (IsInRange(1, 3, rangeTolerance))
		{
			position--;
			Servo(position);
		}
	}
	else // tilt down
	{
		while (IsInRange(1, 3, rangeTolerance))
		{
			position++;
			Servo(position);
		}
	}
}