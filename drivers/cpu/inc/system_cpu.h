/**
******************************************************************************************************************************************************************************************
*@file                       system_cpu.h
*@author         
*@data                       
*@version                    v1.0
*@par Copyright(c):
*
*@par History:
*     version:author,date,desc
*     v1.0   :create this file
*
*******************************************************************************************************************************************************************************************
*/
#ifndef SYSTEM_CPU_H   /* ToDo: replace '<Device>' with your device name */
#define SYSTEM_CPU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "cpu.h"

extern uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  */

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
/* ToDo: add here your necessary defines for device initialization
         following is an example for different system frequencies             */
#define __HSI             (16000000UL)
#define __XTAL            (16000000UL)    /* Oscillator frequency             */
#define __SYS_OSC_CLK     (    ___HSI)    /* Main oscillator frequency        */

#define __SYSTEM_CLOCK    (1*__XTAL)



/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
 * Update SystemCoreClock variable
 *
 * @param  none
 * @return none
 *
 * @brief  Updates the SystemCoreClock with current core Clock
 *         retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_<Device>_H */
