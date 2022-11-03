// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "log.h"

#include "../app.h"

#include "ultrasonic.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_leuart.h"
#include "em_chip.h"
#include "em_usart.h"
#include "timers.h"
#include "cbfifo.h"

extern cbfifo_t cbfifo_receive;


// Size of the buffer for received data
#define BUFLEN  80

// Receive data buffer
uint8_t buffer[BUFLEN];

// Current position ins buffer
uint32_t inpos = 0;
uint32_t outpos = 0;

// True while receiving data (waiting for CR or BUFLEN characters)
bool receive = true;

//static char txBuffer[RX_BUFFER_SIZE]; // Software transmit buffer

///**************************************************************************//**
// * @brief
// *    Initialize the GPIO pins for the LEUART module
// *****************************************************************************/
//void initGpio(void)
//{
//  timerWaitUs_polled(1000000);
//  // GPIO clock
//  CMU_ClockEnable(cmuClock_GPIO, true);
//
//  // Initialize LEUART0 TX and RX pins
////  GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1); // TX
//  GPIO_PinModeSet(gpioPortA, 5, gpioModeInput, 0);    // RX
//
//  //PA2: RX US1_RX #1
//  //PA3: TX US1_TX #3
//
////  // Configure VCOM transmit pin to board controller as an output
////  GPIO_PinModeSet(BSP_BCC_TXPORT, BSP_BCC_TXPIN, gpioModePushPull, 1);
////
////  // Configure VCOM reeive pin from board controller as an input
////  GPIO_PinModeSet(BSP_BCC_RXPORT, BSP_BCC_RXPIN, gpioModeInput, 0);
////
////  // Enable VCOM connection to board controller
////  GPIO_PinModeSet(BSP_BCC_ENABLE_PORT, BSP_BCC_ENABLE_PIN, gpioModePushPull, 1);
//}


#if 0
/**************************************************************************//**
 * @brief
 *    Initialize the USART module
 *****************************************************************************/
void init_usart2(void)
{

  /*********************Setting up GPIOs for USART2*****************/
  //initializing gpio for usart2
  // GPIO clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Initialize LEUART0 TX and RX pins
//  GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1); // TX
  GPIO_PinModeSet(gpioPortA, 5, gpioModeInput, 0);    // RX




  //PA2: RX US1_RX #1
  //PA3: TX US1_TX #3

  //PA5: RX US2_RX #31  //P16 on WSTK board


  /*********************Setting up USART2*************************/

  // Enable clocks for LEUART0
  CMU_ClockEnable(cmuClock_USART2, true);
//  CMU_ClockDivSet(cmuClock_USART1, cmuClkDiv_1); // Don't prescale LEUART clock

  //Initialize the USART1 module

  USART_InitAsync_TypeDef init = {                                                                   \
      usartEnable,           /* Enable RX/TX when initialization is complete. */                     \
      0,                     /* Use current configured reference clock for configuring baud rate. */ \
      9600,                /* 9600 bits/s. */                                                    \
      usartOVS4,            /* 16x oversampling. */                                                 \
      usartDatabits8,        /* 8 data bits. */                                                      \
      usartNoParity,         /* No parity. */                                                        \
      usartStopbits1,        /* 1 stop bit. */                                                       \
      false,                 /* Do not disable majority vote. */                                     \
      false,                 /* Not USART PRS input mode. */                                         \
      0,                     /* PRS channel 0. */                                                    \
      false,                 /* Auto CS functionality enable/disable switch */                       \
      0,                     /* Auto CS Hold cycles */                                               \
      0,                     /* Auto CS Setup cycles */                                              \
      usartHwFlowControlNone /* No HW flow control */                                                \
    };



  // Configure and enable USART1
  USART_InitAsync(USART2, &init);

  // Enable NVIC USART sources
  NVIC_ClearPendingIRQ(USART2_RX_IRQn);
  NVIC_EnableIRQ(USART2_RX_IRQn);
//  NVIC_ClearPendingIRQ(USART2_TX_IRQn);
//  NVIC_EnableIRQ(USART2_TX_IRQn);

  // Enable RX and TX for USART1 VCOM connection
  USART2->ROUTELOC0 = /*USART_ROUTELOC0_TXLOC_LOC3 | */USART_ROUTELOC0_RXLOC_LOC31;
  USART2->ROUTEPEN |= USART_ROUTEPEN_RXPEN/* | USART_ROUTEPEN_TXPEN*/;
  USART2->CTRL |= USART_CTRL_RXINV;

  USART_IntEnable(USART2, USART_IEN_RXDATAV);
  LOG_INFO("Finished setting up usart2\n\r");

}


