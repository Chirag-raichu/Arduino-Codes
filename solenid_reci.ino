#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
boolean button_state1 = 0;
int led_pin = 4;
void setup() {
pinMode(led_pin , OUTPUT);
pinMode(5 , OUTPUT);
Serial.begin(115200);
radio.begin();
radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
radio.startListening();              //This sets the module as receiver
}
void loop()
{
if (radio.available())              //Looking for the data.
{
 radio.read(&button_state, sizeof(button_state));    //Reading the data
 radio.read(&button_state1, sizeof(button_state1));
}
buttonstate();
buttonstate1();
}
void buttonstate()
{
 if(button_state == LOW)
{
digitalWrite(led_pin ,HIGH);
Serial.println(button_state);
}
else
{
digitalWrite(led_pin , LOW);
Serial.println(button_state);
}
delay(1000);
}
void buttonstate1()
{
 if(button_state1 == LOW)
{
digitalWrite(5,HIGH);
Serial.print(button_state1);
}
else
{
digitalWrite(5, LOW);
Serial.print(button_state1);
}
}
