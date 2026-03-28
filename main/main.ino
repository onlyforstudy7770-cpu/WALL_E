#include <IBusBM.h>

IBusBM ibus; // Create IBus Object

const int rxPin = 16;

void setup() {

  // Serial.begin(115200);
  // Start IBus on Serial2 (RX pin 16, TX pin 17)
  IBus.begin(Serial2, 1, rxPin, 17);
}

void loop() {
  // Tell the library to read the incoming serial data
  IBus.loop();
  // Read channel 0 (Channel 1 on transmitter)
  // iBus channels are 0-indexed, so 0=CH1, 1=CH2, etc.
  int ch1 = ibus.readChannel(0); 
  int ch2 = ibus.readChannel(1);
  // int ch3 = ibus.readChannel(2); // Often throttle
  int ch4 = ibus.readChannel(3);
  int ch5 = ibus.readChannel(4);
  int ch6 = ibus.readChannel(5);

  Serial.print("Ch1: ");
  Serial.print(ch1);
  Serial.print(" | Ch2: ");
  Serial.print(ch2);
  Serial.println();
  
  delay(20);
}
