#include <io.h>
#include <signal.h>

#define LED_DIR P1DIR
#define LED_OUT P1OUT

#define LED_GREEN BIT6
#define LED_RED BIT0

void timer_init()
{
    // initialize the timer
    TACTL |= TASSEL_1 + ID_DIV1 + MC_1;
    TACCTL0 |= CCIE;
}

void led_init()
{
    // initialize leds (output) and button (input)
    LED_DIR |= LED_RED;
    LED_DIR |= LED_GREEN;
}

void init()
{
	// stop the watchdog
    WDTCTL = WDTPW + WDTHOLD; 
	
    led_init();
	timer_init();
	
    // enable interrupts globally
    _BIS_SR(GIE);

}

void timer_start()
{
	TACTL |= MC_1;
	TACCR0 = 32768;
}


int main()
{
	init();
	timer_start();
	
	_BIS_SR(LPM3_bits);
	
	return 0;
}

interrupt(TIMERA0_VECTOR) TIMERA0_ISR() {
    LED_OUT ^= LED_GREEN;
	LED_OUT ^= LED_RED;
}
