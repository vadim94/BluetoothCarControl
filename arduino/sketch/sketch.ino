/*
	Arduino control with bluetooth HC-05

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
int Led1=3;
int Led2=4;
int BTData; // the data given from Computer

void setup() {
  // put your setup code here, to run once:
  BT.begin(9600);
  BT.println("Bluetooth On please press 1 or 0 blink LED ..");
  pinMode(Led1,OUTPUT);
  pinMode(Led2,OUTPUT);
}

void loop() {
if (BT.available()){
BTData=BT.read();
if(BTData=='1'){   // if number 1 pressed ....
digitalWrite(Led1,1);
digitalWrite(Led2,0);
BT.println("LED  On D13 ON ! ");
}
if (BTData=='0'){// if number 0 pressed ....
digitalWrite(Led1,0);
digitalWrite(Led2,1);
BT.println("LED  On D13 Off ! ");
}
}
delay(100);// prepare for next data ...
}
