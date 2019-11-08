
#include <SPI.h>  
#include "RF24.h"
#include<Servo.h>

Servo servo1,servo2,servo3,servo4,servo5;
RF24 myRadio (7, 8);

struct package
{ 
  int id1=0;
  int id2=1;
  int id3=1;
  int id4=1;
  int id5=1;
 // float temperature = 0.0;
  //char  text[100] ="empty";
};

byte addresses[][6] = {"0"};



typedef struct package Package;
Package data;
void setup()
{
  Serial.begin(115200);
  delay(1000);

  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ;
  myRadio.openReadingPipe(1, addresses[0]);
   
  myRadio.startListening();
  servo1.attach(5);
  servo2.attach(6);
  servo3.attach(3);
  servo4.attach(9);
  servo5.attach(10);
}


void loop()  
{
 
  
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
      Serial.print("\nServo1:");
      Serial.print(data.id1);
      Serial.print("\tServo2:");
      Serial.print(data.id2);
      Serial.print("\tServo3:");
      Serial.print(data.id3);
      Serial.print("\tServo4:");
      Serial.print(data.id4);
      Serial.print("\tServo5:");
      Serial.print(data.id5);
   
    servo1.write(data.id1);
     servo2.write(data.id2);
     servo3.write(data.id3);
     servo4.write(data.id4);
      servo5.write(data.id5);
    }
 }
