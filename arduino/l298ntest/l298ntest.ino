/*
	Test of L298N driver
        
	The circuit (controller - Arduino Nano AT256):
	* D3 - ENB
        * D4 - IN3
        * D5 - IN4

	Created 17 Nov 2015
	vadim94@live.com
*/

int IN3 = 5;
int IN4 = 4;
int ENB = 3;
void setup()
{
 pinMode (ENB, OUTPUT); 
 pinMode (IN3, OUTPUT);
 pinMode (IN4, OUTPUT);
}
void loop()
{
  // Engine is ready
  digitalWrite (IN3, HIGH);
  digitalWrite (IN4, LOW);
  // Started
  analogWrite(ENB,55);
  delay(2000);
  analogWrite(ENB,105);
  delay(2000);
  analogWrite(ENB,255);
  delay(2000);
  // Stop
  analogWrite(ENB,0);
  delay(5000);
}

