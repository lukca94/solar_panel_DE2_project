#ifndef SENSOR_OLED_H
#define SENSOR_OLED_H

#include <stdint.h>

// Function prototypes
void oled_setup(void);
void timer1_init(void);
void draw_image(const uint16_t *img_data, uint8_t start_x, uint8_t start_y);
void update_oled(uint8_t *sensor_values, uint16_t photoresistor_value, uint8_t voltage);

#endif // SENSOR_OLED_H
