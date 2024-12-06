
/* 
 * Read values from I2C (TWI) temperature/humidity sensor and send
 * them to OLED screen.
 * (c) 2023-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 * 
 * Improvements and adjustments for course DE2 project made by group Mackovych, Svec, Matejcek and Muzikar
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC         
#include <twi.h>            // I2C/TWI library for AVR-GCC
#include <stdio.h>          // C library for `sprintf`
#include <util/delay.h>
#include <stdlib.h>
// included libraries
#include "timer.h"
#include "uart.h"
#include "oled.h"
#include "adc.h"

// -- Defines --------------------------------------------------------
#define SENSOR_ADR 0x3c        // Replace with the actual I2C address for your sensor
#define SENSOR_CURR_MEM PC1      // Memory location for current data
#define SENSOR_POWER_MEM PC2     // Memory location for power data
#define SENSOR_LIGHT_MEM PC4     // Memory location for light intensity
            // Defining stable voltage, for calculated values only (NOT real value from solar panel)
#define PHOTO_RES1 PC0         // TEST RESISTOR

#define F_CPU 16000000UL // Frekvence CPU (16 MHz pro Arduino Uno)
#define PWM_PORT PB1
// -- Global variables -----------------------------------------------
volatile uint8_t flag_update_oled = 0;
volatile uint8_t sensor_values[4]; // Adjusted for three 16-bit values
volatile uint16_t photoresistor_value = 0;
         uint8_t voltage = 5;


const uint16_t image_data[10] = {
// Binary example of picture for OLED display
0x40, 0x00, 0x48, 0x00, 0x00, 0x00, 0xc4, 0x00, 0x44, 0x00, 0x28, 0x00, 0x18, 0x00, 0x0c, 0x00, 0x04, 0x00, 0x00, 0x00
};

// -- Function definitions -------------------------------------------
void oled_setup(void)
{
    oled_init(OLED_DISP_ON);
    oled_clrscr();

    oled_gotoxy(4, 1);
    oled_charMode(NORMALSIZE);
    oled_puts("Solar system");

    oled_drawRect(100,20,20,1,WHITE);
   
    oled_gotoxy(0, 3);
    oled_puts("Current:");

    oled_gotoxy(15,3);
    oled_puts("mA");

    oled_gotoxy(0, 5);
    oled_puts("Power:");
   
    oled_gotoxy(15,5);
    oled_puts("W/m2");
    
    oled_gotoxy(0, 7);
    oled_puts("Intensity");

    oled_gotoxy(15,7);
    oled_puts("%");
    // Copy buffer to display RAM
    oled_display();
}


void timer1_init(void)
{
    TIM1_ovf_1sec();
    TIM1_ovf_enable();
}

void draw_image(const uint16_t* img_data, uint8_t start_x, uint8_t start_y)
{
    // Procházíme řádky obrázku (10 řádků)
    for (uint8_t y = 0; y < 10; y++) {
        // Načteme data pro aktuální řádek (10 bitů)
        uint16_t row_data = img_data[y];

        // Procházíme sloupce obrázku (10 pixelů v řádku)
        for (uint8_t x = 0; x < 10; x++) {
            // Zjistíme, zda je aktuální pixel bílý (1) nebo černý (0)
            if (row_data & (1 << (9 - x))) {  // Posuneme bit na správnou pozici
                oled_drawPixel(start_x + x, start_y + y, WHITE);  // Vykreslíme bílý pixel
            } else {
                oled_drawPixel(start_x + x, start_y + y, BLACK);  // Vykreslíme černý pixel
            }
        }
    }
}

// OLED displaying updated data after timer overflows
int main(void)
{
    char oled_msg[10];
    twi_init();
    oled_setup();
    timer1_init();

    DDRB |= (1 << PWM_PORT);
    DDRC = DDRC & ~(1 << PHOTO_RES1);
    TCCR1A |= (1 << WGM11) | (1 << COM1A1);
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
    ICR1 = 19999;

    int angle = 1500;
    int min_pulse = 900;
    int max_pulse = 2000;
    OCR1A = angle;

    uart_init(UART_BAUD_SELECT(250000, F_CPU));
    adc_init();
    SREG |= (1 << SREG_I);
    

    sei();

    draw_image(image_data, 5,0);  // Draw at position (10, 5)

    draw_image(image_data, 109,0);  // Draw at position (10, 5)

    // Infinite loop
    while (1)
    {

        uint8_t pin = 0;
        uint16_t value = adc_read(pin);
        value = 100-((value/1023)*100);

        char string[10];
        itoa(value, string, 10);
        uart_puts(string);
        uart_puts("\n");

        _delay_ms(100);
       
        oled_gotoxy(10, 7);
        oled_puts(string);
        oled_puts("  ");
        oled_display();
        if (flag_update_oled == 1)
        {
            // Clear previous values on OLED and set new ones for Current
            oled_gotoxy(10, 3);
            oled_puts("    ");
            oled_gotoxy(10, 3);
            sprintf(oled_msg, "%u.%u", sensor_values[0], sensor_values[1]);
            oled_puts(oled_msg);

            // Clear previous values on OLED and set new ones for Power
            oled_gotoxy(10, 5);
            oled_puts("    ");
            oled_gotoxy(10, 5);
            // Renewed data of current to calculate power with fictive value of voltage
            uint16_t current_mA = (sensor_values[0] << 8) | sensor_values[1]; 
            float power = (current_mA / 1000.0) * voltage;
            sprintf(oled_msg, "%.2f", power);
            oled_puts(oled_msg);

            oled_gotoxy(12, 7);
            sprintf(oled_msg, "%s", string);
            oled_puts(oled_msg);

            // Clear previous values on OLED and set new ones for Intensity
            //oled_gotoxy(10, 7);
            //sprintf(oled_msg, "%u", (photoresistor_value * 100)/1023);
            //oled_puts(oled_msg);


            oled_display();

            // Waiting until another values update
            flag_update_oled = 0;
        }
        
        // Výpočet nového úhlu (převod ADC na úhel)
        angle = (37 * value) / 10 + 5;

        // Zajištění, že hodnota je v povoleném rozsahu
        if (angle < min_pulse) angle = min_pulse;
        if (angle > max_pulse) angle = max_pulse;

        OCR1A = angle; // Nastavení PWM
        _delay_ms(100);
        // Převod ADC hodnoty na řetězec a odeslání přes UART
        
        

    }

    // Will never reach this
    return 0;
}



// -- Interrupt service routines -------------------------------------

/*
* Function: Timer/Counter1 overflow interrupt

 * Updating values from sensors every 1 second
 */ 
ISR(TIMER1_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    n_ovfs++;
    // Read the data every 1 secs
    if (n_ovfs >= 1)
    {
        n_ovfs = 0;

       // twi_readfrom_mem_into(SENSOR_ADR, SENSOR_CURR_MEM, &sensor_values[], 4);
        
        flag_update_oled = 1;
    }
}
