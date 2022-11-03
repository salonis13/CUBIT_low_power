/*
  gpio.c

   Created on: Dec 12, 2018
       Author: Dan Walkes
   Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

   March 17
   Dave Sluiter: Use this file to define functions that set up or control GPIOs.

 */




#include "gpio.h"
#include "../app.h"
#include "main.h"





// Set GPIO drive strengths and modes of operation
void gpioInit()
{


//  //GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
//  GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);


//  GPIO_DriveStrengthSet(sensor_port, gpioDriveStrengthWeakAlternateWeak);
//  GPIO_PinModeSet(sensor_port, sensor_pin, gpioModePushPull, false);

  GPIO_DriveStrengthSet(lcd_load_sw_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(lcd_load_sw_port, lcd_load_sw_pin, gpioModePushPull, false);

  GPIO_DriveStrengthSet(mag_enc_load_sw_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(mag_enc_load_sw_port, mag_enc_load_sw_pin, gpioModePushPull, false);

  GPIO_DriveStrengthSet(ultrasonic_load_sw_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(ultrasonic_load_sw_port, ultrasonic_load_sw_pin, gpioModePushPull, false);

//  GPIO_DriveStrengthSet(extcom_port, gpioDriveStrengthWeakAlternateWeak);
//  GPIO_PinModeSet(extcom_port, extcom_pin, gpioModePushPull, false);

//  GPIO_PinModeSet(PB0_port, PB0_pin, gpioModeInputPullFilter, true);
//  GPIO_ExtIntConfig(PB0_port, PB0_pin, PB0_pin, true, true, true);

//  GPIO_PinModeSet(PB1_port, PB1_pin, gpioModeInputPullFilter, true);
//  GPIO_ExtIntConfig(PB1_port, PB1_pin, PB1_pin, true, true, true);

//  GPIO_PinModeSet(IMU_INTRPT_port, IMU_INTRPT_pin, gpioModeInputPullFilter, true);  //DOUT : true means pull up
//  GPIO_ExtIntConfig (IMU_INTRPT_port, IMU_INTRPT_pin, IMU_INTRPT_pin, false, true, true);  //enable at falling edge



  //HMI switches pin configuration
  GPIO_PinModeSet(SW_PORT, SW_SELECT_PIN, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(SW_PORT, SW_SELECT_PIN, SW_SELECT_PIN, true, false, true);

  GPIO_PinModeSet(SW_PORT, SW_BACK_PIN, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(SW_PORT, SW_BACK_PIN, SW_BACK_PIN, true, false, true);

  GPIO_PinModeSet(SW_PORT, SW_DOWN_PIN, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(SW_PORT, SW_DOWN_PIN, SW_DOWN_PIN, true, false, true);

  GPIO_PinModeSet(SW_PORT, SW_UP_PIN, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(SW_PORT, SW_UP_PIN, SW_UP_PIN, true, false, true);

  GPIO_IntClear(0xFFFFFFFF);
} // gpioInit()


void gpioMagEncSetOff()
{
  GPIO_PinOutSet(mag_enc_load_sw_port,mag_enc_load_sw_pin);

  //turning GPIOs off to save on power
//  GPIO_DriveStrengthSet(mag_enc_A_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(mag_enc_A_port, mag_enc_A_pin, gpioModeDisabled, false);
//  GPIO_PinOutSet(mag_enc_A_port,mag_enc_A_pin);

//  GPIO_DriveStrengthSet(mag_enc_B_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(mag_enc_B_port, mag_enc_B_pin, gpioModeDisabled, false);
//  GPIO_PinOutSet(mag_enc_B_port,mag_enc_B_pin);
}


void gpioMagEncSetOn()
{
  GPIO_PinOutClear(mag_enc_load_sw_port,mag_enc_load_sw_pin);

  GPIO_PinModeSet(mag_enc_A_port, mag_enc_A_pin, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(mag_enc_A_port, mag_enc_A_pin, mag_enc_A_pin, true, true, true);

  GPIO_PinModeSet(mag_enc_B_port, mag_enc_B_pin, gpioModeInputPullFilter, true);
  GPIO_ExtIntConfig(mag_enc_B_port, mag_enc_B_pin, mag_enc_B_pin, true, true, true);
}



void gpioUltrasonicSetOff()
{
  GPIO_PinOutSet(ultrasonic_load_sw_port,ultrasonic_load_sw_pin);


}


void gpioUltrasonicSetOn()
{
  GPIO_PinOutClear(ultrasonic_load_sw_port,ultrasonic_load_sw_pin);
}


void gpioLcdSetOff()
{
  GPIO_PinOutSet(lcd_load_sw_port,lcd_load_sw_pin);
}


void gpioLcdSetOn()
{
  GPIO_PinOutClear(lcd_load_sw_port,lcd_load_sw_pin);
}




void gpioLed0SetOn()
{
  GPIO_PinOutSet(LED0_port,LED0_pin);
}


void gpioLed0SetOff()
{
  GPIO_PinOutClear(LED0_port,LED0_pin);
}

void enable_sensor() {
  //GPIO_PinOutSet(sensor_port, sensor_pin);
  GPIO_PinOutClear(sensor_port, sensor_pin);
}

void disable_sensor() {
  GPIO_PinOutClear(sensor_port, sensor_pin);
}

void gpioSetDisplayExtcomin(bool value) {

//  GPIO_PinOutClear(extcom_port, extcom_pin);
//  if(value == true) {
//      GPIO_PinOutSet(lcd_port, lcd_pin);
//  }
//  else {
//      GPIO_PinOutClear(lcd_port, lcd_pin);
//  }
}




