// Sergio Campama 2011
// http://kaipi.me
//
// Made for the TI MSP430 LaunchPad
// http://ti.com/launchpadwiki
//
// Heavily based on http://www.43oh.com/forum/viewtopic.php?f=10&t=1159

#include <msp430.h>
#include <inttypes.h>

//In uniarch there is no more signal.h to sugar coat the interrupts definition, so we do it here
#define interrupt(x) void __attribute__((interrupt (x))) 

//Defines to help understand what is attached to each port
#define LED_DIR P1DIR
#define LED_OUT P1OUT

//P1.0 is the red LED
#define LED_RED BIT0

//Piezo defines
//Here it is defined as BIT6 to see the results on the LED, but should be changed to the bit where it is 
//connected the piezo
#define PIEZO BIT6
#define PIEZO_OUT P1OUT
#define PIEZO_DIR P1DIR

void timer_init()
{
	//Select ACLK, TimerA counter divisor in 1
	TACTL |= TASSEL_1 + ID_0; 

	//Enables the CCR0 interrupt
	TACCTL0 |= CCIE; 
}

void piezo_init()
{
	//Set the direction as output
	PIEZO_DIR |= PIEZO; 
	
	//Piezo starts off
	PIEZO_OUT &= ~PIEZO;
	
}

void led_init()
{
	//Set the direction as output
	LED_DIR |= LED_RED; 
	
	//Red led starts off
	LED_OUT &= ~LED_RED;
	
}

void init()
{
	//Stop the watchdog
	WDTCTL = WDTPW + WDTHOLD; 

	//Inits the different parts
	led_init();
	piezo_init();
	timer_init();

	//Enable global interrupts
	_BIS_SR(GIE);

}

void timer_start()
{
	//Since the crystal attached to LFXT is 32.768 kHz, 
	//when the counter reaches this value, exactly 1 second should have passed
	//The clock skew is 20 ppm, which at this frequency, should be +-.65 Hz
	TACCR0 = 32768;
	
	TACTL |= MC_1;//Sets the TimerA count mode in UP, and thus starting the Timer
	
}


int main()
{
	init();
	timer_start();
	
	//We enter the LPM3 Low Power Mode, which only leaves ACLK running
	//This way, the CPU will be off for 1 second, toggle the LED, and turn off for another second
	_BIS_SR(LPM3_bits);
	
	return 0;
}

uint64_t random_seed = 65738265;

//This function delivers a uniform random variable between 20 and 899
uint16_t random_uniform()
{
	random_seed = (16807*random_seed) % (2147483647LLU);
	return random_seed % 880 + 20;
}

uint16_t actual_delay = 0;
uint16_t delay_s = 10;

//CCR0 timer interrupt, which toggles the LEDs
interrupt(TIMERA0_VECTOR) TIMERA0_ISR() {
	
	//Turn off the piezo
	PIEZO_OUT &= ~PIEZO;
	//We set the normal time on CCR0 so the interrupt
	//interval is still 1 second
	TACCR0 = 32768;
	
	if (++actual_delay >= delay_s)
	{
		//Turn on the piezo
		PIEZO_OUT |= PIEZO;
		
		//Reset the delays
		actual_delay = 0;
		delay_s = random_uniform();
		
		//The next interrupt will be half a second earlier
		//We turned on the piezo and will wait for the next interrupt to turn it of
		//This way, when the piezo is on, we are still in low power mode
		//Then te interrupt comes and turns it off
		//Adjusting this value will change the piezo duration
		TACCR0 = 32768 >> 1;
	}
	
}
