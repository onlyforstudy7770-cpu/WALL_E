#include <IBusBM.h>
#include <ESP32Servo.h>


Servo myServo;
// const int receiverPin = 39;
const int servoPin = 22;

IBusBM IBus;  // Create IBus Object



const int rxPin = 36;

const int motor_Ain1 = 25;
const int motor_Ain2 = 26;
const int motor_PWMA = 32;

const int motor_PWMB = 33;
const int motor_Bin1 = 27;
const int motor_Bin2 = 35;

// for front ultrasonicsensor
const int trigPin = 24;
const int echoPin = 35;
//for back ultrasonicsensor
const int trigPin2 = 18;
const int echoPin2 = 34;



//lead screw motors
const int lead_motor_Ain1 = 13;
const int lead_motor_Ain2 = 23;
const int lead_motor_PWMA = 12;

const int lead_motor_PWMB = 19;
const int lead_motor_Bin1 = 17;
const int lead_motor_Bin2 = 15;

// The speed of sound is roughly 0.034 centimeters per microsecond
const float SOUND_SPEED = 0.034;



// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = IBus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}

// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

// Control Motor 
void move_front(int speed) {
  digitalWrite(motor_Ain1, HIGH);
  digitalWrite(motor_Ain2, LOW);

  digitalWrite(motor_Bin1, HIGH);
  digitalWrite(motor_Bin2, LOW);

  analogWrite(motor_PWMA, speed);
  analogWrite(motor_PWMB, speed);
}
void move_back(int speed) {
  digitalWrite(motor_Ain1, LOW);
  digitalWrite(motor_Ain2, HIGH);

  digitalWrite(motor_Bin1, LOW);
  digitalWrite(motor_Bin2, HIGH);

  analogWrite(motor_PWMA, speed);
  analogWrite(motor_PWMB, speed);
}
void move_left(int speed) {
  digitalWrite(motor_Ain1, HIGH);
  digitalWrite(motor_Ain2, LOW);

  digitalWrite(motor_Bin1, LOW);
  digitalWrite(motor_Bin2, LOW);

  analogWrite(motor_PWMA, speed);
  analogWrite(motor_PWMB, speed);
}
void move_right(int speed) {
  digitalWrite(motor_Ain1, LOW);
  digitalWrite(motor_Ain2, LOW);

  digitalWrite(motor_Bin1, HIGH);
  digitalWrite(motor_Bin2, LOW);

  analogWrite(motor_PWMA, speed);
  analogWrite(motor_PWMB, speed);
}
void stop() {
  digitalWrite(motor_Ain1, LOW);
  digitalWrite(motor_Ain2, LOW);

  digitalWrite(motor_Bin1, LOW);
  digitalWrite(motor_Bin2, LOW);

  analogWrite(motor_PWMA, 0);
  analogWrite(motor_PWMB, 0);
}


