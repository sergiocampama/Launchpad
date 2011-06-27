## MacBook Breathe

This is a simple app that replicates the "breathing" of the MacBook led when the computer is asleep. It relies on having a 32.768 kHz crystal soldered to the Launchpad.

This project is heavily based in the one by Javi, which can be found in http://osx-launchpad.blogspot.com/2010/11/breathing-led-effect-with-launchpad.html

It also works on the LPM3 Low Power Mode, so the CPU only wakes up to "breath" the LEDs and then goes to sleep. LPM3 only leaves the ACLK on, while all the other peripherals are off.
