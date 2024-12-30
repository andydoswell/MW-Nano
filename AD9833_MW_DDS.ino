/* Medium Wave DDS sythesiser for Arduino Nano
   by Doz 2024

   Details of schematic and build can be found on https://andydoz.blogspot.com 
   and on YouTube Doz Television Workshop @andydoz

   AD9833.h available from https://github.com/RobTillaart/AD9833
   Version 0.4.3 Used for this implementation
*/

#include "AD9833.h"
unsigned long frequency;
int channel;
int oldChannel;
int fCal=3; // use to calibrate the output frequency

AD9833 AD(10);  //  HW SPI, select pin 10
void setup() {
  SPI.begin();
  AD.begin();
    //  1000 Hz.
  AD.setWave(AD9833_SINE);
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT);
  pinMode(A7, INPUT);
  Serial.begin (9600);
}

void loop() {
  delay(100);
  // get channel binary (not so easy on a nano, due to A5-A7 issues, easy on an UNO, just use PORTC)
  channel = 0;
  if (!digitalRead(A0)) { channel += 1; }
  if (!digitalRead(A1)) { channel += 2; }
  if (!digitalRead(A2)) { channel += 4; }
  if (!digitalRead(A3)) { channel += 8; }
  if (!digitalRead(A4)) { channel += 16; }
  if (!digitalRead(A5)) { channel += 32; }
  if (analogRead(A6) < 500) { channel += 64; }
  if (analogRead(A7) < 500) { channel += 128; }
  if (oldChannel != channel) { setFrequency(); }
}

void setFrequency() {

  if (channel > 0 && channel <= 122) {  // calculate EU (9KHz) channels
    frequency = ((channel * 9) + 522) ;
    frequency *= 1000;
  }
  if (channel >= 129 && channel <= 247) {  // calculuate US (10KHz) channels
    frequency = (((channel - 128) * 10) + 520) ;
    frequency *=1000;
  }
  AD.setFrequency((frequency+fCal), 0);
  oldChannel = channel;
  Serial.println(frequency,10);
}

