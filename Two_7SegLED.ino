// Arduino 7 segment display example software
// http://www.hacktronics.com/Tutorials/arduino-and-7-segment-led.html
// License: http://www.opensource.org/licenses/mit-license.php (Go crazy)
 
// Define the LED digit patters, from 0 - 9
// Note that these patterns are for common cathode displays
// For common anode displays, change the 1's to 0's and 0's to 1's
// 1 = LED on, 0 = LED off, in this order:
//                                    Arduino pin: 2,3,4,5,6,7,8

#include <avr/io.h>
#include <avr/interrupt.h>

/* Interrupt definitions */
volatile boolean ssFlag = false;  // start/stop flag
volatile int ones = 0;
volatile int tens = 0;

/* 7 Segment LED definitions */
byte digitOne[10]= {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x60, 0x0F, 0x00, 0x0C};
byte digitTwo[10]= {0x01, 0x4F, 0x12, 0x06, 0x4C, 0x24, 0x60, 0x0F, 0x00, 0x0C};
int dataPin = 4;
int latchPin = 6;
int clockPin = 7;
int buzzerPin = 8;

/* Arduino Setup */
void setup() {                
  Serial.begin(9600);  
  Serial.println("Initializing device");
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(9,0);
  pinMode(buzzerPin, OUTPUT);
  Serial.println("Initializing handler");
  attachInterrupt(0, buttonPress, RISING);
  attachInterrupt(1, incrementPress, RISING);
  Serial.println("Finished initialization");
}

/* Function Called with INT0 */
void buttonPress() {
  ssFlag = true;
}

/* Function Called with INT1 */
void incrementPress() {
  if (tens == 9 && ones == 9) {
    // Keep max value
  } else if (ones == 9) {
    tens += 1;
    ones = 0;
  } else {
    ones += 1;
  }
//  Serial.print(tens);
//  Serial.print(ones);
//  Serial.println("");
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ~digitTwo[tens]);
  shiftOut(dataPin, clockPin, MSBFIRST, ~digitOne[ones]);
  digitalWrite(latchPin, HIGH);
}

/* Main Loop */
void loop() {
  if (ssFlag) {
    Serial.println("Timer Started");
    // Count Down
    for (int kk=ones; kk>=0; kk--) {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, ~digitTwo[tens]);
      shiftOut(dataPin, clockPin, MSBFIRST, ~digitOne[kk]);
      digitalWrite(latchPin, HIGH);
      delay(1000);
    }
    for (int ii=tens-1; ii>=0; ii--) {
      for (int jj=9; jj>=0; jj--) {
        digitalWrite(latchPin, LOW);
        shiftOut(dataPin, clockPin, MSBFIRST, ~digitTwo[ii]);
        shiftOut(dataPin, clockPin, MSBFIRST, ~digitOne[jj]);
        digitalWrite(latchPin, HIGH);
        delay(1000);
      }
    }
    // Timer finished, do something (alarm)
    Serial.println("Timer Finished!"); 
    ssFlag = false;  // reset the start/stop flag
    buzz(buzzerPin, 4250, 500); // buzz the buzzer on pin at 4250Hz for 500 milliseconds
    delay(200);
    buzz(buzzerPin, 4250, 500); // buzz the buzzer on pin at 4250Hz for 500 milliseconds
    delay(200);
    buzz(buzzerPin, 4250, 500); // buzz the buzzer on pin at 4250Hz for 500 milliseconds
    delay(200);
  } else {
    delay(500);
  }
}

void buzz(int targetPin, long frequency, long length) {
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency*length/1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
  for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait againf or the calculated delay value
  }
}
