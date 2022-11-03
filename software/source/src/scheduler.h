/*
 * scheduler.h
 *
 *  Created on: Sep 15, 2021
 *      Author: salon
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include "src/timers.h"
#include "src/oscillators.h"
#include "src/gpio.h"
#include "src/ble.h"
#include "app.h"

#include "em_letimer.h"
#include "main.h"


typedef enum rotation_direction_e{
  NONE,
  CLK_WISE,
  COUNTER_CLK_WISE
}rotation_direction_t;


typedef enum last_pulse_e{
  NOT_SET,
  CHNL_A,
  CHNL_B
}last_pulse_t;


typedef struct mag_enc_s
{
  uint64_t clkwise_counter;
  uint64_t counter_clkwise_counter;
  rotation_direction_t rotation_direction;
  last_pulse_t last_pulse;
}mag_enc_t;


//enum for interrupt based events
typedef enum events_e{
  evt_NoEvent=1,
  evt_TimerUF = 2,
  evt_COMP1 = 4,
  evt_TransferDone = 8,
  evt_retry = 16,
  evt_Button_UP = 32,
  evt_Button_BACK = 64,
  evt_Button_SELECT = 128,
  evt_Button_DOWN = 256,
  evt_start_next_imu_measurement = 512,
  LAST_EVENT_IN_THE_LIST = 1024
}events_t;

typedef enum auto_update_display_e{
  MAG_ENC_CM,
  MAG_ENC_INCH,
  ULTRASONIC_CM,
  ULTRASONIC_INCH,
  ANGULAR,
  NO_AUTO_UPDATE_DISPLAY
}auto_update_display_t;

enum unit{
  CM,
  INCH
};

typedef struct sensor_data_s{
  bool app_init;
  bool is_mag_enc_enabled;
  bool is_ultrasonic_enabled;
  bool is_angular_enabled;
  bool is_imu_initialization_complete;
  bool is_imu_reading_ready;
  bool is_mag_enc_string;
  uint64_t refreshing_mag_enc_reading;
  int refreshing_ultrasonic_reading;
  int refreshing_angular_reading;
  int mag_enc_reading_to_be_saved;
  int ultrasonic_reading_to_be_saved;
  int angular_reading_to_be_saved;
  int settings_unit;
}sensor_data_t;

sensor_data_t* get_sensor_data(void);

//function to set a scheduler event
void schedulerSetEventUF();

void schedulerSetEventCOMP1();

void schedulerSetEventTransferDone();
void schedulerSetEventRetryTransfer();
void schedulerSetEventStartNextImuReading();

void schedulerSetEventButtonUp();
void schedulerSetEventButtonBack();
void schedulerSetEventButtonSelect();
void schedulerSetEventButtonDown();


//function to get scheduler event
uint32_t getNextEvent();

//state machine function
void temperature_state_machine(sl_bt_msg_t *evt);

void discovery_state_machine(sl_bt_msg_t *evt);


#endif /* SRC_SCHEDULER_H_ */
