/* *********************************************
   File:        rs232.c
	 Title:       Ex4: servo motor control
	 Purpose:     LM3S9D96-DK
	              using UART1 as RS232 free running full duplex interface
	 Business:    HSA Elektrotechnik
	 Compiler:    MDK-ARM
	 Author/Date: Franz Haunstetter / 16.09.13
	 Comment:     new
	 Author/Date: Franz Haunstetter / 21.09.13
	 Comment:     added support for buffer
   *********************************************
*/

/* includes */
#include <lm3s9d96.h>		// hardware register names
#include <labex.h>			// defines baud rate

/* private macros */
#define DIVINT    SYSCLK / (16*BDR)
#define DIVFRAC   (((SYSCLK / (16.0*BDR)) - DIVINT)*64+0.5)

#define IPRIO			1

/* global buffers */
extern char c[], r_c, w_c;

//
// Initialize UART1 Rx/Tx on PD2/PD3 out of reset:
// UART is idle, FIFO disabled and empty.
//
void init_rs232()
{
	//
	// Enable the clock for GPIO and UART.
	//
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;		// supply port D
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1;		// supply UART1

	//
	// Configure the port pins to their new functions.
	//
	GPIO_PORTD_AFSEL_R |= BIT(2) | BIT(3);	// Rx and Tx only, first alternative in PD
	GPIO_PORTD_PCTL_R = GPIO_PCTL_PD2_U1RX | GPIO_PCTL_PD3_U1TX;
	GPIO_PORTD_DEN_R = BIT(2) | BIT(3);			// enable pins for digital signals

	//
	// UART is already disabled, thus ready to configure.
	//
	UART1_IBRD_R = DIVINT;									// Baud rate divisor
	UART1_FBRD_R = (unsigned int) DIVFRAC;
	
	UART1_LCRH_R = UART_LCRH_WLEN_8;				// line controls
	UART1_CTL_R |= UART_CTL_UARTEN;					// ready to run

	//
	// Enable receiver interrupts only.
	//
	UART1_IM_R = UART_IM_RXIM;
	NVIC_PRI1_R &= ~NVIC_PRI1_INT6_M;
	NVIC_PRI1_R |= IPRIO << 21;
	NVIC_EN0_R |= NVIC_EN0_INT6;
}

//
// Send 1 character, if the holding register is empty.
// Returns: 1 = char sent, 0 = transmitter busy.
//
int send_rs232( char c )
{
	if (UART1_FR_R & UART_FR_TXFE)
	{
		UART1_DR_R = (unsigned int) c;
		r_c++;
		return 1;
	}
	return 0;
}

//
// Send all characters, until it is empty.
//
void send_rs232_buffer( char* buf, int len )
{
	while (len > 0)
	{
		if (UART1_FR_R & UART_FR_TXFE)
		{
			UART1_DR_R = (unsigned int) *buf++;
			len--;
		}
	}
}

//
// UART interrupt service.
// Receive 1 character, if the holding register is full.
//
void RxISR()
{
	if (r_c == w_c)
		r_c = w_c = 0;
	
	if (w_c < TXTBUFL)
		c[w_c++] = UART1_DR_R;
}
