# indicator
This is an indicator object class, providing a generic way to use any MCU's GPIO output pin to indicate a binary state.

Popular use cases could be 

* controlling the built-in LED 
* or just a pin which could be probed by an oscilloscope for instance.

The indicator is blinking by default (@ 2.5 Hz, on & off time is 200ms each) and can be set to OFF and ON.
