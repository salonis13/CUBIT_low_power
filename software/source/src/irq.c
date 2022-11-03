/*
 * irq.c
 *
 * Function to
 *  Created on: Sep 8, 2021
 *      Author: salon
 */

// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "log.h"

#include "app.h"
#include "src/irq.h"
#include "src/cbfifo.h"
#include "em_usart.h"
#include "em_leuart.h"
#include "src/scheduler.h"

extern cbfifo_t cbfifo_receive;
extern uint32_t curr_lcd_screen;


last_pulse_t mag_enc_last_pulse = NOT_SET;


mag_enc_t mag_enc = {0, 0, NONE, NOT_SET};

uint32_t encoder_pulses_A = 0;
uint32_t letimer_uf_count = 0;


uint8_t mode = 0;

uint32_t letimerMilliseconds() {
  uint32_t time_ms;
  ble_data_struct_t *bleData = getBleDataPtr();
  time_ms = ((bleData->rollover_cnt)*1000);
  return time_ms;
}

void LETIMER0_IRQHandler(void) {

  ble_data_struct_t *bleData = getBleDataPtr();

  // determine pending interrupts in peripheral
  uint32_t reason = LETIMER_IntGetEnabled(LETIMER0);
  // clear pending interrupts in peripheral
  LETIMER_IntClear(LETIMER0, reason);

  //check for COMP1 interrupt
  if(reason & LETIMER_IF_COMP1) {

      //disable COMP1 interrupt of timer peripheral
      LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);

      //set scheduler event
      schedulerSetEventCOMP1();
  }

  //check for UF interrupt
  if(reason & LETIMER_IF_UF) {

      //set scheduler event


      letimer_uf_count++;
      if(mode == 0) //sensor 0 IMU
        {
          if(letimer_uf_count == 3) //every second
            {
                    letimer_uf_count = 0;
                    schedulerSetEventUF();
            }
        }
      else if(mode == 1) //sensor 1 RangeFinder
        {
          if(letimer_uf_count == 2) //every 2 seconds
            {
                    letimer_uf_count = 0;
                    schedulerSetEventUF();
            }
        }
      else if(mode == 2) //sensor 2 Magnetic Encoder
        if(letimer_uf_count == 6)  //every 6 seconds
        {
          schedulerSetEventUF();
          letimer_uf_count = 0;
          encoder_pulses_A = 0;
        }

      bleData->rollover_cnt+=1;

  }

}

void I2C0_IRQHandler(void) {

  I2C_TransferReturn_TypeDef transferStatus;

  //get I2C transfer status
  transferStatus = I2C_Transfer(I2C0);

  //check if I2C transfer is done
  if(transferStatus == i2cTransferDone) {

      //disable I2C transfer
      NVIC_DisableIRQ(I2C0_IRQn);

      //set scheduler event
      schedulerSetEventTransferDone();
//      sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
//      sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM2);
  }
  else if(transferStatus == i2cTransferNack) {

      //disable I2C transfer
            schedulerSetEventRetryTransfer();
    }

  if(transferStatus < 0) {

      LOG_ERROR("I2C_TStatus %d : failed\n\r", (uint32_t)transferStatus);

  }
}

//void GPIO_EVEN_IRQHandler(void) {
//
//  ble_data_struct_t *bleData = getBleDataPtr();
//
//  // determine pending interrupts in peripheral
//  uint32_t reason = GPIO_IntGet();
//
//  GPIO_IntClear(reason);
//
//  //get the push button status
//  uint8_t button_status = GPIO_PinInGet(PB0_port, PB0_pin);
//
//  //check if the interrupt triggered was from PB0
//  if(reason == 64) {
//
//      if(!button_status) {
//          bleData->button_pressed = true;
//          schedulerSetEventButtonPressed();
//      }
//
//      else {
//          bleData->button_pressed = false;
//          schedulerSetEventButtonReleased();
//      }
//  }
//}

#if 0

/**************************************************************************//**
 * @brief
 *    The USART0 receive interrupt saves incoming characters.
 *****************************************************************************/
