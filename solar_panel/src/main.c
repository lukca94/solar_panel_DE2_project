#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define F_CPU 16000000UL // Frekvence CPU (16 MHz pro Arduino Uno)
#define PWM_PORT PB1

// Definice od Lukyho

#include "uart.h"
#include "adc.h"

#define PHOTO_RES1 PC0
#define PHOTO_RES2 PC1
#define PHOTO_RES3 PC2
#define PHOTO_RES4 PC3


int main(void) {
    // Nastavení portu PB1 jako výstup pro PWM
    DDRB |= (1 << PWM_PORT);

    // Nastavení časovače 1 pro režim Fast PWM s TOP v ICR1
    TCCR1A |= (1 << WGM11) | (1 << COM1A1); // Režim PWM a přepínání OC1A
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); // Prescaler 8

    // Nastavení TOP hodnoty časovače (20 ms cyklus)
    ICR1 = 19999; // 16 MHz / (8 * 50 Hz) - 1 = 19999

    // Inicializace OCR1A (počáteční šířka pulzu)
    int angle = 1500;     // Počáteční hodnota 1.5 ms (střední poloha)
    //int step = 100;        // Velikost kroku pro změnu šířky pulzu
    //int min_pulse = 900; // 1 ms odpovídá -90°
    //int max_pulse = 4800; // 2 ms odpovídá +90°
    OCR1A = angle;
    int Analog = 0;
    
    //LUKY
    uart_init(UART_BAUD_SELECT(250000, F_CPU));

    adc_init();
    SREG |= (1 << SREG_I); // enable interrupts probably

    DDRC = DDRC & ~(1 << PHOTO_RES1);//set input

    while (1) {
         uint8_t pin = 0;
         uint16_t value = adc_read(pin);
         char string[10];
         itoa(value, string, 10);

         uart_puts(string);
         uart_puts("\n");

        // Změna hodnoty pulzu
        Analog = value;

        angle = 3.7*Analog + 5;


        // Nastavení nové hodnoty pro OCR1A
        OCR1A = angle;

        // Krátké zpoždění pro plynulý pohyb
        

    }
}
