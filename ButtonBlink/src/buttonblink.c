// Sergio Campama 2011
// http://kaipi.me
//
// Made for the TI MSP430 LaunchPad
// http://ti.com/launchpadwiki
//
// Released under the Beerware License
// http://en.wikipedia.org/wiki/Beerware

#include <msp430.h>
//In uniarch there is no more signal.h to sugar coat the interrupts definition, so we do it here
#define interrupt(x) void __attribute__((interrupt (x)))

//Defines to help understand what is attached to each port
#define LED_DIR P1DIR
#define LED_OUT P1OUT

//Interrupt Enable
#define BUTTON_IE P1IE
#define BUTTON_IES P1IES
#define BUTTON_IFG P1IFG

//P1.0 and P1.6 are the RED and GREEN LEDs respectively
#define LED_RED BIT0
#define LED_GREEN BIT6

//P1.3 is the button
#define BUTTON BIT3

void button_init()
{
	BUTTON_IE |= BUTTON; //We activate the button interrupt
	BUTTON_IES |= BUTTON; //High to low edge
	BUTTON_IFG &= ~BUTTON;
}

void led_init()
{
	//Set the direction as output
	LED_DIR |= LED_RED + LED_GREEN; 
	
	//Start with the red led on.
	LED_OUT &= ~LED_GREEN;
	LED_OUT |= LED_RED;
}

void init()
{
	//Stop the watchdog
	WDTCTL = WDTPW + WDTHOLD; 

	//Inits the different parts
	led_init();
	button_init();

	//Enable global interrupts
	_BIS_SR(GIE);

}


int main()
{
	init();
	
	//We enter the LPM4 Low Power Mode, which leaves nothing running
	//This way, the CPU will be off until the button is pressed
	_BIS_SR(LPM4_bits);

	return 0;
}

//Port 1 interrupt, which toggles the LEDs
interrupt(PORT1_VECTOR) PORT1_ISR() {
	
	if (BUTTON & BUTTON_IFG)
	{
		LED_OUT ^= LED_GREEN + LED_RED;
		BUTTON_IFG &= ~BUTTON;
	}
	
}
