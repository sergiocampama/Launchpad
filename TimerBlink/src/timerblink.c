// Sergio Campama 2011
// http://kaipi.me
//
// Made for the TI MSP430 LaunchPad
// http://ti.com/launchpadwiki
//
// Released under the Beerware License
// http://en.wikipedia.org/wiki/Beerware

#include <io.h>
#include <signal.h>

//Defines to help understand what is attached to each port
#define LED_DIR P1DIR
#define LED_OUT P1OUT

//P1.0 and P1.6 are the RED and GREEN LEDs respectively
#define LED_RED BIT0
#define LED_GREEN BIT6

void timer_init()
{
	//Select ACLK, TimerA counter divisor in 1
    TACTL |= TASSEL_1 + ID_DIV1; 

    //Enables the CCR0 interrupt
	TACCTL0 |= CCIE; 
}

void led_init()
{
	//Set the direction as output
    LED_DIR |= LED_RED; 
    LED_DIR |= LED_GREEN;
}

void init()
{
	//Stop the watchdog
    WDTCTL = WDTPW + WDTHOLD; 
	
	//Inits the different parts
    led_init();
	timer_init();
	
    //Enable global interrupts
    _BIS_SR(GIE);

}

void timer_start()
{
	TACTL |= MC_1;//Sets the TimerA count mode in UP, and thus starting the Timer
	
	//Since the crystal attached to LFXT is 32.768 kHz, 
	//when the counter reaches this value, exactly 1 second should have passed
	//This could be off a little because of the clock skew (20 ppm)
	TACCR0 = 32768;
}


int main()
{
	init();
	timer_start();
	
	//We enter the LPM3 Low Power Mode, which only leaves ACLK running
	//This way, the CPU will be off for 1 second, and it only will work on interrupts
	_BIS_SR(LPM3_bits);
	
	return 0;
}

//CCR0 timer interrupt, which toggles the LEDs
interrupt(TIMERA0_VECTOR) TIMERA0_ISR() {
    LED_OUT ^= LED_GREEN;
	LED_OUT ^= LED_RED;
}
