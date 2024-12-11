# solar_panel_DE2_project
Monitoring and adjusting performance of a solar panel


Team members:

Lukáš Mackových\
David Matějček\
Jaroslav Švec\
David Muzikář

### Preparation
In order to create solar monitoring with current sensor, four photoresistors and OLED display, which monitors the current and power value. First thing to do was creating a schemematic and system algorithm.
OLED display uses I2C interface. Libraries created or learned about in AVR DE2 course were updated and used in this project. 

Structure of code

- Servo rotation
- photoresistor monitoring
- automative servo motor rotation
- values storing
- OLED displaying values
  
### Hardware setup
Hardware connection is showed on schematic below.
![schematic](https://github.com/user-attachments/assets/cd43df7a-ca8a-48cf-9797-bcc118313665)

The schematic for the first prototype on the UNO arduino, unfortunately it does not have enough pins to read analog values. The first idea was to switch the signal source of one photoresistor with a current sensor (as shown in the schematic). Eventually we solved the problem by using the Arduino NANO board, which has up to 8 analogue pins. 4 of these pins are used to read the voltage level on the divider consisting of a resistor (10k) and a photoresistor (PC0, PC1, PC2, PC3). The connectors PC4 (SDA) and PC5 (SCL), are used for serial synchronous communication IIC, for writing data to the display. We connected a probe to analog pin 6.

The pins PB1 (D9) and PB2 (D10) are used to control the resistors. The control is done by PWM signal. 


![image](https://github.com/user-attachments/assets/803730a9-5853-4a67-862d-e48125dab336)
*Arduino NANO pinout, from [1]*

### Software setup

#### Servo
![image](https://github.com/user-attachments/assets/7abaa3bd-5444-482b-98fa-11c60e8b1147)
*Servo control according to datasheet, from [2]*

According to the servo datasheet, we can find out that a PWM signal of the exact frequency and duty cycle is needed. The easiest way to achieve this signal is by using Timer1, which is already built into the ATMega328P. 
![image](https://github.com/user-attachments/assets/25499d6c-944d-4033-aecf-a007b1f6d0ba)
*Timer1 features, from [3]*
The easiest way to control the servo is mode 14 (Fast PWM). In this particular case, we can set the top counter value using the ICR1 register, this is convenient because it is very easy to set the 50 Hz needed for control. The value of the register can be calculated using the relation:

$TOP = \frac{f_{CPU}}{N\cdot f_{desired}} - 1$

The result for $f = 50\,Hz$ is $ICR1 = 19 999$. For the extreme positions of the servo, 1-2 ms is specified, which is 5-10% of the period. By using the OCR1A and OCR1B registers, we can set a specific duty cycle. If we use the inverted mode for PWM we can enter the values directly into the register and not have to subtract them from the maximum value. The calculation results in a control interval of approximately 1 000 - 2 000. Because of the telerance of the servo we had to find the correct limits for specific servos by trial and error. 

The servo determining the angle is set with the following values: $OCR1A = 1 500 - 4 700$

To set the tilt: $OCR1B = 900 - 2 000$.

#### Photoresistors


#### Current sensor
![image](https://github.com/user-attachments/assets/91da927b-1b6a-47b6-bb32-0f8962081bee)
*Current sensor, from[4]*
Z datasheetu [5], se můžeme dočíst, že senzor dává výstupní hodnoty jako napětí. Díky tomu můžeme využít funkce vytvořené pro čtení hodnot z fotorezistorů. Dále se můžeme dočíst, že pro proud $I = 0\,A$ se výstupní napětí senzoru rovná $U_{0A} = U_{cc}/2  = 2,5 \, V$. Senzor disponuje citlivostí ($185 \, mV/A$). Z těchto hodnot můžeme snadno vypočítat hodnoty výstupního napětí pro celý rozsah senzoru. 

$U_{OUTmax} = U_{0A} + (I \cdot sensitivity) = 2,5 + (5 \cdot 0,185) = 3,425 \,V$

Z této hodnoty a faktu, že pro $U = 5\, V$ má převodník hodnotu 1023, můžeme určit hodnotu A/D převodníku pro rozsah proudů:

$x = \frac{U_{OUTmax}}{U_{max}} \cdot MAX_{A/D} = \frac{3,425}{5} \cdot 1023 \approx 701$

Pro proud $I = 5\,A$ můžeme snadno odvodit hodnotu převodníku 562.

Pro výpis proudu můžeme použít rovnici:

$I = (-562 + \text{A/D Value}) \cdot \frac{1}{27,8}$

#### Display 
![image](https://github.com/user-attachments/assets/7bee33f4-e9fd-4415-8c4b-683bdc1ebba1)
*OLED display, from [6]*
Jako display je využíván OLED s rozlišením 168x64. Display komuniku je přes IIC.


### Used parts
 - Arduino NANO
 - 2x Servo motor SG90
 - OLED display (168x64, IIC)
 - 4x Photoresistor
 - Current sensor ACS712

### Sources
[1] https://www.studiopieters.nl/arduino-nano-pinout/
[2] http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf
[3] https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
[4] https://www.laskakit.cz/arduino-proudovy-senzor-5a-acs712/?utm_source=google&utm_medium=cpc&utm_campaign=1_PLA_All_ROAS_%5BCZ%5D_tROAS_570%2F350&utm_id=1371265813&gad_source=1&gclid=CjwKCAiAjeW6BhBAEiwAdKltMkxjIJc-CQQdpvqv7aDmpNT8D9rHq1pxshQiIphx7CQkUo7BzBrefRoCjq4QAvD_BwE
[5] https://www.laskakit.cz/user/related_files/acs712.pdf
[6] https://www.hwkitchen.cz/graficky-displej-oled-096-128x64-i2c-bily/?gad_source=1&gclid=CjwKCAiAjeW6BhBAEiwAdKltMsdv_tMKsyUl8U-bwB8IE8ftAJTWY9HhmumnMm_Rn9zQiKFi_XuZ2xoCNjgQAvD_BwE