/*----------------------------------------------------------------------------
 * Standard system I/O function
 ----------------------------------------------------------------------------*/
char get_received_char_usart2(void)
{
  char c;

  //returns -1 to indicate that no new char has received
  if(cbfifo_length(&cbfifo_receive) < 1)
  {
    return '@';
  }
  else
  {
    //Entering critical section
      // enter critical section
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    cbfifo_dequeue(&cbfifo_receive, &c, sizeof(c));
    CORE_EXIT_CRITICAL();
    return c;
  }
}



/*----------------------------------------------------------------------------
 * Standard system I/O function
 ----------------------------------------------------------------------------*/
int get_processed_data_from_ultrasonic_sensor(void)
{
  char c;
  c = get_received_char_usart2();
  static uint8_t multiplier;
  static uint8_t distance;

  switch(c)
  {
    case '@': //'@' is returned when there is no character in circular buffer
      break;

    case '\r':
      distance = 0;
      multiplier = 1;
      break;

    case 'R':
      multiplier = 0;
      return distance;
      break;

    default:
      distance += multiplier * (c - '0');
      multiplier = multiplier*10;
      break;
  }
  return 0;
}

#endif

void ultrasonicInit()
{
  gpioUltrasonicSetOn();


}


/**************************************************************************//**
 * @brief
 *    Initialize the leuart module
 *****************************************************************************/
void init_leuart(void)
{

  /*********************Setting up GPIOs for USART2*****************/
  //initializing gpio for usart2
  // GPIO clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Initialize LEUART0 TX and RX pins
//  GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 1); // TX
  //GPIO_PinModeSet(gpioPortA, 5, gpioModeInput, 0);    // RX

  GPIO_PinModeSet(gpioPortF, 7, gpioModeInput, 0);    // RX




  //PA2: RX US1_RX #1
  //PA3: TX US1_TX #3

  //PA5: RX US2_RX #31  //P16 on WSTK board


  /*********************Setting up USART2*************************/

  // Enable LE (low energy) clocks
    CMU_ClockEnable(cmuClock_HFLE, true); // Necessary for accessing LE modules
    CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO); // Set a reference clock

    // Enable clocks for LEUART0
    CMU_ClockEnable(cmuClock_LEUART0, true);
    CMU_ClockDivSet(cmuClock_LEUART0, cmuClkDiv_1); // Don't prescale LEUART clock

    // Initialize the LEUART0 module
    LEUART_Init_TypeDef init = LEUART_INIT_DEFAULT;
    LEUART_Init(LEUART0, &init);



  // Enable RX and TX for LEUART
  LEUART0->CTRL |= LEUART_CTRL_INV;
  LEUART0->ROUTELOC0 = /*USART_ROUTELOC0_TXLOC_LOC3 | */USART_ROUTELOC0_RXLOC_LOC30;
  LEUART0->ROUTEPEN |= USART_ROUTEPEN_RXPEN/* | USART_ROUTEPEN_TXPEN*/;



  LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);
  NVIC_ClearPendingIRQ(LEUART0_IRQn);
  NVIC_EnableIRQ(LEUART0_IRQn);


  LOG_INFO("Finished setting up LEUART0\n\r");

}


/*----------------------------------------------------------------------------
 * Standard system I/O function
 ----------------------------------------------------------------------------*/
char get_received_char_leuart0(void)
{
  char c;

  //returns -1 to indicate that no new char has received
  if(cbfifo_length(&cbfifo_receive) < 1)
  {
    return '@';
  }
  else
  {
    //Entering critical section
      // enter critical section
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    cbfifo_dequeue(&cbfifo_receive, &c, sizeof(c));
    CORE_EXIT_CRITICAL();
    return c;
  }
}



/*----------------------------------------------------------------------------
 * Standard system I/O function
 ----------------------------------------------------------------------------*/
int get_processed_data_from_ultrasonic_sensor(void)
{
  char c;
  c = get_received_char_leuart0();
  static uint8_t multiplier;
  static uint32_t distance;

  switch(c)
  {
    case '@': //'@' is returned when there is no character in circular buffer
      break;

    case '\r':
      return distance;
     // multiplier = 0;
      break;

    case 'R':
      multiplier = 100;
      distance = 0;

      break;

    default:
      distance += multiplier * (c - '0');
      multiplier = multiplier/10;
      break;
  }
  return 0;
}
