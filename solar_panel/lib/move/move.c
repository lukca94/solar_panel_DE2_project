#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>

#include "adc.h"

uint8_t FindHighest(uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin)
{

	uint16_t firstValue = adc_read(firstPin);
	uint16_t secondValue = adc_read(secondPin);
	uint16_t thirdValue = adc_read(thirdPin);
	uint16_t fourthValue = adc_read(fourthPin);

	uint16_t array[4] = {firstValue, secondValue, thirdValue, fourthValue};

	uint16_t result = array[0];
	uint8_t highest = 0;

	for (uint8_t i = 0; i < 4; i++)
	{
		if (result < array[i])
		{
			result = array[i];
			highest = i;
		}
	}
	return highest;
}
bool isInRange(uint8_t pin1, uint8_t pin2, uint8_t range)
{
	uint8_t value1 = adc_read(pin1);
	uint8_t value2 = adc_read(pin2);

	if (abs(value1 - value2) > range)
		return true;
	else
		return false;
}

bool Rotate(uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin)
{
	uint8_t highestPin = FindHighest(firstPin, secondPin, thirdPin, fourthPin);
	// group A= pin0, pin1 1
	// group B= pin2, pin3 2
	uint8_t range = 20; // tolerance for pin reading
	uint8_t currentGroup;

	if (highestPin == 0 | highestPin == 1)
		currentGroup = 1;
	else
		currentGroup = 2;

	if (currentGroup == 1)
	{
		while (isInRange(0, 1, range))
		{
			if (highestPin == 0) // rotate left
			{
				// rotate -1
			}
			else
			{
				// rotate +1
			}
		}
	}
	else
	{
		while (isInRange(2, 3, range))
		{
			if (highestPin == 2) // rotate left
			{
				// rotate -1
			}
			else
			{
				// rotate +1
			}
		}
	}
	return true; // rotation done
}

bool Tilt(uint8_t highestPin)
{
	// group A= pin0, pin1 1
	// group B= pin2, pin3 2
	uint8_t range = 20; // tolerance for pin reading
	uint8_t higherGroup;
	if (highestPin == 0 | highestPin == 1)
	{
		higherGroup = 1;
	}
	else
	{
		higherGroup = 2;
	}

	if (higherGroup == 1) // tilt down
	{
		uint8_t firstPin = adc_read(2);
		uint8_t thirdPin = adc_read(3);

		while (abs(firstPin - thirdPin) > range)
		{
			// tilt
			uint8_t firstPin = adc_read(2);
			uint8_t secondPin = adc_read(3);
		}
	}
	return true;
}