//Modified by Anurag Chugh on 2012-03-07

/****************************************************************************
*  Copyright (c) 2009 by Michael Fischer. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*  
*  1. Redistributions of source code must retain the above copyright 
*     notice, this list of conditions and the following disclaimer.
*  2. Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the 
*     documentation and/or other materials provided with the distribution.
*  3. Neither the name of the author nor the names of its contributors may 
*     be used to endorse or promote products derived from this software 
*     without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
*  THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
*  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
*  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF 
*  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
*  SUCH DAMAGE.
*
****************************************************************************
*
*  History:
*
*  25.09.09  mifi   First Version, based on the example from Quantum Leaps 
*                   with some small modifications. The original can be find 
*                   here: http://www.embedded.com/design/200900043
*
*                   For information e.g. how to setup the CPU take a look
*                   in the AT91 Software Packages which can be find here:
*                   http://www.atmel.com/dyn/products/tools_card.asp?tool_id=4343
****************************************************************************/
#define __LOW_LEVEL_INIT_C__

#include <stdint.h>
#include "board-netduino.h"

extern void UndefHandler (void);
extern void SWIHandler (void);
extern void PAbortHandler (void);
extern void DAbortHandler (void);
extern void IRQHandler (void);
extern void FIQHandler (void);

/*=========================================================================*/
/*  DEFINE: All Structures and Common Constants                            */
/*=========================================================================*/
/* LDR pc, [pc, #0x18] */
#define LDR_PC_PC       0xE59FF018U 
#define MAGIC           0xDEADBEEFU

/* Register Base Address */
#define RSTC_KEY        0xA5000000
#define RSTC_URSTEN     0x00000001

#define MC_FMR_OFF      0x00000060
#define MC_FWS_1FWS     0x00480100


/*=========================================================================*/
/*  DEFINE: Prototypes                                                     */
/*=========================================================================*/

/*=========================================================================*/
/*  DEFINE: Definition of all local Data                                   */
/*=========================================================================*/

/*=========================================================================*/
/*  DEFINE: Definition of all local Procedures                             */
/*=========================================================================*/

