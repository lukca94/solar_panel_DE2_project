#include <stdio.h>
#include <stdbool.h>

#ifndef MOVE_H
#define MOVE_H

uint8_t FindHighest(uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin);

bool IsInRange(uint8_t pin1, uint8_t pin2, uint8_t range);

void Rotate(uint8_t PWM_PIN, uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin, uint8_t rangeTolerance);

void Tilt(uint8_t PWM_PIN, uint8_t firstPin, uint8_t secondPin, uint8_t thirdPin, uint8_t fourthPin, uint8_t rangeTolerance);

#endif // MOVE_H
