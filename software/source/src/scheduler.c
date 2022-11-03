/*
 * scheduler.c
 *
 *  Created on: Sep 15, 2021
 *      Author: salon
 */

// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"

#include "app.h"
#include "stdint.h"
#include "src/scheduler.h"
#include "src/ultrasonic.h"
#include "src/lcd_menu.h"

//uint32_t MyEvent;
static uint32_t event_status  = 0x00000000;

extern uint32_t encoder_pulses_A;

extern mag_enc_t mag_enc;
extern mode;

sl_status_t rc=0;

#if !DEVICE_IS_BLE_SERVER
// Health Thermometer service UUID defined by Bluetooth SIG
static const uint8_t thermo_service[2] = { 0x09, 0x18 };
// Temperature Measurement characteristic UUID defined by Bluetooth SIG
static const uint8_t thermo_char[2] = { 0x1c, 0x2a };

// button state service UUID defined by Bluetooth SIG
// 00000001-38c8-433e-87ec-652a2d136289
static const uint8_t button_service[16] = { 0x89, 0x62, 0x13, 0x2d, 0x2a, 0x65, 0xec, 0x87, 0x3e, 0x43, 0xc8, 0x38, 0x01, 0x00, 0x00, 0x00 };
// button state characteristic UUID defined by Bluetooth SIG
// 00000002-38c8-433e-87ec-652a2d136289
static const uint8_t button_char[16] = { 0x89, 0x62, 0x13, 0x2d, 0x2a, 0x65, 0xec, 0x87, 0x3e, 0x43, 0xc8, 0x38, 0x02, 0x00, 0x00, 0x00 };


#endif



sensor_data_t sensor_data = {false, false, false, false, false, false, false, 0, 0, 0, 0, 0, 0, 0};


sensor_data_t* get_sensor_data()
{
  return &sensor_data;
}

extern uint32_t letimer_uf_count;
// scheduler routine to set a scheduler event
void schedulerSetEventUF() {

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_TimerUF);

  // set the event in your data structure, this is a read-modify-write
  //MyEvent |= evt_TimerUF;

  // exit critical section
  CORE_EXIT_CRITICAL();

//  int dist=0;
//  while(dist == 0)
//  {dist = get_processed_data_from_ultrasonic_sensor();}
//  //if(distance != 0)
//    {
//      //log
//      LOG_INFO("Dist = %d\n\r", dist);
//
////      distance = 100;
//    }

//  displayPrintf(DISPLAY_ROW_NAME, "Server");
//
//  if(mode == 0) //IMU
//    {
//      //read imu data and print it
//      //LOG_INFO("printing IMU data\n\r");
//      //imu_read_angle();
//      displayPrintf(DISPLAY_ROW_NAME, "Server");
////      displayUpdate();
//
//    }
//  else if(mode == 1) //Rangefinder
//    {
//      //read rangefinder data and print it
//      LOG_INFO("printing RangeFinder data\n\r");
////      displayPrintf(DISPLAY_ROW_NAME, "Server");
////      displayUpdate();
//
//    }
//  else if(mode == 2) //magnetic encoder
//    {
//      LOG_INFO("Count=%d\n\r", encoder_pulses_A);
////      displayPrintf(DISPLAY_ROW_NAME, "Server");
////      displayUpdate();
//
////      displayPrintf(DISPLAY_ROW_NAME, "Server");
//    }


} // schedulerSetEventXXX()


void schedulerSetEventCOMP1()
{
  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  //sl_bt_external_signal(evt_COMP1);

  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_COMP1;

  // exit critical section
  CORE_EXIT_CRITICAL();
}

// scheduler routine to set a scheduler event
void schedulerSetEventButtonUp() {
  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_Button_UP);
//  handle_lcd_menus(evt_Button_UP);
  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_Button_UP;

  // exit critical section
  CORE_EXIT_CRITICAL();

} // schedulerSetEventXXX()

// scheduler routine to set a scheduler event
void schedulerSetEventButtonBack() {

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_Button_BACK);
//  handle_lcd_menus(evt_Button_BACK);
  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_Button_BACK;

  // exit critical section
  CORE_EXIT_CRITICAL();

} // schedulerSetEventXXX()

// scheduler routine to set a scheduler event
void schedulerSetEventButtonSelect() {

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_Button_SELECT);
//  handle_lcd_menus(evt_Button_SELECT);

