#include <IBusBM.h>

IBusBM ibus; // Create IBus Object

const int rxPin = 16;
const int motor_Ain1 = 25;
const int motor_Ain2 = 26;
const int motor_PWMA = 32;

const int motor_PWMB = 33;
const int motor_Bin1 = 27;
const int motor_Bin2 = 35;

// for front ultrasonicsensor
const int trigPin = 24;
const int echoPin = 36;

// The speed of sound is roughly 0.034 centimeters per microsecond
const float SOUND_SPEED = 0.034;

void setup() {

  // Serial.begin(115200);
  // Start IBus on Serial2 (RX pin 16, TX pin 17)
  IBus.begin(Serial2, 1, rxPin, 17);

  pinMode(motor_Ain1, OUTPUT);
  pinMode(motor_Ain2, OUTPUT);

  pinMode(motor_PWMA, OUTPUT);
  pinMode(motor_PWMB, OUTPUT);

  pinMode(motor_Bin1, OUTPUT);
  pinMode(motor_Bin2, OUTPUT);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
}

void loop() {
  // Tell the library to read the incoming serial data
  IBus.loop();
  // Read channel 0 (Channel 1 on transmitter)
  // iBus channels are 0-indexed, so 0=CH1, 1=CH2, etc.
  int ch1 = ibus.readChannel(0); 
  int ch2 = ibus.readChannel(1);
  int ch3 = ibus.readChannel(2); // Often throttle
  // int ch4 = ibus.readChannel(3);
  int ch5 = ibus.readChannel(4);
  // int ch6 = ibus.readChannel(5);
  
  // to move front 
  if (ch1 > 1520 && ( ch2 < 1520 || ch2 > 1480))
  {
    int speed = map(ch1, 1520, 2000, 0, 255);
    move_front(speed);
  }
  // to move back
  else if ( ch1 < 1480 && ( ch2 < 1520 || ch2 > 1480))
  {
    int speed = map(ch1, 1480, 1000, 0, 255);
    move_back(speed);
  }
  // to move left
  else if ( ch2 > 1520 && ( ch1 < 1520 || ch1 > 1480))
  {
    int speed = map(ch2, 1520, 2000 , 0, 255);
    move_left(speed);
  }
  //to move right
 else if ( ch2 < 1480  && ( ch1 < 1520 || ch1 > 1480))
  {
    int speed = map(ch2, 1480 , 1000 , 0, 255);
    move_left(speed);
  }
  
  // turn on the automatic stair climbing mode
  if (ch5 > 1600)
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // We add a 30,000 microsecond timeout (which is roughly 5 meters). 
  // This prevents the ESP32 from freezing if the sound never bounces back.
  long duration = pulseIn(echoPin, HIGH, 30000);
  while ( duration != 0)
  {

  }
    
  }

  delay(200);
}
void move_front( int speed)
{
    digitalWrite(motor_Ain1, HIGH);
    digitalWrite(motor_Ain2, LOW);

    digitalWrite(motor_Bin1, HIGH);
    digitalWrite(motor_Bin2, LOW);

    analogWrite(motor_PWMA, speed); 
    analogWrite(motor_PWMB, speed); 
}
void move_back(int speed)
{
digitalWrite(motor_Ain1, LOW);
    digitalWrite(motor_Ain2, HIGH);

    digitalWrite(motor_Bin1, LOW);
    digitalWrite(motor_Bin2, HIGH);

    analogWrite(motor_PWMA, speed); 
    analogWrite(motor_PWMB, speed); 
}
void move_left (int speed)
{
  digitalWrite(motor_Ain1, HIGH);
  digitalWrite(motor_Ain2, LOW);

    digitalWrite(motor_Bin1, LOW);
    digitalWrite(motor_Bin2, LOW);

    analogWrite(motor_PWMA, speed); 
    analogWrite(motor_PWMB, speed); 
}
void move_right (int speed)
{
  digitalWrite(motor_Ain1, LOW);
  digitalWrite(motor_Ain2, LOW);

    digitalWrite(motor_Bin1, HIGH);
    digitalWrite(motor_Bin2, LOW);

    analogWrite(motor_PWMA, speed); 
    analogWrite(motor_PWMB, speed); 
}

