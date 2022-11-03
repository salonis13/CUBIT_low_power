/*
 * i2c.c
 *
 *  Created on: Sep 16, 2021
 *      Author: salon
 */
// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "log.h"

#include "src/i2c.h"
#include "app.h"


#define SI7021_DEVICE_ADDR 0x40


uint8_t cmd_data;
uint8_t read_data[2];
I2C_TransferSeq_TypeDef transfer_seq;

uint8_t data[2];  //data[0] Write and data[1] Read

void i2c_init() {

  //struct for i2c initialization
  I2CSPM_Init_TypeDef I2C_Config = {
      .port = I2C0,
      .sclPort = gpioPortC,
      .sclPin =  11,//10,
      .sdaPort = gpioPortC,
      .sdaPin = 10,//11,
      .portLocationScl = 15,
      .portLocationSda = 15,
      .i2cRefFreq = 0,
      .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
      .i2cClhr = i2cClockHLRStandard
  };

  //initialize i2c peripheral
  I2CSPM_Init(&I2C_Config);

  //enable I2C interrupt
  NVIC_EnableIRQ(I2C0_IRQn);

}

void i2c_deinit() {

  I2C_Enable(I2C0, false);

  //configuring PC10 and PC11 as GPIOs
  GPIO_PinModeSet(gpioPortC, 10, gpioModeDisabled, true);
  GPIO_PinModeSet(gpioPortC, 11, gpioModeDisabled, true);

}

//function to perform write command operation on slave
void write_cmd() {

  I2C_TransferReturn_TypeDef transferStatus;

  i2c_init();

  //structure to write command from master to slave
  cmd_data = 0xF3;
  transfer_seq.addr = SI7021_DEVICE_ADDR << 1;
  transfer_seq.flags = I2C_FLAG_WRITE;
  transfer_seq.buf[0].data = &cmd_data;
  transfer_seq.buf[0].len = sizeof(cmd_data);

  //enable I2C interrupt
  NVIC_EnableIRQ(I2C0_IRQn);

  //initialize I2C transfer
  transferStatus = I2C_TransferInit(I2C0, &transfer_seq);

  //check transfer function return status
  if(transferStatus < 0) {
      LOG_ERROR("I2C_TransferInit status %d write: failed\n\r", (uint32_t)transferStatus);
  }
}


//function to perform write command operation on slave
void i2c_write(uint8_t address, uint8_t cmd) {

  I2C_TransferReturn_TypeDef transferStatus;

  i2c_init();

  //structure to write command from master to slave
  transfer_seq.addr = address << 1;
  transfer_seq.flags = I2C_FLAG_WRITE;
  transfer_seq.buf[0].data = &cmd;
  transfer_seq.buf[0].len = sizeof(cmd_data);

  //enable I2C interrupt
  NVIC_EnableIRQ(I2C0_IRQn);

  //initialize I2C transfer
  transferStatus = I2C_TransferInit(I2C0, &transfer_seq);

  //check transfer function return status
  if(transferStatus < 0) {
      LOG_ERROR("I2C_TransferInit status %d write: failed\n\r", (uint32_t)transferStatus);
  }
}

/** -------------------------------------------------------------------------------------------
 * @brief i2c routine to read data from sensor over i2c
 *
 * @param true/false
 * @return Read value
 *-------------------------------------------------------------------------------------------- **/
I2C_TransferReturn_TypeDef i2c_read(uint8_t address, uint8_t* data_buffer)
{
  I2C_TransferReturn_TypeDef transferStatus;

  NVIC_DisableIRQ(I2C0_IRQn);
  transfer_seq.addr = address << 1;
  transfer_seq.flags = I2C_FLAG_READ;
  transfer_seq.buf[0].data = data_buffer;
  transfer_seq.buf[0].len = 1;

  NVIC_EnableIRQ(I2C0_IRQn);
  transferStatus = I2C_TransferInit(I2C0, &transfer_seq);

  //check transfer function return status
    if(transferStatus < 0) {
        LOG_ERROR("I2C_TransferInit status %d write: failed\n\r", (uint32_t)transferStatus);
    }

  return transferStatus;
}