void setup() {

  Serial.begin(115200);
  // Start IBus on Serial2 (RX 2)
  IBus.begin(Serial2 , 0 , rxPin );

  pinMode(motor_Ain1, OUTPUT);
  pinMode(motor_Ain2, OUTPUT);

  pinMode(motor_PWMA, OUTPUT);
  pinMode(motor_PWMB, OUTPUT);

  pinMode(motor_Bin1, OUTPUT);
  pinMode(motor_Bin2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Attach the servo to GPIO 25
  myServo.attach(servoPin);



  pinMode(lead_motor_Ain1, OUTPUT);
  pinMode(lead_motor_Ain2, OUTPUT);

  pinMode(lead_motor_PWMA, OUTPUT);
  pinMode(lead_motor_PWMB, OUTPUT);

  pinMode(lead_motor_Bin1, OUTPUT);
  pinMode(lead_motor_Bin2, OUTPUT);


}

void loop() {

   // Get RC channel values
  int CH1 = readChannel(0, -255, 255, 0);
  int CH2 = readChannel(1, -255, 255, 0);
  int CH3 = readChannel(2, 0, 255, 0);
  bool CH5 = readSwitch(4, false);
  bool CH6 = readSwitch(5, false);

  // Print values to serial monitor for debugging
  Serial.print("CH1 = ");
  Serial.print(CH1);

  Serial.print(" CH2 = ");
  Serial.print(CH2);

  Serial.print(" Ch3 = ");
  Serial.print(CH3);

  Serial.print(" Ch5 = ");
  Serial.print(CH5);

  Serial.print(" Ch6 = ");
  Serial.println(CH6);


  bool run =  true ;
  // turn on the automatic stair climbing mode
  if (CH6)
  {
    float distance = distance_u_s_1();
    //  check if stairs is present or not
    if (distance >= 2 && distance <= 400) 
    {
      while (distance > 10) {
        stop();
        move_front(125);
        delay(1000);
        stop();

        distance = distance_u_s_1();
      }

      move_down_front_lead(255);
      move_down_front_lead(255);

      int time = millis();

      while (distance_u_s_1() - distance >= 25) {
        delay(100);
      }
      stop_front_lead();
      stop_back_lead();
      time = millis()- time;

      move_front(125);
      delay(1000);
      stop();
      delay(500);

      move_up_front_lead(255);
      delay( time);
      stop_front_lead();
      delay(500);

      move_front(125);
      delay(2000);
      stop();
      delay(500);

      move_up_back_lead(255);
      delay(time);
      stop_back_lead();
      delay(500);

      move_front(125);
      delay(500);
      stop();
      delay(500);
    }
  }


  else if (!CH6) 
  {
      // turn on stairs climbing down
  int speed2 = 125;
  long height = 0 ;
    if (run) {
      height = { distance_u_s_2() };
      digitalWrite(motor_Ain1, HIGH);
      digitalWrite(motor_Ain2, LOW);

      digitalWrite(motor_Bin1, LOW);
      digitalWrite(motor_Bin2, HIGH);

      analogWrite(motor_PWMA, speed2);
      analogWrite(motor_PWMB, speed2);
      delay(2000);
      stop();
      run = false;
    }

    move_back(125);
    while ((distance_u_s_2() - height < 4 )) {
      delay(500);
    }
    stop();

    move_down_back_lead(255);
    delay((distance_u_s_2() - height) * 75 + 100);
    stop_back_lead();

    move_back(125);
    delay(2000);
    stop();

    move_down_front_lead(255);
    delay((distance_u_s_2() - height) * 75 + 100);
    stop_front_lead();

    move_back(125);
    delay(500);
    stop();

    move_up_front_lead(255);
    move_up_back_lead(255);
    delay((distance_u_s_2() - height) * 75 + 100);
    stop_lead();
    delay(500);
  }
  else 
  {

    // to move front
    if (CH2 > 10 && (!(CH1 > 10  || CH1 < -10))) {
      move_front(CH2);
    }
    // to move back
    else if (CH2 < -10 && (!(CH1 > 10  || CH1 < -10))) {
      move_back(abs(CH2));
    }
    // to move left
    else if (CH1 >  10 && (!(CH2 > 10  || CH2 < -10))) {
     
      move_left(CH1);
    }
    //to move right
    else if (CH1 < 1480 &&  (!(CH2 > 10  || CH2 < -10))) {
      move_left(abs(CH2));
    } else {
      stop();
    }


    if (CH3 > 10 )
    {
      int angle = map(CH3, 0, 255, 0, 90);
      
      // Tell the servo to move to that angle
      myServo.write(angle);
    }
    run = true;
  }
  delay(200);
}

void move_up_front_lead(int speed) {
  digitalWrite(lead_motor_Ain1, HIGH);
  digitalWrite(lead_motor_Ain2, LOW);
  analogWrite(lead_motor_PWMA, speed);
}
void move_up_back_lead(int speed) {
  digitalWrite(lead_motor_Bin1, HIGH);
  digitalWrite(lead_motor_Bin2, LOW);
  analogWrite(lead_motor_PWMB, speed);
}
void move_down_front_lead(int speed) {
  digitalWrite(lead_motor_Ain1, LOW);
  digitalWrite(lead_motor_Ain2, HIGH);
  analogWrite(lead_motor_PWMA, speed);
}
void move_down_back_lead(int speed) {
  digitalWrite(lead_motor_Bin1, LOW);
  digitalWrite(lead_motor_Bin2, HIGH);
  analogWrite(lead_motor_PWMB, speed);
}

void stop_front_lead() {
  digitalWrite(lead_motor_Ain1, LOW);
  digitalWrite(lead_motor_Ain2, LOW);
  analogWrite(lead_motor_PWMA, 0);
}

void stop_back_lead() {
  digitalWrite(lead_motor_Bin1, LOW);
  digitalWrite(lead_motor_Bin2, LOW);
  analogWrite(lead_motor_PWMB, 0);
}
void stop_lead()
{
  digitalWrite(lead_motor_Bin1, LOW);
  digitalWrite(lead_motor_Bin2, LOW);
  analogWrite(lead_motor_PWMB, 0);

  digitalWrite(lead_motor_Ain1, LOW);
  digitalWrite(lead_motor_Ain2, LOW);
  analogWrite(lead_motor_PWMA, 0);

}

float distance_u_s_1() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);


  // We add a 30,000 microsecond timeout (which is roughly 5 meters).
  // This prevents the ESP32 from freezing if the sound never bounces back.
  long duration = pulseIn(echoPin, HIGH, 30000);
  return (duration * 0.0343 / 2);
}

float distance_u_s_2() {

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);


  // We add a 30,000 microsecond timeout (which is roughly 5 meters).
  // This prevents the ESP32 from freezing if the sound never bounces back.
  long duration = pulseIn(echoPin2, HIGH, 30000);
  return (duration * 0.0343 / 2);
}
