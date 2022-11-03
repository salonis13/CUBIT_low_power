/*
   gpio.h

    Created on: Dec 12, 2018
        Author: Dan Walkes

    Updated by Dave Sluiter Sept 7, 2020. moved #defines from .c to .h file.
    Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include <stdbool.h>
#include "em_gpio.h"
#include <string.h>


// Student Edit: Define these, 0's are placeholder values.
// See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
// and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
// to determine the correct values for these.

#define	LED0_port  gpioPortD // change to correct ports and pins
#define LED0_pin   10
#define sensor_port gpioPortD
#define sensor_pin  15

#define PB0_port gpioPortF
#define PB0_pin  6
#define PB1_port gpioPortF
#define PB1_pin  7
//switches
#define SW_PORT   gpioPortA
#define SW_SELECT_PIN 3//0
#define SW_BACK_PIN 2//1
#define SW_DOWN_PIN 0//2
#define SW_UP_PIN 1//3

//Load switches
#define lcd_load_sw_port  gpioPortA
#define lcd_load_sw_pin   4

#define mag_enc_load_sw_port  gpioPortD
#define mag_enc_load_sw_pin   11
#define mag_enc_A_port  gpioPortD
#define mag_enc_A_pin   13
#define mag_enc_B_port  gpioPortD
#define mag_enc_B_pin   14

#define ultrasonic_load_sw_port gpioPortB
#define ultrasonic_load_sw_pin 11

#define extcom_port gpioPortB
#define extcom_pin 13



// Function prototypes
void gpioInit();
void gpioLed0SetOn();
void gpioLed0SetOff();
void gpioMagEncSetOn();
void gpioMagEncSetOff();
void gpioUltrasonicSetOn();
void gpioUltrasonicSetOff();
void gpioLcdSetOff();
void gpioLcdSetOn();
void enable_sensor();
void disable_sensor();
void gpioSetDisplayExtcomin(bool value);



#endif /* SRC_GPIO_H_ */
