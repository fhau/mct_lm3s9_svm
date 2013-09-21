/* *********************************************
   File:        GPIO.c
	 Title:       Ex4: servo motor control
	 Purpose:     LM3S9D96-DK
	              GPIO port driver (initialize only)
	 Business:    HSA Elektrotechnik
	 Compiler:    MDK-ARM
	 Author/Date: Franz Haunstetter / 16.09.13
	 Comment:     new
   *********************************************
*/

/* includes */
#include <lm3s9d96.h>		// hardware register names
#include <labex.h>

/* private macros */

//
// Initialize GPIO out of reset:
//
void init_IO()
{
	volatile unsigned long ulIOBits;
	
	//
	// Enable the clock for IO devices.
	//
	// GPIO ports:
	SYSCTL_RCGC2_R |=
		SYSCTL_RCGC2_GPIOF									// LEDs
		| SYSCTL_RCGC2_GPIOJ;								// button

	//
	// Do a dummy read to insert a few cycles after enabling the peripheral.
	//
	ulIOBits = SYSCTL_RCGC2_R;

	//
	// Enable the GPIO pins for the LEDs (PF2, PF3) as output, and
	// enable the GPIO pins for digital function.
	//
	GPIO_PORTF_DIR_R = BIT(3) | BIT(2);
	GPIO_PORTF_DEN_R = BIT(3) | BIT(2);

	//
	// Enable the GPIO pin for the BUTTON (PJ7).  Set the direction as input, and
	// enable the GPIO pin for digital function.
	//
	GPIO_PORTJ_DEN_R = BIT(7);
}
