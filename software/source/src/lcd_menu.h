/*
 * lcd_menu.h
 *
 *  Created on: Mar 31, 2022
 *      Author: rajat
 */

#ifndef SRC_LCD_MENU_H_
#define SRC_LCD_MENU_H_

#include "stdint.h"

typedef enum lcd_screens_e
{
  LCD_FLASH_SCREEN,
  LCD_MENU_LINEAR,
  LCD_MENU_ANGULAR,
  LCD_MENU_SETTINGS,
  LCD_LINEAR_STRING,
  LCD_LINEAR_WHEEL,
  LCD_LINEAR_SONIC,
  LCD_STRING_MEAS,
  LCD_WHEEL_MEAS,
  LCD_SONIC_MEAS,
  LCD_ANGULAR_MEAS,
  LCD_SETTINGS_UNIT,
  LCD_SETTINGS_CHARGE_STATUS,
  LCD_UNIT_CM,
  LCD_UNIT_INCH,
  LCD_CHARGE_STATUS_ON,
  LCD_CHARGE_STATUS_OFF
}lcd_screens_t;


void handle_lcd_menus(uint32_t event);
void lcd_auto_update_display(void);


#endif /* SRC_LCD_MENU_H_ */
