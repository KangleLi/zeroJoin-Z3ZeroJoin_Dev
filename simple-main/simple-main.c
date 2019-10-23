/***************************************************************************//**
 * @file
 * @brief This file provides a definition for main() for non-RTOS applications.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include PLATFORM_HEADER
#include "app/framework/include/af.h"

// Our entry point is typically main(), except during testing.
#ifdef EMBER_TEST
  #define MAIN nodeMain
#else
  #define MAIN main
#endif

int MAIN(MAIN_FUNCTION_PARAMETERS)
{
  // Disable JTAG
  BUS_RegBitWrite(&(GPIO->ROUTEPEN), _GPIO_ROUTEPEN_TDIPEN_SHIFT, false);	// TDIPEN (JTAG Test Debug Input Pin)
  BUS_RegBitWrite(&(GPIO->ROUTEPEN), _GPIO_ROUTEPEN_TDOPEN_SHIFT, false);	// TDOPEN (JTAG Test Debug Output Pin)
  volatile static uint32_t counter ;
  volatile static uint32_t i;
  for (counter = 0; counter < 5; counter++) {
	for (i = 0; i < 1000000; i++) {
	}
  }
  GPIO_DbgSWDClkEnable(false);
  GPIO_DbgSWDIOEnable(false);
  GPIO_DbgSWOEnable(false);
  halInit();
  emberAfMainInit();
  return emberAfMain(MAIN_FUNCTION_ARGUMENTS);
}