void USART2_RX_IRQHandler(void)
{
  uint32_t received_data = 0;
  uint32_t flags = USART_IntGet(USART2);
  USART_IntClear(USART2, flags);
  uint8_t received_char = 0;
  char removed_character;


  // Get the character just received
  //received_data = USART2->RXDATA;
  received_char = (uint8_t)USART2->RXDATA;

//  received_char = (uint8_t)~received_char;
  //buf[inpos] = ~buf[inpos];

  //if character is received save it to Fifo
  if(cbfifo_length(&cbfifo_receive) != cbfifo_capacity(&cbfifo_receive)) //if fifo not full
    {
      //entering critical section
      // enter critical section
        CORE_DECLARE_IRQ_STATE;
        CORE_ENTER_CRITICAL();
//      masking_state = __get_PRIMASK();
//      __disable_irq();
      cbfifo_enqueue(&cbfifo_receive, &received_char, sizeof(received_char));
//      __set_PRIMASK(masking_state);

      // exit critical section
      CORE_EXIT_CRITICAL();
    }
    else
    {
      //character discarded when queue is full
//        received_char = 0;
//        for(int i=0;i<256;i++)
//          {
//            removed_character = get_received_char_usart2();
//          }

    }

}

/** -------------------------------------------------------------------------------------------
* Interrupt handler for Even GPIOs
*-------------------------------------------------------------------------------------------- **/
void GPIO_EVEN_IRQHandler()
{
  //Getting reason behind raised interrupt
  uint32_t reason = GPIO_IntGet();

  //Clearing an interrupt
  GPIO_IntClear(reason);

  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  if(reason == 64)
  {
    if(GPIO_PinInGet(PB0_port, PB0_pin) == false)
      schedulerSetEventButtonPressed();
    else if(GPIO_PinInGet(PB0_port, PB0_pin) == true)
      {
        schedulerSetEventButtonReleased();
        GPIO_PinOutToggle(LED0_port,LED0_pin);
        mode++;
        if(mode == 3)
          {
            mode = 0;
          }
      }
  }
  else if(reason == 1024)
  {
    encoder_pulses_A++;
    reason = 0;
    //scheduler_set_event_proximity_detected();
    //Interuupt on proximity received
  }

  CORE_EXIT_CRITICAL();


} // GPIO_EVEN_IRQHandler()


#endif

/**************************************************************************//**
 * @brief
 *    The USART0 receive interrupt saves incoming characters.
 *****************************************************************************/
void LEUART0_IRQHandler(void)
{
//  uint32_t received_data = 0;
////  uint32_t flags = USART_IntGet(USART2);
//  char received_char = 0;
//  char removed_character;

  // Acknowledge the interrupt
    uint32_t flags = LEUART_IntGet(LEUART0);
    LEUART_IntClear(LEUART0, flags);


    // RX portion of the interrupt handler
      if (flags & LEUART_IF_RXDATAV) {
        while (LEUART0->STATUS & LEUART_STATUS_RXDATAV) { // While there is still incoming data

        char received_char = LEUART_Rx(LEUART0);
          if(cbfifo_length(&cbfifo_receive) != cbfifo_capacity(&cbfifo_receive)) //if fifo not full
            {
              CORE_DECLARE_IRQ_STATE;
              CORE_ENTER_CRITICAL();
              cbfifo_enqueue(&cbfifo_receive, &received_char, sizeof(received_char));

              // exit critical section
              CORE_EXIT_CRITICAL();
            }
            else
            {
//                received_char = 5;
                CORE_DECLARE_IRQ_STATE;
                CORE_ENTER_CRITICAL();
                //clearing entire queue
                for(int i=0;i<CBFIFO_SIZE;i++)
                  //received_char = get_received_char_leuart0();
                  cbfifo_dequeue(&cbfifo_receive, &received_char, sizeof(received_char));
                CORE_EXIT_CRITICAL();
            }


        }
      }


#if 0

  // Get the character just received
  //received_data = USART2->RXDATA;
  received_char = (uint8_t)USART2->RXDATA;

//  received_char = (uint8_t)~received_char;
  //buf[inpos] = ~buf[inpos];

  //if character is received save it to Fifo
  if(cbfifo_length(&cbfifo_receive) != cbfifo_capacity(&cbfifo_receive)) //if fifo not full
    {
      //entering critical section
      // enter critical section
        CORE_DECLARE_IRQ_STATE;
        CORE_ENTER_CRITICAL();
//      masking_state = __get_PRIMASK();
//      __disable_irq();
      cbfifo_enqueue(&cbfifo_receive, &received_char, sizeof(received_char));
//      __set_PRIMASK(masking_state);

      // exit critical section
      CORE_EXIT_CRITICAL();
    }
    else
    {
      //character discarded when queue is full
//        received_char = 0;
//        for(int i=0;i<256;i++)
//          {
//            removed_character = get_received_char_usart2();
//          }

    }

#endif
}


