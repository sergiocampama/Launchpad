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
#define PIEZO_DIR P1DIR
#define PIEZO_SEL P1SEL
#define PIEZO_OUT P1OUT

void timer_init()
{
	//Set DCO to 1 MHz
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;
	
	//Select ACLK, TimerA counter divisor in 1
	TACTL |= TASSEL_1 + ID_0; 

	//Enables the CCR0 interrupt
	TACCTL0 |= CCIE; 
}

void piezo_init()
{
	//Set the direction as output
	PIEZO_DIR |= PIEZO; 
	
	//Set the piezo bit to PWM output of TA1, which coincides with the green
	//LED in the Launchpad
	PIEZO_SEL |= PIEZO;
	
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
	
	//Sets the TimerA count mode in UP, and thus starting the Timer
	TACTL |= MC_1;
	
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


//This function delivers a uniform random variable between 20 and 899
uint64_t random_seed = 65738265;

uint16_t random_uniform()
{
	random_seed = (16807*random_seed) % (2147483647LLU);
	return random_seed % 880 + 20;
}

//actual_delay counts the number of seconds passed since the last
//beep, while delay_s is time where the beep will sound, referenced
//to the last beep also
uint16_t actual_delay = 0;
uint16_t delay_s = 10;

//playing is an indicator that the piezo is currently on
//piezo cycles counts the number of cycles it has been playing,
//referenced to the PWM frequency
uint8_t playing = 0;
uint16_t piezo_cycles = 0;

//CCR0 timer interrupt, which toggles the LEDs
interrupt(TIMERA0_VECTOR) TIMERA0_ISR() {
	
	//if we are playing and the time has not yet passed to stop
	//playing, return. To understand this value, read first the long comment just before
	//setting TACCR0 and TACCR1 inside the 'if'. Ok, so we want to play for half a second,
	//which in a crystal of 32768 Hz, is 16384. This interrupt will be called every 64 cycles,
	//value calculated below. So, we will achieve 16384 cycles (half a second)
	//after 16384/64 interrupt calls, which equals 256.
	//Remember, if you want a frequency of X, you need to set TACCR0 to 32768/X.
	//Then if you want to play for T seconds, set this comparison to 32768*T/TACCR0
	if (playing && piezo_cycles++ < 256) return;
	
	//after it has finished the playing time, stop playing and reset the cycles for the next
	//beep
	playing = 0;
	piezo_cycles = 0;
	
	//We turn the PIEZO off just in case
	PIEZO_OUT &= ~PIEZO;
	
	//This turns off the pwm in cc1
	TACCTL1 &= ~OUTMOD_7; 
	
	//We set the normal time on CCR0 so the interrupt
	//interval is still 1 second
	TACCR0 = 32768;
	
	if (++actual_delay >= delay_s)
	{
		//The time has come to beep
		//we calculate the new delay and reset actual_delay
		actual_delay = 0;
		delay_s = random_uniform();
		
		//Having a crystal of 32768 Hz, and wanting to play at a frequency of
		//500 hz, we set TACCR0 to 32768/500, rounded down to the nearest even
		//which is 64. We then set the PWM duty cycle in TACCR1, in this case to
		//50% -> 64*0.5 = 32. The duty cycle should control the volume of the piezo,
		//but when tested on my headphones, it didn't make much difference, but
		//with a higher value I think it sounds better. Keep in mind, if its higher 
		//than TACCR0, nothing will sound
		TACCR0 = 64;//32768 >> 1;
		TACCR1 = 32;
		
		//We set the PWM output mode to toggle set/reset mode
		//See the MSP430x2xx User's Guide for more information
		//in the TimerA module
		TACCTL1 |= OUTMOD_7; 
		
		//Finally, we tell the system that it is playing
		playing = 1;
		
	}
	
}