//  if(mag_enc.clkwise_counter > mag_enc.counter_clkwise_counter)
//    LOG_INFO("Direction=clkwise\n\r");
//  else
//    LOG_INFO("Direction=counter_clkwise\n\r");
  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_Button_SELECT;

  // exit critical section
  CORE_EXIT_CRITICAL();

} // schedulerSetEventXXX()

//void schedulerTempFunction() {
//  //This function to be moved to getNextEvent
//
//  uint64_t value;
//  value = (abs(mag_enc.clkwise_counter - mag_enc.counter_clkwise_counter)*22.7/10); //1 pulse corresponds to 0.227mm (1024 pulses correspond to 2*pi*37mm... multiplied by 100 to improve resolution
//  //  printf("Counter=%d.%d cm\t\t", (uint32_t)value/100, (uint32_t)value%100);
//
//
//}

// scheduler routine to set a scheduler event
void schedulerSetEventButtonDown() {

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_Button_DOWN);
//  handle_lcd_menus(evt_Button_DOWN);
  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_Button_DOWN;

  // exit critical section
  CORE_EXIT_CRITICAL();

} // schedulerSetEventXXX()

// scheduler routine to set a scheduler event
void schedulerSetEventTransferDone() {

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_TransferDone);
  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_TransferDone;

  // exit critical section
  CORE_EXIT_CRITICAL();

} // schedulerSetEventXXX()


// scheduler routine to set a scheduler event
void schedulerSetEventRetryTransfer() {

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_TransferDone);
  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_retry;

  // exit critical section
  CORE_EXIT_CRITICAL();

} // schedulerSetEventXXX()


// scheduler routine to set a scheduler event
void schedulerSetEventStartNextImuReading() {

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

//  sl_bt_external_signal(evt_start_next_imu_measurement);
  // set the event in your data structure, this is a read-modify-write
  event_status |= evt_start_next_imu_measurement;

  // exit critical section
  CORE_EXIT_CRITICAL();

} // schedulerSetEventXXX()



//// scheduler routine to return 1 event to main()code and clear that event
//uint32_t getNextEvent() {
//
//  static uint32_t theEvent=evt_NoEvent;
//
//  //determine 1 event to return to main() code, apply priorities etc.
//  // clear the event in your data structure, this is a read-modify-write
//  // enter critical section
//  CORE_DECLARE_IRQ_STATE;
//  CORE_ENTER_CRITICAL();
//
//  if(MyEvent & evt_TimerUF) {
//      theEvent = evt_TimerUF;
//      MyEvent ^= evt_TimerUF;
//  }
//  else if(MyEvent & evt_COMP1) {
//      theEvent = evt_COMP1;
//      MyEvent ^= evt_COMP1;
//  }
//  else if(MyEvent & evt_TransferDone) {
//      theEvent = evt_TransferDone;
//      MyEvent ^= evt_TransferDone;
//  }
//
//  // exit critical section
//  CORE_EXIT_CRITICAL();
//
//  return (theEvent);
//} // getNextEvent()



/** -------------------------------------------------------------------------------------------
 * @brief scheduler routine to return 1 event to main()
 *
 * @param None
 * @return theEvent : returns status of events
 *-------------------------------------------------------------------------------------------- **/
uint32_t getNextEvent()
{
  uint32_t theEvent;
  // determine 1 event to return to main()
  for(uint32_t i=1; i<=LAST_EVENT_IN_THE_LIST; i<<=1)
  {
    if(i & event_status)
    {
      theEvent = i;
      break;
    }
  }

  // enter critical section
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  // clear the event
  event_status &= ~theEvent;//~(LETIMER_UF_INTERRUPT_EVENT);
  // exit critical section
  CORE_EXIT_CRITICAL();
  return (theEvent);
} // get_event()


