/***************************************************************************//**
 * @file
 * @brief main() function.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Date:        08-07-2021
 * Author:      Dave Sluiter
 * Description: This code was created by the Silicon Labs application wizard
 *              and started as "Bluetooth - SoC Empty".
 *              It is to be used only for ECEN 5823 "IoT Embedded Firmware".
 *              The MSLA referenced above is in effect.
 *              
 *              This code was written with:
 *                  Gecko SDK 3.2.1
 *                  GNU ARM 10.2.1
 *
 ******************************************************************************/

#include "main.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG 1
#include "src/log.h"
#include "src/imu.h"




int main(void)
{

  // Initialize Silicon Labs device, system, service(s) and protocol stack(s).
  // Note that if the kernel is present, processing task(s) will be created by
  // this call.
  sl_system_init();

  // Initialize the application. For example, create periodic timer(s) or
  // task(s) if the kernel is present.
  // Students: Place all of your 1 time initialization code in this function.
  //           See app.c
  app_init();

//  init_imu();

#if defined(SL_CATALOG_KERNEL_PRESENT)

  // Start the kernel. Task(s) created in app_init() will start running.
  sl_system_kernel_start();

#else // SL_CATALOG_KERNEL_PRESENT

  // The so-called super-loop, sometimes called: "the main while(1) loop"
  while (1) {

      // Do not remove this call: Silicon Labs components process action routine
      // must be called from the super loop.
      // Students: This call will eventually call the function sl_bt_on_event()
      // defined in app.c which we will discuss in lecture and learn more about
      // in later programming assignments. Do not remove this call.
      sl_system_process_action();

      // Application process. See app.c
      app_process_action();

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
      // Let the CPU go to sleep if the system allows it.
      sl_power_manager_sleep();
#endif

  } // while

#endif // SL_CATALOG_KERNEL_PRESENT

} // main()



//
///***************************************************************************//**
// * @file main_tg11.c
// * @brief This project reads input on the UART RX port and echoes it back to the
// * TX port one line at a time. It does not use the VCOM or stdio. See readme.txt
// * for details.
// *******************************************************************************
// * # License
// * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
// *******************************************************************************
// *
// * SPDX-License-Identifier: Zlib
// *
// * The licensor of this software is Silicon Laboratories Inc.
// *
// * This software is provided 'as-is', without any express or implied
// * warranty. In no event will the authors be held liable for any damages
// * arising from the use of this software.
// *
// * Permission is granted to anyone to use this software for any purpose,
// * including commercial applications, and to alter it and redistribute it
// * freely, subject to the following restrictions:
// *
// * 1. The origin of this software must not be misrepresented; you must not
// *    claim that you wrote the original software. If you use this software
// *    in a product, an acknowledgment in the product documentation would be
// *    appreciated but is not required.
// * 2. Altered source versions must be plainly marked as such, and must not be
// *    misrepresented as being the original software.
// * 3. This notice may not be removed or altered from any source distribution.
// *
// *******************************************************************************
// * # Evaluation Quality
// * This code has been minimally tested to ensure that it builds and is suitable
// * as a demonstration for evaluation purposes only. This code will be maintained
// * at the sole discretion of Silicon Labs.
// ******************************************************************************/
//
//#include "em_device.h"
//#include "em_chip.h"
//#include "em_emu.h"
//#include "em_cmu.h"
//#include "em_gpio.h"
//#include "em_usart.h"
//
//#include "bsp.h"
//
//// Size of the buffer for received data
//#define BUFLEN  80
//
//// Receive data buffer
//uint8_t buffer[BUFLEN];
//
//// Current position ins buffer
//uint32_t inpos = 0;
//uint32_t outpos = 0;
//
//// True while receiving data (waiting for CR or BUFLEN characters)
//bool receive = true;
//
///**************************************************************************//**
// * @brief
// *    GPIO initialization
// *****************************************************************************/
//void initGpio(void)
//{
//  CMU_ClockEnable(cmuClock_GPIO, true);
//
//  // Configure VCOM transmit pin to board controller as an output
//  GPIO_PinModeSet(BSP_BCC_TXPORT, BSP_BCC_TXPIN, gpioModePushPull, 1);
//
//  // Configure VCOM reeive pin from board controller as an input
//  GPIO_PinModeSet(BSP_BCC_RXPORT, BSP_BCC_RXPIN, gpioModeInput, 0);
//
//  // Enable VCOM connection to board controller
//  GPIO_PinModeSet(BSP_BCC_ENABLE_PORT, BSP_BCC_ENABLE_PIN, gpioModePushPull, 1);
//}
//
///**************************************************************************//**
// * @brief
// *    USART1 initialization (VCOM on xG1/xG12/xG13 boards)
// *****************************************************************************/
//void initUsart1(void)
//{
//  CMU_ClockEnable(cmuClock_USART1, true);
//
//  // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
//  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
//
//  // Configure and enable USART1
//  USART_InitAsync(USART1, &init);
//
//  // Enable NVIC USART sources
//  NVIC_ClearPendingIRQ(USART1_IRQn);
//  NVIC_EnableIRQ(USART1_IRQn);
//
//  // Enable RX and TX for USART1 VCOM connection
//  USART1->ROUTELOC0 = BSP_BCC_RX_LOCATION | BSP_BCC_TX_LOCATION;
//  USART1->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
//}
//
///**************************************************************************//**
// * @brief
// *    The USART1 receive/transmit interrupt processes characters.
// *****************************************************************************/
//void USART1_IRQHandler(void)
//{
//  // In the receive state
//  if (receive)
//  {
//    // Get the character just received
//    buffer[inpos] = USART1->RXDATA;
//
//    // Exit loop on new line or buffer full
//    if ((buffer[inpos] != '\r') && (inpos < BUFLEN))
//      inpos++;
//    else
//      receive = false;   // Stop receiving on CR
//  }
//  // In the transmit state
//  else
//  {
//    // Send a previously received character
//    if (outpos < inpos)
//      USART1->TXDATA = buffer[outpos++];
//    else
//    /*
//     * Need to disable the transmit buffer level interrupt in this IRQ
//     * handler when done or it will immediately trigger again upon exit
//     * even though there is no data left to send.
//     */
//    {
//      receive = true;   // Go back into receive when all is sent
//      USART_IntDisable(USART1, USART_IEN_TXBL);
//    }
//  }
//}
//
///**************************************************************************//**
// * @brief
// *    Main function
// *****************************************************************************/
//int main(void)
//{
//  uint32_t i;
//
//  // Chip errata
//  CHIP_Init();
//
//  // Initialize GPIO and USART0
//  initGpio();
//  initUsart1();
//
//  while (1)
//  {
//    // Zero out buffer
//    for (i = 0; i < BUFLEN; i++)
//      buffer[i] = 0;
//
//    // Enable receive data valid interrupt
//    USART_IntEnable(USART1, USART_IEN_RXDATAV);
//
//    // Wait in EM1 while receiving to reduce current draw
//    while (receive)
//      EMU_EnterEM1();
//
//    // Disable receive data valid interrupt
//    USART_IntDisable(USART1, USART_IEN_RXDATAV);
//
//    // Enable transmit buffer level interrupt
//    USART_IntEnable(USART1, USART_IEN_TXBL);
//
//    // Wait in EM1 while transmitting to reduce current draw
//    while (!receive)
//      EMU_EnterEM1();
//
//    // Reset buffer indices
//    inpos = outpos = 0;
//  }
//}
