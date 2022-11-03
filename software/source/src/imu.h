/*
 * imu.h
 *
 *  Created on: Feb 23, 2022
 *      Author: rajat
 */

#ifndef SRC_IMU_H_
#define SRC_IMU_H_

#include "scheduler.h"

#define CMD_CHIP_ID 0x00
#define CMD_OPR_MODE 0x3D
#define IMU_PWR_MODE 0x3E
#define MAG_DATA_Z_LSB 0x12
#define ACC_DATA_X_LSB 0x08
#define EUL_DATA_Z_LSB 0x1E //PITCH
#define EUL_DATA_Y_LSB 0x1C //ROLL
#define EUL_DATA_X_LSB 0x1A



//enum to define scheduler events
typedef enum uint32_t {
  state0_change_config_mode,
  state0_timer_wait_config,
  state0_start_imu,
  state1_init_imu,
  state2_timer_wait,
  state3_write_cmd,
  state4_timer_wait,
  state5_do_nothing,
  state0_i2c_read,
  state0_read_timer_wait,
  state1_update_values_0,
  state1_update_values,
  state2_do_nothing,
  state0_off_change_config_mode,
  state1_off_timer_wait,
  state2_off_imu_suspend,
  state3_off_timer_wait,
  state4_off_em2_mode,
  state1_got_services,
  state1_got_another_services,
  state2_got_char,
  state2_got_another_char,
  state3_set_indication,
  state4_wait_for_close,
  MY_NUM_STATES,
}imu_state;

void init_imu();

void imuSetOn();

void imuSetOff();

void imu_read_angle();

void imu_read_angle_MSB();

void imu_init_state_machine(events_t);

void imu_data_state_machine(events_t);  //(sl_bt_msg_t *evt);

void imu_turnoff_state_machine(events_t);

#endif /* SRC_IMU_H_ */
