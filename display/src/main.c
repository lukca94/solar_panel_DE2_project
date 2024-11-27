/* 
 * Read values from I2C (TWI) temperature/humidity sensor and send
 * them to OLED screen.
 * (c) 2023-2024 Tomas Fryza, MIT license
 *
 * Developed using PlatformIO and AVR 8-bit Toolchain 3.6.2.
 * Tested on Arduino Uno board and ATmega328P, 16 MHz.
 */

// -- Includes -------------------------------------------------------
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <twi.h>            // I2C/TWI library for AVR-GCC
#include <oled.h>
#include <stdio.h>          // C library for `sprintf`


// -- Defines --------------------------------------------------------
#define DHT_ADR 0x5c
#define DHT_HUM_MEM 0
#define DHT_TEMP_MEM 2


// -- Global variables -----------------------------------------------
volatile uint8_t flag_update_oled = 0;
volatile uint8_t dht12_values[5];


//--constant
const uint16_t image_data[15] = {
// 'images', 15x15px
0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xbe, 0xfe, 0x3e, 0xfc, 0x7e, 0xf0, 0x7e, 0xf8, 0x3e, 
0xfc, 0x1e, 0xfc, 0x7e, 0xf8, 0xfe, 0xfb, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe
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
    oled_puts("Power now");

    oled_gotoxy(15,7);
    oled_puts("W/m2");
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
    // Loop through the rows (15 rows)
    for (uint8_t y = 0; y < 15; y++) {
        // Get the pixel data for the current row (16 bits)
        uint16_t pixel_data = img_data[y];

        // Loop through the 15 pixels in the current row
        for (uint8_t x = 0; x < 15; x++) {
            // Check if the pixel is set to 1 (white) or 0 (black)
            if (pixel_data & (1 << (14 - x))) {  // Shift to check each bit
                oled_drawPixel(start_x + x, start_y + y, WHITE);  // Draw white pixel
            } else {
                oled_drawPixel(start_x + x, start_y + y, BLACK);  // Draw black pixel
            }
        }
    }
}





/*
 * Function: Main function where the program execution begins
 * Purpose:  Wait for new data from the sensor and sent them to OLED.
 * Returns:  none
 */
int main(void)
{
    char oled_msg[5];

    twi_init();
    oled_setup();
    timer1_init();

    sei();


    draw_image(image_data, 0,0);  // Draw at position (10, 5)

    draw_image(image_data, 101,0);  // Draw at position (10, 5)

    // Infinite loop
    while (1)
    {
        if (flag_update_oled == 1)
        {
            // Clear previous temperature value on OLED
            oled_gotoxy(10, 3);
            oled_puts("    ");

            // Display new temperature data
            oled_gotoxy(10, 3);
            sprintf(oled_msg, "%u.%u", dht12_values[2], dht12_values[3]);
            oled_puts(oled_msg);

            // Clear previous humidity values on OLED
            oled_gotoxy(10, 5);
            oled_puts("    ");

            // Display new humidity data
            oled_gotoxy(10, 5);
            sprintf(oled_msg, "%u.%u", dht12_values[0], dht12_values[1]);
            oled_puts(oled_msg);

            

            // Display new temperature data
            oled_gotoxy(10, 7);
            sprintf(oled_msg, "%u.%u", dht12_values[2], dht12_values[3]);
            oled_puts(oled_msg);


            oled_display();

            // Do not print it again and wait for the new data
            flag_update_oled = 0;
        }
    }

    // Will never reach this
    return 0;
}


// -- Interrupt service routines -------------------------------------
/*
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Read temperature and humidity from DHT12, SLA = 0x5c.
 */
ISR(TIMER1_OVF_vect)
{
    static uint8_t n_ovfs = 0;

    n_ovfs++;
    // Read the data every 1 secs
    if (n_ovfs >= 1)
    {
        n_ovfs = 0;
        twi_readfrom_mem_into(DHT_ADR, DHT_HUM_MEM, dht12_values, 5);
        flag_update_oled = 1;
    }
}
