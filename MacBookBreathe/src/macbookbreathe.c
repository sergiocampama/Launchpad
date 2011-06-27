// Sergio Campama 2011
// http://kaipi.me
//
// Made for the TI MSP430 LaunchPad
// http://ti.com/launchpadwiki
//
//
// Heavily based on http://osx-launchpad.blogspot.com/2010/11/breathing-led-effect-with-launchpad.html

#include <msp430.h>
//We need the definition of uint16_t. I like this ones better than int and long, because
//They are more verbose as to how much bits they have
#include <inttypes.h>

//In uniarch there is no more signal.h to sugar coat the interrupts definition, so we do it here
#define interrupt(x) void __attribute__((interrupt (x))) 

//Defines to help understand what is attached to each port
#define LED_DIR P1DIR
#define LED_OUT P1OUT

#define PWM_DIR P1DIR
#define PWM_SEL P1SEL
#define PWM_LED BIT6

//P1.0 is the RED LED
//The green is used for PWM
#define LED_RED BIT0

//This is the same curve used in the original project
const uint8_t curve[] = {
    1,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     1,     1,     1,     1,     1,
    1,     1,     1,     2,     2,     2,     2,     2,
    2,     2,     3,     3,     3,     3,     3,     3,
    4,     4,     4,     4,     4,     5,     5,     5,
    5,     6,     6,     6,     6,     7,     7,     7,
    8,     8,     8,     8,     9,     9,     9,    10,
   10,    10,    11,    11,    11,    12,    12,    13,
   13,    13,    14,    14,    15,    15,    15,    16,
   16,    17,    17,    18,    18,    18,    19,    19,
   20,    20,    21,    21,    22,    22,    23,    23,
   24,    24,    25,    25,    26,    26,    27,    27,
   28,    29,    29,    30,    30,    31,    31,    32,
   33,    33,    34,    34,    35,    36,    36,    37,
   38,    38,    39,    39,    40,    41,    41,    42,
   43,    43,    44,    45,    46,    46,    47,    48,
   48,    49,    50,    50,    51,    52,    53,    53,
   54,    55,    56,    56,    57,    58,    59,    59,
   60,    61,    62,    62,    63,    64,    65,    66,
   66,    67,    68,    69,    70,    70,    71,    72,
   73,    74,    75,    75,    76,    77,    78,    79,
   80,    80,    81,    82,    83,    84,    85,    86,
   87,    87,    88,    89,    90,    91,    92,    93,
   94,    95,    95,    96,    97,    98,    99,   100,
  101,   102,   103,   104,   105,   106,   106,   107,
  108,   109,   110,   111,   112,   113,   114,   115,
  116,   117,   118,   119,   120,   121,   122,   122,
  123,   124,   125,   126,   127,   128,   129,   130,
  131,   132,   133,   134,   135,   136,   137,   138,
  139,   140,   141,   142,   143,   144,   145,   146,
  147,   148,   149,   150,   151,   152,   153,   154,
  155,   156
};


void timer_init()
{
	//Select ACLK, TimerA counter divisor in 1
	TACTL |= TASSEL_1; 

	//Enables the CCR0 interrupt
	TACCTL1 |= CCIE + OUTMOD_7; 
	
	PWM_DIR |= PWM_LED;
	PWM_SEL |= PWM_LED;
	
}

void led_init()
{
	//Set the direction as output
	LED_DIR |= LED_RED; 
	
	LED_OUT &= ~(LED_RED);
	
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
	//With this value, and using the external 32.768 crystal, the frequency of the PWM
	//will be 200 Hz, which is enough to not feel the flickering
	TACCR0 = 164;
	
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

uint16_t count_in_cycle = 0;

//CCR1 timer interrupt, which breathes the LED
interrupt(TIMERA1_VECTOR) TIMERA1_ISR() {
	TACCTL1 &= ~CCIFG;
	
	
	uint8_t new_ccr = 1;
	
	if (count_in_cycle < 500)
	{
		new_ccr = curve[count_in_cycle >> 1];
	}
	
	else if (count_in_cycle < 1000)
	{
		new_ccr = curve[(999 - count_in_cycle) >> 1];
	}
	else
		count_in_cycle = 0;
	
	count_in_cycle++;
	//This is to set the TACCR1 register after the TAR has passed its value, 
	//so we dont get 2 interrupt calls in the same cycle
	while (TAR <= new_ccr);
	TACCR1 = new_ccr;
}