void GPIO_EVEN_IRQHandler(void)
{

  ble_data_struct_t *bleData = getBleDataPtr();

  // determine pending interrupts in peripheral
  uint32_t reason_even = GPIO_IntGet();
  reason_even &= 0x55555555;  //making sure only even interrupts are cleared

  GPIO_IntClear(reason_even);

  //PA0 RIGHT reason 1
  //PA2 DOWN reason 4
  //PD14 MAG ENC B reason 16384


   if(reason_even == 16384) //Mag Enc Channel
    {
      CORE_DECLARE_IRQ_STATE;
       CORE_ENTER_CRITICAL();
      //setting initial direction of rotation
      if(mag_enc.rotation_direction == NONE)
        {
          mag_enc.rotation_direction = CLK_WISE;
        }

      //checking if direction of rotation is changed
      if(mag_enc.last_pulse == CHNL_A)
        {
          //change rotation direction;
          if(mag_enc.rotation_direction == CLK_WISE)
            mag_enc.rotation_direction = COUNTER_CLK_WISE;
          else if(mag_enc.rotation_direction == COUNTER_CLK_WISE)
            mag_enc.rotation_direction = CLK_WISE;
        }
      mag_enc.last_pulse = CHNL_A;

#ifdef INC_ONLY_FOR_WHEEL
      if(curr_lcd_screen != LCD_WHEEL_MEAS)
#endif
      {
          if(mag_enc.rotation_direction == CLK_WISE)
            mag_enc.clkwise_counter++;
          else if(mag_enc.rotation_direction == COUNTER_CLK_WISE)
            mag_enc.counter_clkwise_counter++;
      }
#ifdef INC_ONLY_FOR_WHEEL
      else
      {
          mag_enc.clkwise_counter++;
      }
#endif


      CORE_EXIT_CRITICAL();

    }
   //check if the interrupt triggered was from PB1
   else if(reason_even == 1)  //4 New Down
       {
         schedulerSetEventButtonDown();
       }
   else if(reason_even == 4)
       {
               schedulerSetEventButtonBack();
               mag_enc.clkwise_counter = 0;
               mag_enc.counter_clkwise_counter = 0;
       }


}

void GPIO_ODD_IRQHandler(void) {

  ble_data_struct_t *bleData = getBleDataPtr();

  // determine pending interrupts in peripheral
  uint32_t reason_odd = GPIO_IntGet();

  reason_odd &= 0xAAAAAAAA; //making sure only odd interrupts are cleared

  GPIO_IntClear(reason_odd);

  //PA3 UP reason 8
  //PA1 LEFT reason 2
  //PD13 MAG ENC A reason 8192

  //check if the interrupt triggered was from PB1

  if(reason_odd == 8192) //Mag Enc Channel
      {
        CORE_DECLARE_IRQ_STATE;
        CORE_ENTER_CRITICAL();
        //setting initial direction of rotation
        if(mag_enc.rotation_direction == NONE)
          {
            mag_enc.rotation_direction = COUNTER_CLK_WISE;
          }

        //checking if direction of rotation is changed
        if(mag_enc.last_pulse == CHNL_B)
          {
            //change rotation direction;
            if(mag_enc.rotation_direction == CLK_WISE)
              mag_enc.rotation_direction = COUNTER_CLK_WISE;
            else if(mag_enc.rotation_direction == COUNTER_CLK_WISE)
              mag_enc.rotation_direction = CLK_WISE;
          }
        mag_enc.last_pulse = CHNL_B;



        CORE_EXIT_CRITICAL();
      }

    else if(reason_odd == 2)
     {
       schedulerSetEventButtonUp();
     }
   else if(reason_odd == 8)  //8
     {
       schedulerSetEventButtonSelect();//
     }
}




