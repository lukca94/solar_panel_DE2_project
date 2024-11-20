#include <stdio.h>

#ifndef ADC_H
#define ADC_H
void adc_init();
uint16_t adc_read(uint8_t pin);
#endif // ADC_H
