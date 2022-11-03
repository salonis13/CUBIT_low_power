/*
 * imu.c
 *
 *  Created on: Feb 23, 2022
 *      Author: rajat
 */


#include "i2c.h"
#include "imu.h"


#define SENSOR_IMU_ADDRESS 0x28 // IMU sensor adrress 0010 1001
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
#include "src/scheduler.h"
#include "src/timers.h"

extern uint8_t data_read_from_imu_sensor[2];

void imuSetOn()
{
  i2c_write_write(SENSOR_IMU_ADDRESS, IMU_PWR_MODE, 0x00);
}

void imuSetOff()
{
  i2c_write_write(SENSOR_IMU_ADDRESS, CMD_OPR_MODE, 0x00);
  timerWaitUs_polled(25000);
  i2c_write_write(SENSOR_IMU_ADDRESS, IMU_PWR_MODE, 0x02);
}

void init_imu()
{
  i2c_write_write(SENSOR_IMU_ADDRESS, CMD_OPR_MODE, 0x09);
}

void imu_read_angle()
{
  i2c_write_read(SENSOR_IMU_ADDRESS, EUL_DATA_Y_LSB);
}



void imu_init_state_machine(events_t evt)
{
  imu_state currentState;
  static imu_state nextState = state0_change_config_mode;
  sensor_data_t* sensor_data =  get_sensor_data();

  if(evt == evt_NoEvent)
    nextState = state0_change_config_mode;

  currentState = nextState;     //set current state of the process

  switch(currentState) {

    case state0_change_config_mode:
      nextState = state0_change_config_mode;          //default

      LOG_INFO("state0_change_config_mode");
      sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
      i2c_write_write(SENSOR_IMU_ADDRESS, CMD_OPR_MODE, 0x00);

      nextState = state0_start_imu;

      break;

    case state0_start_imu:
      nextState = state0_start_imu;          //default

      if(evt == evt_TransferDone) {
          LOG_INFO("state0_start_imu");
          imuSetOn();

          nextState = state1_init_imu;
      }
      if(evt == evt_retry){
          i2c_write_write(SENSOR_IMU_ADDRESS, CMD_OPR_MODE, 0x00);
            }

      break;

    case state1_init_imu:
      nextState = state1_init_imu;    //default

      if(evt == evt_TransferDone) {

          LOG_INFO("state1_init_imu");
          timerWaitUs_interrupt(50000);

          nextState = state2_timer_wait;
      }
      if(evt == evt_retry){
          imuSetOn();
      }

      break;

    case state2_timer_wait:
      nextState = state2_timer_wait;     //default

      if(evt == evt_COMP1) {
          LOG_INFO("state2_timer_wait");

          init_imu();

          nextState = state3_write_cmd;
      }

      break;

    case state3_write_cmd:
      nextState = state3_write_cmd;    //default

      if(evt == evt_TransferDone) {

          LOG_INFO("state3_write_cmd");
          //wait for 10ms for sensor to power on
          timerWaitUs_interrupt(10000);

          nextState = state4_timer_wait;
      }
      if(evt == evt_retry){
          init_imu();
      }

      break;

    case state4_timer_wait:
      nextState = state4_timer_wait;          //default

      if(evt == evt_COMP1) {

          LOG_INFO("state4_timer_wait");
          sensor_data->is_imu_initialization_complete = true;

          nextState = state0_start_imu;
      }

      break;

    case state5_do_nothing:
      if(evt == evt_start_next_imu_measurement)
        nextState = state5_do_nothing;
      break;

    default:

      LOG_ERROR("Should not be here in state machine\n\r");

      break;
  }

  return;
}