#if DEVICE_IS_BLE_SERVER
////for server only
////state machine to be executed
//void temperature_state_machine(sl_bt_msg_t *evt) {
//
//  my_state currentState;
//  static my_state nextState = state0_idle;
//  ble_data_struct_t *bleData = getBleDataPtr();
//
//  if((SL_BT_MSG_ID(evt->header)==sl_bt_evt_system_external_signal_id)
//      && (bleData->connected==true)
//      && (bleData->indication==true)) {
//
//      currentState = nextState;     //set current state of the process
//
//      switch(currentState) {
//
//        case state0_idle:
//          nextState = state0_idle;          //default
//
//          //check for underflow event
//          if(evt->data.evt_system_external_signal.extsignals == evt_TimerUF) {
//
//              //LOG_INFO("timerUF event\n\r");
//              //enable temperature sensor
//              //enable_sensor();
//
//              //wait for 80ms for sensor to power on
//              timerWaitUs_interrupt(80000);
//
//              nextState = state1_timer_wait;
//          }
//
//          break;
//
//        case state1_timer_wait:
//          nextState = state1_timer_wait;    //default
//
//          //check for COMP1 event after timerwait
//          if(evt->data.evt_system_external_signal.extsignals == evt_COMP1) {
//
//              //LOG_INFO("Comp1 event\n\r");
//
//              //set the processor in EM1 energy mode
//              sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
//
//              //send write command to slave
//              write_cmd();
//
//              nextState = state2_write_cmd;
//          }
//
//          break;
//
//        case state2_write_cmd:
//          nextState = state2_write_cmd;     //default
//
//          //check for I2C transfer complete event after writing command to slave
//          if(evt->data.evt_system_external_signal.extsignals == evt_TransferDone) {
//
//              //LOG_INFO("write transfer done\n\r");
//              //remove processor from EM1 energy mode
//              sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
//
//              //wait 10.8ms for measurement of temperature
//              timerWaitUs_interrupt(10800);
//
//              nextState = state3_write_wait;
//          }
//
//          break;
//
//        case state3_write_wait:
//          nextState = state3_write_wait;    //default
//
//          //check for COMP1 event after timerwait
//          if(evt->data.evt_system_external_signal.extsignals == evt_COMP1) {
//
//              //read data from sensor
//              read_cmd();
//
//              //set the processor in EM1 energy mode
//              sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
//
//              nextState = state4_read;
//          }
//
//          break;
//
//        case state4_read:
//          nextState = state4_read;          //default
//
//          //check for I2C transfer complete event after reading data from slave
//          if(evt->data.evt_system_external_signal.extsignals == evt_TransferDone) {
//
//              //LOG_INFO("read transfer  done\n\r");
//              //remove processor from EM1 energy mode
//              sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
//
//              //disable si7021 sensor
//              //disable_sensor();
//
//              //disable I2C interrupt
//              NVIC_DisableIRQ(I2C0_IRQn);
//
//              //log temperature value
//              //LOG_INFO("Temp = %f C\n\r", convertTemp());
//
//              //send temperature indication to client
//              ble_SendTemp();
//
//              nextState = state0_idle;
//          }
//
//          break;
//
//        default:
//
//          LOG_ERROR("Should not be here in state machine\n\r");
//
//          break;
//      }
//
//  }
//  return;
//}

#else

