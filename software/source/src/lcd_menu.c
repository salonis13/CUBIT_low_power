/*
 * lcd_menu.c
 *
 *  Created on: Mar 31, 2022
 *      Author: rajat
 */

#include "src/lcd_menu.h"
#include "src/imu.h"
#include "stdint.h"
#include <stdio.h>
//#include <string.h>
#include "scheduler.h"
#include "lcd_bitmap.h"
#include "ultrasonic.h"
#include "ble.h"

extern mag_enc_t mag_enc;
uint32_t curr_lcd_screen = 0;


void handle_lcd_menus(uint32_t event)
{
#if 1
  //check if event is from switches
  //or from LCD timeout
  static uint32_t next_lcd_screen = 0;
  static uint32_t previous_lcd_screen = LCD_FLASH_SCREEN;
  sensor_data_t* sensor_data =  get_sensor_data();

  switch(previous_lcd_screen)
  {
    case LCD_FLASH_SCREEN:

      if((event == evt_Button_UP) ||
          (event == evt_Button_BACK)||
          (event == evt_Button_SELECT)||
          (event == evt_Button_DOWN))
      {
        next_lcd_screen = LCD_MENU_LINEAR;
        draw_custom_graphics(menu, RECTANGLE1);
      }

    break;
    
    case LCD_MENU_LINEAR:
      if(event == evt_Button_UP)
      {
        draw_custom_graphics(menu, RECTANGLE3);
        next_lcd_screen = LCD_MENU_SETTINGS;
      }
      else if(event == evt_Button_BACK)
      {
        //No action
      }
      else if(event == evt_Button_SELECT)
      {
        draw_custom_graphics(linear, RECTANGLE1);

        next_lcd_screen = LCD_LINEAR_STRING;
      }
      else if(event == evt_Button_DOWN)
      {
        draw_custom_graphics(menu, RECTANGLE2);
        next_lcd_screen = LCD_MENU_ANGULAR;
      }

    break;

    case LCD_LINEAR_STRING:
      if(event == evt_Button_UP)
      {
        draw_custom_graphics(linear, RECTANGLE3);
        next_lcd_screen = LCD_LINEAR_SONIC;
      }
      else if(event == evt_Button_BACK)
      {
        draw_custom_graphics(menu, RECTANGLE1);
        next_lcd_screen = LCD_MENU_LINEAR;
      }
      else if(event == evt_Button_SELECT)
      {
          /*code for enabling magnetic encoder and enable auto_updating_display*/
        //Turn magnetic encoder ON
        gpioMagEncSetOn();
        sensor_data->is_mag_enc_string = true;
        sensor_data->is_mag_enc_enabled = true;
        sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
        //set flag that would keep updating measured values using soft_timer
        //if setting is to use cm as unit
        draw_custom_graphics(string, NO_RECTANGLE);
        //Add CM or INCH
        next_lcd_screen = LCD_STRING_MEAS;
        //if setting is to use cm as unit
        // draw_custom_graphics(string_meas_inch);
        if(sensor_data->settings_unit==CM)
            displayUnit(DISPLAY_ROW_BTADDR, "cm");
        else if(sensor_data->settings_unit==INCH)
            displayUnit(DISPLAY_ROW_BTADDR, "inch");
      }
      else if(event == evt_Button_DOWN)
      {
        draw_custom_graphics(linear, RECTANGLE2);
//        EMSTATUS GLIB_drawRect(GLIB_Context_t *pContext, const GLIB_Rectangle_t *pRect);
        next_lcd_screen = LCD_LINEAR_WHEEL;
      }
    break;

    case LCD_STRING_MEAS:
      if(event == evt_Button_UP)
      {
        //No action
      }
      else if(event == evt_Button_BACK)
      {
        gpioMagEncSetOff();
        sensor_data->is_mag_enc_enabled = false;  // move this to gpioMagEncSetOff()
        draw_custom_graphics(linear, RECTANGLE1);
        sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
        sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM2);
        next_lcd_screen = LCD_LINEAR_STRING;
      }
      else if(event == evt_Button_SELECT)
      {
//          gpioMagEncSetOff();
          //saving parameter in the database. To be sent over bluetooth
          sensor_data->mag_enc_reading_to_be_saved = sensor_data->refreshing_mag_enc_reading;
          ble_SendMeasurement(LCD_STRING_MEAS, (float)((float)sensor_data->mag_enc_reading_to_be_saved / 100.0));
          //ble_SendTemp((float)((float)sensor_data->mag_enc_reading_to_be_saved / 100.0));
//          sensor_data->is_mag_enc_enabled = false;
        //No action
      }
      else if(event == evt_Button_DOWN)
      {
        //No action
      }
    break;
    

    case LCD_LINEAR_WHEEL:
      if(event == evt_Button_UP)
      {
        draw_custom_graphics(linear, RECTANGLE1);
        next_lcd_screen = LCD_LINEAR_STRING;
      }
      else if(event == evt_Button_BACK)
      {
        draw_custom_graphics(menu, RECTANGLE1);
        next_lcd_screen = LCD_MENU_LINEAR;
      }
      else if(event == evt_Button_SELECT)
      {
          /*code for enabling magnetic encoder and enable auto_updating_display*/
        //Turn magnetic encoder ON
        gpioMagEncSetOn();
        sensor_data->is_mag_enc_string = false;
        sensor_data->is_mag_enc_enabled = true;
        sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
        //set flag that would keep updating measured values using soft_timer

        //if unit is set as cm
        draw_custom_graphics(wheel, NO_RECTANGLE);
        //Add CM or INCH text
        next_lcd_screen = LCD_WHEEL_MEAS;
        curr_lcd_screen = LCD_WHEEL_MEAS;
        //if unit is set as inch
        //draw_custom_graphics(wheel_meas_inch);
        if(sensor_data->settings_unit==CM)
            displayUnit(DISPLAY_ROW_BTADDR, "cm");
        else if(sensor_data->settings_unit==INCH)
            displayUnit(DISPLAY_ROW_BTADDR, "inch");
      }
      else if(event == evt_Button_DOWN)
      {
        draw_custom_graphics(linear, RECTANGLE3);
        next_lcd_screen = LCD_LINEAR_SONIC;
      }
    break;

    case LCD_WHEEL_MEAS:
      if(event == evt_Button_UP)
      {
        //No action
      }
      else if(event == evt_Button_BACK)
      {
        gpioMagEncSetOff();
        sensor_data->is_mag_enc_enabled = false;
        draw_custom_graphics(linear, RECTANGLE2);
        sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
        sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM2);
        next_lcd_screen = LCD_LINEAR_WHEEL;
        curr_lcd_screen = 0;
      }
      else if(event == evt_Button_SELECT)
      {
          gpioMagEncSetOff();
          //saving parameter in the database. To be sent over bluetooth
          sensor_data->mag_enc_reading_to_be_saved = sensor_data->refreshing_mag_enc_reading;
          sensor_data->is_mag_enc_enabled = false;
          ble_SendMeasurement(LCD_WHEEL_MEAS, (float)((float)sensor_data->mag_enc_reading_to_be_saved / 100.0));
          curr_lcd_screen = 0;
        //No action
      }
      else if(event == evt_Button_DOWN)
      {
        //No action
      }
    break;


    case LCD_LINEAR_SONIC: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_LINEAR_WHEEL;
        draw_custom_graphics(linear, RECTANGLE2);
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_MENU_LINEAR;
        draw_custom_graphics(menu, RECTANGLE1);
      }
      else if(event == evt_Button_SELECT)
      {
          gpioUltrasonicSetOn();
          sensor_data->is_ultrasonic_enabled = true;
        //if unit is set as cm
        next_lcd_screen = LCD_SONIC_MEAS;
        //Add CM or INCH
        draw_custom_graphics(sonic, NO_RECTANGLE);
        if(sensor_data->settings_unit==CM)
            displayUnit(DISPLAY_ROW_BTADDR, "cm");
        else if(sensor_data->settings_unit==INCH)
            displayUnit(DISPLAY_ROW_BTADDR, "inch");

        //if unit is set as inch
        // draw_custom_graphics(sonic_meas_inch);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_LINEAR_STRING;
        draw_custom_graphics(linear, RECTANGLE1);
      }
    break;

    case LCD_SONIC_MEAS:
      if(event == evt_Button_UP)
      {
        //No action
      }
      else if(event == evt_Button_BACK)
      {
        gpioUltrasonicSetOff();
        sensor_data->is_ultrasonic_enabled = false;
        clear_display();
        //add delay to make sure display has been updated
        next_lcd_screen = LCD_LINEAR_SONIC;
        draw_custom_graphics(linear, RECTANGLE3);
      }
      else if(event == evt_Button_SELECT)
      {
          sensor_data->ultrasonic_reading_to_be_saved = sensor_data->refreshing_ultrasonic_reading;
          ble_SendMeasurement(LCD_SONIC_MEAS, (float)((float)sensor_data->ultrasonic_reading_to_be_saved));
        //No action
      }
      else if(event == evt_Button_DOWN)
      {
        //No action
      }
    break;
    

    case LCD_MENU_ANGULAR: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_MENU_LINEAR;
        draw_custom_graphics(menu, RECTANGLE1);
      }
      else if(event == evt_Button_BACK)
      {
        //No action
      }
      else if(event == evt_Button_SELECT)
      {
        sensor_data->is_angular_enabled = true;



        //if unit is set as cm

        next_lcd_screen = LCD_ANGULAR_MEAS;
        draw_custom_graphics(angular, NO_RECTANGLE);
        imu_init_state_machine(evt_NoEvent);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_MENU_SETTINGS;
        draw_custom_graphics(menu, RECTANGLE3);
      }
    break;

    case LCD_ANGULAR_MEAS: 
      if(event == evt_Button_UP)
      {
        //No action
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_MENU_ANGULAR;
        sensor_data->is_angular_enabled = false;
        sensor_data->is_imu_initialization_complete = false;
        sensor_data->is_imu_reading_ready = false;
//        timerWaitUs_polled(50000);
//        sensor_data->should_refresh_imu_data = false;
        //enter IMU low power
//        imuSetOff();
        imu_turnoff_state_machine(evt_NoEvent);
        draw_custom_graphics(menu, RECTANGLE2);
      }
      else if(event == evt_Button_SELECT)
      {
          sensor_data->angular_reading_to_be_saved = sensor_data->refreshing_angular_reading;
          ble_SendMeasurement(LCD_ANGULAR_MEAS, (float)((float)abs(sensor_data->angular_reading_to_be_saved)));
        //No action
      }
      else if(event == evt_Button_DOWN)
      {
        //No action
      }
    break;

    case LCD_MENU_SETTINGS: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_MENU_ANGULAR;
        draw_custom_graphics(menu, RECTANGLE2);

      }
      else if(event == evt_Button_BACK)
      {
        //No action
      }
      else if(event == evt_Button_SELECT)
      {
        next_lcd_screen = LCD_SETTINGS_UNIT;
        draw_custom_graphics(settings, RECTANGLE1);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_MENU_LINEAR;
        draw_custom_graphics(menu, RECTANGLE1);
      }
    break;

    case LCD_SETTINGS_UNIT: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_SETTINGS_CHARGE_STATUS;
        draw_custom_graphics(settings, RECTANGLE2);
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_MENU_SETTINGS;
        draw_custom_graphics(menu, RECTANGLE3);
      }
      else if(event == evt_Button_SELECT)
      {
        next_lcd_screen = LCD_UNIT_CM;
        draw_custom_graphics(unit, RECTANGLE1);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_SETTINGS_CHARGE_STATUS;
        draw_custom_graphics(settings, RECTANGLE2);
      }
    break;

    case LCD_UNIT_CM: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_UNIT_INCH;
        draw_custom_graphics(unit, RECTANGLE2);
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_SETTINGS_UNIT;
        draw_custom_graphics(settings, RECTANGLE1);
      }
      else if(event == evt_Button_SELECT)
      {
        sensor_data->settings_unit = CM; //change this with enum
        ble_SendMeasurement(LCD_SETTINGS_UNIT, (float)sensor_data->settings_unit);
        next_lcd_screen = LCD_SETTINGS_UNIT;
        //set the unit to cm
        draw_custom_graphics(settings, RECTANGLE1);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_UNIT_INCH;
        draw_custom_graphics(unit, RECTANGLE2);
      }
    break;

    case LCD_UNIT_INCH: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_UNIT_CM;
        draw_custom_graphics(unit, RECTANGLE1);
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_SETTINGS_UNIT;
        draw_custom_graphics(settings, RECTANGLE1);
      }
      else if(event == evt_Button_SELECT)
      {
        sensor_data->settings_unit = INCH; //change this with enum
        ble_SendMeasurement(LCD_SETTINGS_UNIT, (float)sensor_data->settings_unit);
        next_lcd_screen = LCD_SETTINGS_UNIT;
        draw_custom_graphics(settings, RECTANGLE1);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_UNIT_CM;
        draw_custom_graphics(unit, RECTANGLE1);
      }
    break;

    case LCD_SETTINGS_CHARGE_STATUS: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_SETTINGS_UNIT;
        draw_custom_graphics(settings, RECTANGLE1);
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_MENU_SETTINGS;
        draw_custom_graphics(menu, RECTANGLE3);
      }
      else if(event == evt_Button_SELECT)
      {
        next_lcd_screen = LCD_CHARGE_STATUS_ON;
        draw_custom_graphics(charge_status, RECTANGLE2);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_SETTINGS_UNIT;
        draw_custom_graphics(settings, RECTANGLE1);
      }
    break;

    case LCD_CHARGE_STATUS_ON: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_CHARGE_STATUS_OFF;
        draw_custom_graphics(charge_status, RECTANGLE3);
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_SETTINGS_CHARGE_STATUS;
        draw_custom_graphics(settings, RECTANGLE2);
      }
      else if(event == evt_Button_SELECT)
      {
        next_lcd_screen = LCD_SETTINGS_CHARGE_STATUS;
        //set LCD On while charging
        draw_custom_graphics(settings, RECTANGLE2);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_CHARGE_STATUS_OFF;
        draw_custom_graphics(charge_status, RECTANGLE3);
      }
    break;

    case LCD_CHARGE_STATUS_OFF: 
      if(event == evt_Button_UP)
      {
        next_lcd_screen = LCD_CHARGE_STATUS_ON;
        draw_custom_graphics(charge_status, RECTANGLE2);
      }
      else if(event == evt_Button_BACK)
      {
        next_lcd_screen = LCD_SETTINGS_CHARGE_STATUS;
        draw_custom_graphics(settings, RECTANGLE2);
      }
      else if(event == evt_Button_SELECT)
      {
        next_lcd_screen = LCD_SETTINGS_CHARGE_STATUS;
        //set LCD Off while charging
        draw_custom_graphics(settings, RECTANGLE2);
      }
      else if(event == evt_Button_DOWN)
      {
        next_lcd_screen = LCD_CHARGE_STATUS_ON;
        draw_custom_graphics(charge_status, RECTANGLE2);
      }
    break;
  }

  previous_lcd_screen = next_lcd_screen;
  
  displayUpdate();