/*=========================================================================*/
/*  DEFINE: All code exported                                              */
/*=========================================================================*/
/***************************************************************************/
/*  low_level_init                                                         */
/*                                                                         */
/*  This function is invoked by the startup sequence after initializing    */
/*  the C stack, but before initializing the segments in RAM.              */
/*                                                                         */
/*  low_level_init() is invoked in the ARM state. The function gives the   */
/*  application a chance to perform early initializations of the hardware. */
/*  This function cannot rely on initialization of any static variables,   */
/*  because these have not yet been initialized in RAM.                    */
/*  low_level_init() is called from startup.s                              */
/***************************************************************************/
void low_level_init (void *reset_addr, void *return_addr) 
{
   extern uint8_t __ram_start;
   
   /*
    * The watchdog is enabled after processor reset. Disable it.
    */
   AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
   
   /*
    * Enable user reset: assertion length programmed to 1ms
    */
   AT91C_BASE_RSTC->RSTC_RMR = (RSTC_KEY | RSTC_URSTEN | (4 << 8));
   
   /*
    * Use 2 cycles for flash access.
    */
   AT91C_BASE_MC->MC0_FMR = MC_FWS_1FWS;
   AT91C_BASE_MC->MC1_FMR = MC_FWS_1FWS;
   /*
    * Disable all interrupts. Useful for debugging w/o target reset.
    */
   AT91C_BASE_AIC->AIC_EOICR = 0xFFFFFFFF;   
   AT91C_BASE_AIC->AIC_IDCR  = 0xFFFFFFFF;
   
   /*
    * Setup the CPU now...
    */
     
   /* Clock Generator - Programming Sequence follows below */

   /* 1. Enabling the Main Oscillator:
    * The main oscillator is enabled by setting the MOSCEN field in
    * the CKGR_MOR register. In some cases it may be advantageous
    * to define a start-up time. This can be achieved by writing
    * a value in the OSCOUNT field in the CKGR_MOR register.
    */
   AT91C_BASE_PMC->PMC_MOR = BOARD_OSCOUNT | AT91C_CKGR_MOSCEN;
   /* Once this register has been correctly configured, the user must wait for MOSCS field in the
    * PMC_SR register to be set. This can be done either by polling the status register or by waiting
    * the interrupt line to be raised if the associated interrupt to MOSCS has been enabled in
    * the PMC_IER register.
    */
   while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));

   /* 2. Checking the Main Oscillator Frequency (Optional)
    *
    */

   /* 3. Setting PLL and divider:
    * All parameters needed to configure PLL and the divider are located in the CKGR_PLLR register.
    * The DIV field is used to control divider itself. A value between 0 and 255 can be programmed.
    * Divider output is divider input divided by DIV parameter. By default DIV
    * parameter is set to 0 which means that divider is turned off.
    * The OUT field is used to select the PLL B output frequency range.
    * The MUL field is the PLL multiplier factor. This parameter can be programmed between 0
    * and 2047. If MUL is set to 0, PLL will be turned off, otherwise the PLL output frequency is
    * PLL input frequency multiplied by (MUL + 1).
    * The PLLCOUNT field specifies the number of slow clock cycles before LOCK bit is set in the
    * PMC_SR register after CKGR_PLLR register has been written.
    */
   AT91C_BASE_PMC->PMC_PLLR = BOARD_USBDIV | BOARD_CKGR_PLL | BOARD_PLLCOUNT
                              | BOARD_MUL | BOARD_DIV; 		/* Initialize PLL at 96MHz (96.109) and USB clock to 48MHz */

   /*
    * Once the PMC_PLL register has been written, the user must wait for the LOCK bit to be set
    * in the PMC_SR register. This can be done either by polling the status register or by waiting
    * the interrupt line to be raised if the associated interrupt to LOCK has been enabled in the
    * PMC_IER register. All parameters in CKGR_PLLR can be programmed in a single write
    * operation. If at some stage one of the following parameters, MUL, DIV is modified, LOCK bit
    * will go low to indicate that PLL is not ready yet. When PLL is locked, LOCK will be set again.
    * The user is constrained to wait for LOCK bit to be set before using the PLL output clock.
    * The USBDIV field is used to control the additional divider by 1, 2 or 4, which generates the USB clock(s).
    */
   while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK));


   /* 4. Selection of Master Clock and Processor Clock
    * The Master Clock and the Processor Clock are configurable via the PMC_MCKR register.
    * The CSS field is used to select the Master Clock divider source. By default, the selected
    * clock source is slow clock.
    * The PRES field is used to control the Master Clock prescaler. The user can choose between
    * different values (1, 2, 4, 8, 16, 32, 64). Master Clock output is prescaler input divided by
    * PRES parameter. By default, PRES parameter is set to 1 which means that master clock is
    * equal to slow clock.
    * Once the PMC_MCKR register has been written, the user must wait for the MCKRDY bit to
    * be set in the PMC_SR register. This can be done either by polling the status register or by
    * waiting for the interrupt line to be raised if the associated interrupt to MCKRDY has been
    * enabled in the PMC_IER register.
    * The PMC_MCKR register must not be programmed in a single write operation. The preferred
    * programming sequence for the PMC_MCKR register is as follows:
    * 		• If a new value for CSS field corresponds to PLL Clock,
    * 				– Program the PRES field in the PMC_MCKR register.
    * 				– Wait for the MCKRDY bit to be set in the PMC_SR register.
    * 				– Program the CSS field in the PMC_MCKR register.
    * 				– Wait for the MCKRDY bit to be set in the PMC_SR register.
    * 		• If a new value for CSS field corresponds to Main Clock or Slow Clock,
    * 				– Program the CSS field in the PMC_MCKR register.
    * 				– Wait for the MCKRDY bit to be set in the PMC_SR register.
    * 				– Program the PRES field in the PMC_MCKR register.
    * 				– Wait for the MCKRDY bit to be set in the PMC_SR register.
    * 				If at some stage one of the following parameters, CSS or PRES, is modified, the MCKRDY
    * 				bit will go low to indicate that the Master Clock and the Processor Clock are not ready yet.
    * 				The user must wait for MCKRDY bit to be set again before using the Master and Processor Clocks.
    */
   /* Wait for the master clock if it was already initialized */
   while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

   /* Switch to slow clock + prescaler */
   AT91C_BASE_PMC->PMC_MCKR = BOARD_PRESCALER;
   while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));

   /* Switch to fast clock + prescaler */
   AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK;
   while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));
   

   /*******************************************************************/
   /*  Warning, warning, warning, warning, warning, warning           */
   /*                                                                 */
   /*  This trick is used to check if we are running from Flash.      */
   /*  In this case __ram_start is set to the RAM start address       */
   /*  of the AT91SAM, which is 0x00200000.                           */
   /*                                                                 */
   /*  Only than, we can map the exception vectors from RAM to Flash. */
   /*******************************************************************/
   if ((uint32_t)&__ram_start == 0x00200000)
   {
      /* 
       * Setup the exception vectors to RAM
       *
       * NOTE: the exception vectors must be in RAM *before* the remap
       * in order to guarantee that the ARM core is provided with valid vectors
       * during the remap operation.
       */
       
      /* Setup the primary vector table in RAM */
      *(uint32_t volatile *)(&__ram_start + 0x00) = LDR_PC_PC;
      *(uint32_t volatile *)(&__ram_start + 0x04) = LDR_PC_PC;
      *(uint32_t volatile *)(&__ram_start + 0x08) = LDR_PC_PC;
      *(uint32_t volatile *)(&__ram_start + 0x0C) = LDR_PC_PC;
      *(uint32_t volatile *)(&__ram_start + 0x10) = LDR_PC_PC;
      *(uint32_t volatile *)(&__ram_start + 0x14) = MAGIC;
      *(uint32_t volatile *)(&__ram_start + 0x18) = LDR_PC_PC;
      *(uint32_t volatile *)(&__ram_start + 0x1C) = LDR_PC_PC;

      /* setup the secondary vector table in RAM */
      *(uint32_t volatile *)(&__ram_start + 0x20) = (uint32_t)reset_addr;
      *(uint32_t volatile *)(&__ram_start + 0x24) = (uint32_t)UndefHandler;
      *(uint32_t volatile *)(&__ram_start + 0x28) = (uint32_t)SWIHandler;
      *(uint32_t volatile *)(&__ram_start + 0x2C) = (uint32_t)PAbortHandler;
      *(uint32_t volatile *)(&__ram_start + 0x30) = (uint32_t)DAbortHandler;
      *(uint32_t volatile *)(&__ram_start + 0x34) = 0;
      *(uint32_t volatile *)(&__ram_start + 0x38) = (uint32_t)IRQHandler;
      *(uint32_t volatile *)(&__ram_start + 0x3C) = (uint32_t)FIQHandler;

      /* 
       * Check if the Memory Controller has been remapped already 
       */
      if (MAGIC != (*(uint32_t volatile *)0x14)) 
      {
         /* perform Memory Controller remapping */
         *(uint32_t volatile *)(0xFFFFFF00) = 1;
      }
   }      
   
} /* low_level_init */

/*** EOF ***/
