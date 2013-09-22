/* *********************************************
   File:        ADC.c
	 Title:       Ex4: servo motor control
	 Purpose:     LM3S9D96-DK
	              read the ADC0 from port PB4 / 10 bit
	 Business:    HSA Elektrotechnik
	 Compiler:    MDK-ARM
	 Author/Date: Franz Haunstetter / 20.09.13
	 Comment:     new
   *********************************************
*/

/* includes */
#include <lm3s9d96.h>		// hardware register and bit names
#include <labex.h>

/* private macros */

/* private function prototypes */

/* global variables and buffers */


//
// Initialize AD Channel 10 on PB4 to ADC0/SS0 out of reset:
// Pin is digitally disabled, ADC sequencer 0 has maximum priority,
// and is disabled; conversion is to be triggered by software,
// all SSMUX, SSCTL fields are clear.
//
int init_ADC()
{
	//
	// Enable the clock for GPIO and ADC.
	//
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;		// supply port B
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC0;		// supply ADC0

	//
	// Configure the port pins to their new functions.
	//
	GPIO_PORTB_AFSEL_R |= BIT(4);						// AIN10 connected to PB
	GPIO_PORTB_AMSEL_R |= BIT(4);						// for analog signals

	//
	// Configure the sample sequencer 0 to capture analog data.
	//
	ADC0_SSMUX0_R = 10;											// first sample is AIN10
	ADC0_SSCTL0_R =													// properties
		ADC_SSCTL0_IE0 | ADC_SSCTL0_END0			// 1st sample
		/* | ADC_SSCTL0_IE1 | ADC_SSCTL0_END1 | ADC_SSCTL0_TS1*/; // 2nd sample
	ADC0_ACTSS_R = ADC_ACTSS_ASEN0;					// enable sequencer 0 for ADC0

	//
	// Enable only end-of-conversion interrupt for sequencer 0.
	//
	return 1;
}

//
// Place driver capability and shutdown routines here.
//
unsigned long readADC()
{
	ADC0_PSSI_R = ADC_PSSI_SS0;
	while (!(ADC0_RIS_R & BIT(0)));
	ADC0_ISC_R |= BIT(0);
	return ADC0_SSFIFO0_R & 0x000003ff;
}

//
// Place interrupt service routines here.
// Import their names into the Interrupt Vector Table, in Startup.s
//
void ADC_ISR()
{
}
