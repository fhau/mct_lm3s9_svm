/* *********************************************
   File:        system.c
	 Title:       Ex4: servo motor control
	 Purpose:     LM3S9D96-DK
	              system hardware driver
	 Business:    HSA Elektrotechnik
	 Compiler:    MDK-ARM
	 Author/Date: Franz Haunstetter / 16.09.13
	 Comment:     new
	 Author/Date: Franz Haunstetter / 21.09.13
	 Comment:     rs232 line output and analog polling
	 Author/Date: Franz Haunstetter / 22.09.13
	 Comment:     motor control via analog input
   *********************************************
*/

/* includes */
#include <lm3s9d96.h>		// hardware register names
#include <labex.h>
#include <string.h>
#include <stdio.h>

/* private macros */

/* private function prototypes */
void send_rs232_buffer( char* buf, int len ); // rs232.c
int rec_rs232( char* buf );
unsigned long readADC(void);			// ADC.c

//
// Initialize system devices out of reset:
//
void init_sys()
{
	//
	// Initialize the System Tick Timer for 10 Hz (20 ticks/s), then
	// clear the counting element
	//
	NVIC_ST_CURRENT_R = NVIC_ST_RELOAD_R = SYSCLK / (10 * 2);
}

//
// Run the timer together with interrupts.
//
void SysTickRun(void)
{
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE | NVIC_ST_CTRL_INTEN;
}

//
// System Tick Timer interrupt service (max. priority).
//
void SysTickISR()
{
	unsigned long ulShadow;
	char txt[15];
	
	//
	// Print text buffer with analog reading to rs232 console.
	//
	sprintf( txt, "\x1b[2J\x1b[H%4u", (unsigned int) (ulShadow = readADC()) );
//	sprintf( txt, "%4u", readADC() );
	send_rs232_buffer( txt, strlen( txt ) );
	
	PWM_0_CMPA_R = (2500 * ulShadow / 1024) + 2500;

	//
	// Toggle the LED each time the counter reaches 0.
	//
	GPIO_PORTF_DATA_R ^= BIT(3);
	
	//
	// To avoid jitter at small division values remove the ST pending flag
	// and leave the others unchanged (must write 0 to write-only bits).
	//
  ulShadow = NVIC_INT_CTRL_R & ~NVIC_INT_CTRL_UNPEND_SV | NVIC_INT_CTRL_PENDSTCLR;
	NVIC_INT_CTRL_R = ulShadow;
}
