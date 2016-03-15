/*****f*        examples/Timer/Timer_Delay/main.c
  * Usage:
  *       Execute using MPLAB IDE
  * Summary:
  *         Simple loop to time blinking of LED using __delay32() function
  *
  * Inputs:
  *       none
  * Outputs:
  *       Blinking LED

  * Dependencies:
  *      Sask_Init()
  *		__delay32()
  * Notes:
  *		Demonstrates simple timer using __delay32() function.
  *		First of a series of examples that includes Timer_Wait and Timer_Interrupt

  * ToDo:
  *     none

  * Originator:
  *     Chris Bore, BORES Signal processing, chris@bores.com, www.bores.com

  * History:
  *      Version 1.00     13/03/2013
  *****/


#include <p33FJ256GP506.h>
#include <libpic30.h>
#include "..\h\sask.h"

_FGS(GWRP_OFF & GCP_OFF);
_FOSCSEL(FNOSC_FRC);
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
_FWDT(FWDTEN_OFF);

int main(void)
{
	unsigned short int led_state;
	static unsigned short int const LED_ON =SASK_LED_ON;
	static unsigned short int const LED_OFF = SASK_LED_OFF;

	float clock_frequency, cycle_time, delay_time;
	unsigned long delay_cycles;

	/* Configure Oscillator to operate the device at 40MHz.
	 * Fosc= Fin*M/(N1*N2), Fcy=Fosc/2
	 * Fosc= 7.37M*40/(2*2)=80Mhz for 7.37M input clock */
	 
	PLLFBD=41;				/* M=39	*/
	CLKDIVbits.PLLPOST=0;		/* N1=2	*/
	CLKDIVbits.PLLPRE=0;		/* N2=2	*/
	OSCTUN=0;			

	clock_frequency = 40e6;
	cycle_time = 1 / clock_frequency;
	delay_time = 0.5;
	delay_cycles = delay_time / cycle_time;

	__builtin_write_OSCCONH(0x01);		/*	Initiate Clock Switch to FRC with PLL*/
	__builtin_write_OSCCONL(0x01);
	while (OSCCONbits.COSC != 0b01);	/*	Wait for Clock switch to occur	*/
	while(!OSCCONbits.LOCK);

	SASKInit();
	 
	led_state = LED_OFF;
	SWITCH_S1_TRIS = 1;
	while(1)
	{	
		
		YELLOW_LED=LED_OFF;
		if( led_state == LED_OFF )
		{
			led_state = LED_ON;
		}
		else if( led_state == LED_ON )
		{
			led_state = LED_OFF;
		}
		

	
		RED_LED = led_state;

		__delay32( delay_cycles );

	}			
}