void discovery_state_machine(sl_bt_msg_t *evt) {

  my_state currentState;
  static my_state nextState = state0_idle_client;

  ble_data_struct_t *bleData = getBleDataPtr();

  if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_connection_closed_id) {
      nextState = state0_idle_client;
  }

  currentState = nextState;     //set current state of the process

  switch(currentState) {

    //stay in idle state
    case state0_idle_client:
      nextState = state0_idle_client;          //default

      //wait for connection open event
      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_connection_opened_id) {

          //LOG_INFO("Discovering services\n\r");

          //Discover primary services with the specified UUID in a remote GATT database.
          rc = sl_bt_gatt_discover_primary_services_by_uuid(bleData->connection_handle,
                                                            sizeof(thermo_service),
                                                            (const uint8_t*)thermo_service);
          if(rc != SL_STATUS_OK) {
              LOG_ERROR("sl_bt_gatt_discover_primary_services_by_uuid() 1 returned != 0 status=0x%04x\n\r", (unsigned int)rc);
          }

          //gatt command in process
          bleData->gatt_procedure = true;

          nextState = state0_get_another_service;          //default
      }
      break;

      //discover button state service
    case state0_get_another_service:
      nextState = state0_get_another_service;          //default

      //wait for previous gatt command to be completed
      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id) {


          //LOG_INFO("Discovering services 2\n\r");

          rc = sl_bt_gatt_discover_primary_services_by_uuid(bleData->connection_handle,
                                                            sizeof(button_service),
                                                            (const uint8_t*)button_service);
          if(rc != SL_STATUS_OK) {
              LOG_ERROR("sl_bt_gatt_discover_primary_services_by_uuid() 2 returned != 0 status=0x%04x\n\r", (unsigned int)rc);
          }

          //gatt command in process
          bleData->gatt_procedure = true;

          nextState = state1_got_services;
      }

      break;

      //got service from server
    case state1_got_services:
      nextState = state1_got_services;

      //wait for previous gatt command to be completed
      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id) {

          //LOG_INFO("Discovering characteristics\n\r");


          //Discover all characteristics of a GATT service in a remote GATT database
          // having the specified UUID
          rc = sl_bt_gatt_discover_characteristics_by_uuid(bleData->connection_handle,
                                                           bleData->service_handle,
                                                           sizeof(thermo_char),
                                                           (const uint8_t*)thermo_char);
          if(rc != SL_STATUS_OK) {
              LOG_ERROR("sl_bt_gatt_discover_characteristics_by_uuid() 1 returned != 0 status=0x%04x\n\r", (unsigned int)rc);
          }

          //gatt command in process
          bleData->gatt_procedure = true;

          nextState = state1_got_another_services;
      }

      break;

      //discover button state characteristics
    case state1_got_another_services:
      nextState = state1_got_another_services;

      //wait for previous gatt command to be completed
      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id) {


          // LOG_INFO("Discovering services 2\n\r");

          rc = sl_bt_gatt_discover_characteristics_by_uuid(bleData->connection_handle,
                                                           bleData->button_service_handle,
                                                           sizeof(button_char),
                                                           (const uint8_t*)button_char);
          if(rc != SL_STATUS_OK) {
              LOG_ERROR("sl_bt_gatt_discover_characteristics_by_uuid() 2 returned != 0 status=0x%04x\n\r", (unsigned int)rc);
          }

          //gatt command in process
          bleData->gatt_procedure = true;

          nextState = state2_got_char;
      }

      break;

      //got characteristic from server
      //enable indications for temperature service
    case state2_got_char:
      nextState = state2_got_char;

      //wait for previous gatt command to be completed
      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id) {

          //LOG_INFO("Enabling notifications\n\r");


          //enable indications sent from server
          rc = sl_bt_gatt_set_characteristic_notification(bleData->connection_handle,
                                                          bleData->char_handle,
                                                          sl_bt_gatt_indication);
          if(rc != SL_STATUS_OK) {
              LOG_ERROR("sl_bt_gatt_set_characteristic_notification() returned != 0 status=0x%04x\n\r", (unsigned int)rc);
          }

          //gatt command in process
          bleData->gatt_procedure = true;

          nextState = state2_got_another_char;
      }

      break;

      //enble indications for button state service
    case state2_got_another_char:
      nextState = state2_got_another_char;

      //wait for previous gatt command to be completed
      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id) {


          ///LOG_INFO("Enabling notifications 2\n\r");

          rc = sl_bt_gatt_set_characteristic_notification(bleData->connection_handle,
                                                          bleData->button_char_handle,
                                                          sl_bt_gatt_indication);
          if(rc != SL_STATUS_OK) {
              LOG_ERROR("sl_bt_gatt_set_characteristic_notification() returned != 0 status=0x%04x\n\r", (unsigned int)rc);
          }

          //gatt command in process
          bleData->gatt_procedure = true;
          bleData->button_indication = true;

          displayPrintf(DISPLAY_ROW_CONNECTION, "Handling indications");
          nextState = state3_set_indication;
      }

      break;

      //indication is set on from server
    case state3_set_indication:
      nextState = state3_set_indication;

      //gatt complete
      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_gatt_procedure_completed_id) {

          nextState = state4_wait_for_close;
      }

      break;

      //state to wait for a connection close event
    case state4_wait_for_close:
      nextState = state4_wait_for_close;

      if(SL_BT_MSG_ID(evt->header) == sl_bt_evt_connection_closed_id) {

          //go in idle state to wait for a connection open event
          nextState = state0_idle_client;
      }

      break;

    default:

      LOG_ERROR("Should not be here in state machine\n\r");

      break;

  }
}

#endif
