#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);
byte addresses[][6] = {"0"};

struct package
{
  int id1=1;
 int id2=1;
 int id3=1;
 int id4=1; 
 int id5=1;
 
};


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
  myRadio.openWritingPipe( addresses[0]);

  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
   pinMode(A4,INPUT);
}
void loop()
{
  myRadio.write(&data, sizeof(data)); 
 
  Serial.print("\nPackage1:");
  Serial.print(data.id1);
  Serial.print("\tPackage2:");
  Serial.print(data.id2);
  Serial.print("\tPackage3:");
  Serial.print(data.id3);
  Serial.print("\tPackage4:");
  Serial.print(data.id4);
   Serial.print("\tPackage5:");
  Serial.print(data.id5);


 
 // Serial.println(data.temperature);
 // Serial.println(data.text);
int   potvalue1= analogRead(A0);
int   potvalue2= analogRead(A1);
int   potvalue3= analogRead(A2);
int   potvalue4= analogRead(A3);
int   potvalue5= analogRead(A4);

int servovalue1=map(potvalue1,0,1023,0,180);
int servovalue2=map(potvalue2,0,1023,0,180);
int servovalue3=map(potvalue3,0,1023,0,180);
int servovalue4=map(potvalue4,0,1023,0,180);
int servovalue5=map(potvalue5,0,1023,0,180);
//
 data.id1=servovalue1;
//  
 data.id2=servovalue2;
 data.id3=servovalue3;
 data.id4=servovalue4;
  data.id5=servovalue5;
// data.id1=10;
//data.id2=20;

  //data.temperature = data.temperature+0.1;
 // delay(1000);

}
