/* *********************************************
   File:        PWM.c
	 Title:       Ex4: servo motor control
	 Purpose:     LM3S9D96-DK
	              PWM0 control for a servo motor
	 Business:    HSA Elektrotechnik
	 Compiler:    MDK-ARM
	 Author/Date: Franz Haunstetter / 21.09.13
	 Comment:     new
   *********************************************
*/

/* includes */
#include <lm3s9d96.h>		// hardware register names
#include <labex.h>			// defines baud rate

/* global buffers */

//
// Initialize PWM generator 0 on PD0 out of reset:
//
void init_PWM()
{
	//
	// Enable the clock for GPIO and PWM.
	//
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;		// supply port D
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM;			// supply all PWM

	//
	// Configure the port pins to their new functions.
	//
	GPIO_PORTD_AFSEL_R |= BIT(0);						// PWM0 at PD (is generator 0, channel A)
	GPIO_PORTD_PCTL_R |= GPIO_PCTL_PD0_PWM0;
	GPIO_PORTD_DEN_R |= BIT(0);							// enable pins for digital signals
	
	//
	//
	//
	PWM_0_GENA_R = PWM_0_GENA_ACTLOAD_ZERO | PWM_0_GENA_ACTCMPAD_ONE;
	PWM_0_GENB_R = 0;
	PWM_0_LOAD_R = 50000-1;									// set period to 20 ms (80MHz/(32*50000))
	PWM_0_CMPA_R = 2500-1;									// 1 ms pulses to begin with
	PWM_0_CMPB_R = 0;
	PWM_0_CTL_R = PWM_0_CTL_ENABLE;					// start counting
	PWM_ENABLE_R |= PWM_ENABLE_PWM0EN;			// enable output signals
}