void imu_data_state_machine(events_t evt)
{
  imu_state currentState;
  static imu_state nextState = state0_i2c_read;
  sensor_data_t* sensor_data =  get_sensor_data();

  currentState = nextState;     //set current state of the process

  switch(currentState) {

    case state0_i2c_read:
      nextState = state0_i2c_read;          //default

      LOG_INFO("taking imu reading");
      imu_read_angle();

      nextState = state1_update_values;

      break;

    case state1_update_values:
      nextState = state1_update_values;    //default

      if(evt == evt_TransferDone) {

          LOG_INFO("saving imu reading");
          sensor_data->is_imu_reading_ready = true;
          sensor_data->refreshing_angular_reading = ((data_read_from_imu_sensor[1] << 8) | data_read_from_imu_sensor[0])/16;
          if(sensor_data->refreshing_angular_reading > 2048)
            sensor_data->refreshing_angular_reading = 4009 - sensor_data->refreshing_angular_reading;

          nextState = state0_i2c_read;
      }
      if(evt == evt_retry){
          imu_read_angle();
      }


      break;

    case state2_do_nothing:
      if(evt == evt_start_next_imu_measurement)
        nextState = state0_i2c_read;
      break;

    default:

      LOG_ERROR("Should not be here in state machine\n\r");

      break;
  }

  return;
}


void imu_turnoff_state_machine(events_t evt)
{
  imu_state currentState;
  static imu_state nextState = state0_off_change_config_mode;
  sensor_data_t* sensor_data =  get_sensor_data();

  if(evt == evt_NoEvent)
    {
      nextState = state0_off_change_config_mode;
    }

  currentState = nextState;     //set current state of the process

  switch(currentState) {

    case state0_off_change_config_mode:
      nextState = state0_off_change_config_mode;          //default

      LOG_INFO("Changing config mode to default");
      if(sensor_data->app_init == true) {
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
          i2c_write_write(SENSOR_IMU_ADDRESS, CMD_OPR_MODE, 0x00);
          sensor_data->app_init = false;
          nextState = state1_off_timer_wait;
      }

      else {
//      if(evt == evt_TransferDone)
        {

          //waiting for previous (measurement read)  transfer done to complete
          i2c_write_write(SENSOR_IMU_ADDRESS, CMD_OPR_MODE, 0x00);

          nextState = state1_off_timer_wait;
      }
      }


      break;

    case state1_off_timer_wait:
      nextState = state1_off_timer_wait;    //default

      if(evt == evt_TransferDone) {

          LOG_INFO("config mode changed, waiting for timer");
          timerWaitUs_interrupt(25000);

          nextState = state2_off_imu_suspend;
      }
      if(evt == evt_retry){

          i2c_write_write(SENSOR_IMU_ADDRESS, CMD_OPR_MODE, 0x00);
      }


      break;

    case state2_off_imu_suspend:
      nextState = state2_off_imu_suspend;    //default
      if(evt == evt_COMP1) {
          LOG_INFO("imu going in suspend mode");
        i2c_write_write(SENSOR_IMU_ADDRESS, IMU_PWR_MODE, 0x02);

      nextState = state3_off_timer_wait;
      }
      break;

    case state3_off_timer_wait:
      nextState = state3_off_timer_wait;    //default

      if(evt == evt_TransferDone) {

          LOG_INFO("state3_off_timer_wait");
          //wait for 10ms for sensor to power on
          timerWaitUs_interrupt(10000);

          nextState = state4_off_em2_mode;
      }
      if(evt == evt_retry){
//          timerWaitUs_interrupt(1000000);
          i2c_write_write(SENSOR_IMU_ADDRESS, IMU_PWR_MODE, 0x02);
//          nextState = state2_off_imu_suspend;
      }

      break;

    case state4_off_em2_mode:
      nextState = state4_off_em2_mode;    //default

          if(evt == evt_COMP1) {

              LOG_INFO("entering em2 mode");
                  sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
                  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM2);


          nextState = state0_off_change_config_mode;
          }

          break;

    default:

      LOG_ERROR("Should not be here in state machine\n\r");

      break;
  }

  return;
}
