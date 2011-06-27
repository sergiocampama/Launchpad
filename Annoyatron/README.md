## Annoyatron

This is a simple app that produces a beep at random intervals, between 20 an 899 seconds. It assumes a 32.768 kHz crystal soldered to the Launchpad, and a piezo electric buzzer connected to some pin. Be sure to define PIEZO to the BITX in the port it is connected. It is connected to the green LED in this example, to see the results.

It also works on the LPM3 Low Power Mode, so the CPU only wakes up to beep and then goes to sleep. LPM3 only leaves the ACLK on, while all the other peripherals are off.

This project is heavily based on Brian2898's [forum post](http://www.43oh.com/forum/viewtopic.php?f=10&t=1159) on 43oh.com