uint8_t data[2];
/** -------------------------------------------------------------------------------------------
 * @brief i2c routine to write data to sensor over i2c
 *
 * @param true/false
 * @return None
 *-------------------------------------------------------------------------------------------- **/
I2C_TransferReturn_TypeDef i2c_write_write(uint8_t address, uint8_t reg, uint8_t value)
{
  I2C_TransferReturn_TypeDef transferStatus;

//  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);

  data[0] = reg;
  data[1] = value;
  NVIC_DisableIRQ(I2C0_IRQn);
  transfer_seq.addr = address << 1;
  transfer_seq.flags = I2C_FLAG_WRITE_WRITE;
  transfer_seq.buf[0].data = data;
  transfer_seq.buf[0].len = 2;

  NVIC_EnableIRQ(I2C0_IRQn);
  transferStatus = I2C_TransferInit(I2C0, &transfer_seq);

  //check transfer function return status
    if(transferStatus < 0) {
        LOG_ERROR("I2C_TransferInit status %d write: failed\n\r", (uint32_t)transferStatus);
    }

  return transferStatus;
}


uint8_t register_add;
uint8_t data_read_from_imu_sensor[2];
/** -------------------------------------------------------------------------------------------
 * @brief i2c routine to read data from a sensor over i2c
 *
 * @param true/false
 * @return None
 *-------------------------------------------------------------------------------------------- **/
I2C_TransferReturn_TypeDef i2c_write_read(uint8_t address, uint8_t reg)
{
  I2C_TransferReturn_TypeDef transferStatus;

  //S+ADDR(W)+DATA0+Sr+ADDR(R)+DATA1+P
  register_add = reg;

//  sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);

  NVIC_DisableIRQ(I2C0_IRQn);
  transfer_seq.addr = address << 1;
  transfer_seq.flags = I2C_FLAG_WRITE_READ;
  transfer_seq.buf[0].data = &register_add;
  transfer_seq.buf[0].len = 1;
  transfer_seq.buf[1].data = &data_read_from_imu_sensor[0];
  transfer_seq.buf[1].len = sizeof(data_read_from_imu_sensor);

  NVIC_EnableIRQ(I2C0_IRQn);
  transferStatus = I2C_TransferInit(I2C0, &transfer_seq);

  //check transfer function return status
    if(transferStatus < 0) {
        LOG_ERROR("I2C_TransferInit status %d write-read: failed\n\r", (uint32_t)transferStatus);
    }

    return transferStatus;
}


//function to perform read operation from slave
void read_cmd() {

  I2C_TransferReturn_TypeDef transferStatus;

  i2c_init();

  //structure to read temperature measurement from slave
  transfer_seq.addr = SI7021_DEVICE_ADDR << 1;
  transfer_seq.flags = I2C_FLAG_READ;
  transfer_seq.buf[0].data = &read_data[0];
  transfer_seq.buf[0].len = sizeof(read_data);

  //enable I2C interrupt
  NVIC_EnableIRQ(I2C0_IRQn);

  //initialize I2C transfer
  transferStatus = I2C_TransferInit(I2C0, &transfer_seq);

  //check transfer function return status
  if(transferStatus < 0) {
      LOG_ERROR("I2C_TransferInit status %d read: failed\n\r", (uint32_t)transferStatus);
  }
}

//function to convert data read from sensor into temperature value in Celcius
float convertTemp() {

  float tempCelcius;
  uint16_t read_temp;
  read_temp = (read_data[0] << 8);
  read_temp += read_data[1];
  tempCelcius = (175.72*(read_temp));
  tempCelcius /= 65536;
  tempCelcius -= 46.85;
//  return tempCelcius;
  return 1.23;
}