#endif
}



/********************************************************************************
 *
 *******************************************************************************/

void lcd_auto_update_display()
{
  sensor_data_t* sensor_data =  get_sensor_data();
  char refreshing_reading_str[5] = "";


  if(sensor_data->is_mag_enc_enabled == true)
    {
      if(sensor_data->is_mag_enc_string)
        {
          if(sensor_data->settings_unit == CM)
            sensor_data->refreshing_mag_enc_reading = (abs(mag_enc.clkwise_counter - mag_enc.counter_clkwise_counter)*8.38/10); //1 pulse corresponds to 0.168mm (1024 pulses correspond to 2*pi*37mm... multiplied by 100 to improve resolution
          else if((sensor_data->settings_unit == INCH))
            sensor_data->refreshing_mag_enc_reading = (abs(mag_enc.clkwise_counter - mag_enc.counter_clkwise_counter)*8.38*0.3937/10); //1 pulse corresponds to 0.168mm (1024 pulses correspond to 2*pi*37mm... multiplied by 100 to improve resolution
        }
      else
        {
          if(sensor_data->settings_unit == CM)
            sensor_data->refreshing_mag_enc_reading = (abs(mag_enc.clkwise_counter - mag_enc.counter_clkwise_counter)*21.9/10); //1 pulse corresponds to 0.219mm (1024 pulses correspond to 2*pi*35mm... multiplied by 100 to improve resolution
          else if((sensor_data->settings_unit == INCH))
            sensor_data->refreshing_mag_enc_reading = (abs(mag_enc.clkwise_counter - mag_enc.counter_clkwise_counter)*21.9*0.3937/10); //1 pulse corresponds to 0.219mm (1024 pulses correspond to 2*pi*35mm... multiplied by 100 to improve resolution
        }

      if(sensor_data->refreshing_mag_enc_reading < 100)
        sensor_data->refreshing_mag_enc_reading = 0;

      sprintf(refreshing_reading_str,"%3d.%2d",(int)(sensor_data->refreshing_mag_enc_reading/100), ((int)(sensor_data->refreshing_mag_enc_reading) % 100));
      displayPrintf(DISPLAY_ROW_CLIENTADDR, (refreshing_reading_str));
//      displayPrintf(DISPLAY_ROW_CLIENTADDR, "%f", b);//sensor_data->refreshing_mag_enc_reading/100);
      //write mag_enc numbers on the LCD
    }
  else if(sensor_data->is_ultrasonic_enabled == true)
    {
      //ultrasonic_readings
      for(int i=0; i< 15; i++)  // processing entire circular buffer
       {
          sensor_data->refreshing_ultrasonic_reading = get_processed_data_from_ultrasonic_sensor();
          if(sensor_data->refreshing_ultrasonic_reading != 0)
            {
              if((sensor_data->settings_unit == CM))  //checking only if settings is in CM. By default reading is received is in INCH
                sensor_data->refreshing_ultrasonic_reading = (int)(sensor_data->refreshing_ultrasonic_reading*2.54);
              break;
            }
       }
      if((sensor_data->refreshing_ultrasonic_reading != 0) && (sensor_data->refreshing_ultrasonic_reading != 255))
      {

              if((sensor_data->refreshing_ultrasonic_reading > 6) && (sensor_data->settings_unit == INCH))
                {
                  sprintf(refreshing_reading_str,"%3d",(int)(sensor_data->refreshing_ultrasonic_reading));
                  displayPrintf(DISPLAY_ROW_CLIENTADDR, (refreshing_reading_str));
                }
              else if((sensor_data->refreshing_ultrasonic_reading > 15) && (sensor_data->settings_unit == CM))
                {
                  sprintf(refreshing_reading_str,"%3d",(int)(sensor_data->refreshing_ultrasonic_reading));
                  displayPrintf(DISPLAY_ROW_CLIENTADDR, (refreshing_reading_str));
                }
              else
                {
                  displayPrintf(DISPLAY_ROW_CLIENTADDR, "...");
                }

      }
      //write ultrasonic numbers on the LCD
    }

  else if(sensor_data->is_angular_enabled == true)
    {
      if(sensor_data->is_imu_initialization_complete == true)
        {
          if(sensor_data->is_imu_reading_ready == true)
            {
              sprintf(refreshing_reading_str,"%3d",(int)(sensor_data->refreshing_angular_reading));
              displayPrintf(DISPLAY_ROW_CLIENTADDR, (refreshing_reading_str));
              sensor_data->is_imu_reading_ready = false;
              schedulerSetEventStartNextImuReading();

            }
        }

//      sensor_data->should_refresh_imu_data = true;
//      imu_data_state_machine(state0_i2c_read);//write BNO numbers on the LCD

    }
}

