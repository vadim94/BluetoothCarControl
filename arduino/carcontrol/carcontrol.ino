/*
	Car control via bluetooth

	It allows to change state of two leds.
        If bluetooth receives "0" - one led turns on,
        if "1" - the other.
        
	The circuit (controller - Arduino Nano AT256):
	* D3 - led 1
        * D4 - led 2
        * D10 - bluetooth TXD
        * D11 - bluetooth RXD
        * 5V - bluetooth VCC
        * GND - bluetooth GND, led 1, led 2
	* list the components attached to each output

	Created 17 Nov 2015
	vadim94@live.com
*/

#include <SoftwareSerial.h>

SoftwareSerial BT(10, 11);

const int ENA = 3;
const int IN1 = 4;
const int IN2 = 5;
const int IN3 = 6;
const int IN4 = 7;
const int ENB = 9;

const int speedMin = 138;
const int speedMax = 255;
const int speedStep = (speedMax - speedMin) / 9;

int currentL = 0; // left, A pin
int currentR = 0; // right, B pin

int currentSpeed = speedMin;
int currentAngle = 0; // forward

void setup() {
  // put your setup code here, to run once:
  BT.begin(9600);
  BT.println("Car is successfully connected");
  
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
}

void loop() {
  updateCarState();
}

void updateCarState()
{
  if (BT.available() == 5)
  {
    BT.println("updateCarState() > BT is available");
    
    char command = BT.read();
    int value = BT.read() - 48;
    int digit1 = BT.read() - 48;
    int digit2 = BT.read() - 48;
    int digit3 = BT.read() - 48;
    
    switch(command)
    {
      case 'g': // go
        BT.println("updateCarState() > 'g' received");
        updateSpeed(value);
        updateAngle(digit1, digit2, digit3);
        break;
      case 's': // stop
        BT.println("updateCarState() > 's' received");
      default:
        setStopState();
    }
    refreshRightLeftSpeed();
    setSpeedValues();
  }
}

void updateSpeed(int value)
{
  currentSpeed = speedMin + value * speedStep;
  
  BT.print("updateSpeed() > received speed ");
  BT.println(value);
  BT.print("updateSpeed() > current speed "); 
  BT.println(currentSpeed);
  
  // check for correct work
  if (currentSpeed > speedMax) currentSpeed = speedMax;
  else if (currentSpeed < speedMin) currentSpeed = speedMin;
}

void updateAngle(int digit1, int digit2, int digit3)
{
  currentAngle = digit1 * 1 + digit2 * 10 + digit3 * 100;
  
  BT.print("updateAngle() > digits are ");
  BT.print(digit1);
  BT.print(digit2);
  BT.println(digit3);
  BT.print("updateAngle() > angle is ");
  BT.println(currentAngle);
}

void setStopState()
{
  currentSpeed = 0;
  currentAngle = 0;
  
  BT.println("setStopState() > success");
}

void setForwardState()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  
  BT.println("setForwardState() > success");
}

void setBackwardState()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  
  BT.println("setBackwardState() > success");
}

void setSpeedValues()
{
  analogWrite(ENA, currentL);
  analogWrite(ENB, currentR);
  
  BT.print("setSpeedValues() > A: ");
  BT.print(currentL);
  BT.print(" B: ");
  BT.println(currentR);
}

void refreshRightLeftSpeed()
{
  // 0 -> Forward
  // 180 -> Backward
  // 0-180 -> Left
  // 180-360 -> Right
  
  if (currentAngle <= 90)
  {
    setForwardState();
    currentR = currentSpeed;
    currentL = speedMin + (((currentSpeed - speedMin)) * ((100 * (90 - currentAngle)) / 90)) / 100;
  }
  else if (currentAngle <= 180)
  {
    setBackwardState();
    currentR = currentSpeed;
    currentL = speedMin + (((currentSpeed - speedMin)) * ((100 * (currentAngle - 90)) / 90)) / 100;
  }
  else if (currentAngle < 270)
  {
    setBackwardState();
    currentL = currentSpeed;
    currentR = speedMin + (((currentSpeed - speedMin)) * ((100 * (270 - currentAngle)) / 90)) / 100;
  }
  else
  {
    setForwardState();
    currentL = currentSpeed;
    currentR = speedMin + (((currentSpeed - speedMin)) * ((100 * (currentAngle - 270)) / 90)) / 100;
  }
  
  if (currentL < speedMin && currentL != 0) currentL = speedMin;
  else if (currentL > speedMax) currentL = speedMax;
  if (currentR < speedMin && currentR != 0) currentR = speedMin;
  else if (currentR > speedMax) currentR = speedMax;
}
