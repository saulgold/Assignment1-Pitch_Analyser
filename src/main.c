/*******        Assignment1\src\main.c
  * Usage:
  *       Execute using MPLAB IDE
  * Summary:
  *         analyses input signal and finds the peak pitch
  *
  * Inputs:
  *       Audio
  * Outputs:
  *       LEDs

  * Dependencies:
  *      Sask_Init()
  *		__delay32()
  * Notes:
  *			

  * ToDo:
  *     none

  * Originator:
  *     Saul Goldblatt, saulgold@gmail.com

  * History:
  *      Version 1.00     15/06/2016
  *****/


#include <p33FJ256GP506.h>
#include <libpic30.h>
#include "..\h\sask.h"
#include "..\h\ADCChannelDrv.h"
#include "..\h\OCPWMDrv.h"
#define FRAME_SIZE 				ADC_BUFFER_SIZE
#define FILTER_LENGTH			16

	_FGS(GWRP_OFF & GCP_OFF);
	_FOSCSEL(FNOSC_FRC);
	_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);
	_FWDT(FWDTEN_OFF);
	
/*initialise types for ADCChannel functions*/
int		adcBuffer		[ ADC_CHANNEL_DMA_BUFSIZE ] 	__attribute__((space(dma)));	
int		ocPWMBuffer		[ OCPWM_DMA_BUFSIZE ]		__attribute__((space(dma)));

ADCChannelHandle adcChannelHandle;
OCPWMHandle 	ocPWMHandle;

ADCChannelHandle *pADCChannelHandle; 
OCPWMHandle 	*pOCPWMHandle 		= &ocPWMHandle;

int 	AudioIn	[ FRAME_SIZE ], AudioWorkSpace[ FRAME_SIZE ], AudioOut [ FRAME_SIZE ];

int main(void)
{
	ADCChannelInit(pADCChannelHandle,adcBuffer);
	OCPWMInit		(pOCPWMHandle,ocPWMBuffer);
	ADCChannelStart	(pADCChannelHandle);
	OCPWMStart		(pOCPWMHandle);	
		
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
	int i;
	while(1)
	{	
		while(ADCChannelIsBusy(pADCChannelHandle));
		ADCChannelRead	(pADCChannelHandle,AudioIn,FRAME_SIZE);	

	
		while(OCPWMIsBusy(pOCPWMHandle));	
		OCPWMWrite (pOCPWMHandle,AudioIn,FRAME_SIZE);
		
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
