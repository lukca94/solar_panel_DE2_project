#include <stdio.h>
#include <avr/io.h>

#ifndef SERVO_H
#define SERVO_H

void ServoInit();
void ServoPinSet(uint8_t outputPin);
void Servo(uint16_t position);

#endif //SERVO_H
