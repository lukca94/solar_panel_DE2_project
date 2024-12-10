#include <stdio.h>
#include <avr/io.h>

#ifndef SERVO_H
#define SERVO_H

void ServoInit();
void ServoPinSet(uint8_t outputPinAngle, uint8_t outputPinTilt);
void ServoAngle(uint64_t position);
void ServoTilt(uint64_t position);

#endif // SERVO_H
