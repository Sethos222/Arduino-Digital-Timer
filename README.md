Arduino-Digital-Timer
=====================
by: David M Hughes
Digital timer to count down and make audible "beeps" when times is up.  Uses Arduino UNO, two 7-seg LEDs, two buttons and a speaker.

05/1/2013: Initial version.


Why make a simple timer?
------------------------
My tea timer started to die on me, and I did not want to buy something I could make myself.

Hardware:
---------
 * 1 Arduino UNO
 * 2 shift registers
 * 2 7-segment LEDs
 * 2 buttons
 * 1 speaker
 * 13 resistors

Functions:
----------
Press the first button to start or stop the time counter.
Press the second button to increase the time, in 1 minute increments.

Future goals:
-------------
Add a third 7-seg LED to count the seconds.
Look into a power budget, to see what kind of battery is needed and how long it will last.
Include a reset option, possibly hold down the start/stop button to reset time completely.
