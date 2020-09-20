#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7,8); // CE, CSN         
const byte address[6] = "00001";     //Byte of array representing the address. This is the address where we will send the data. This should be same on the receiving side.
int button_pin = 4;
int button_pin1 = 3;
boolean button_state = 0;
boolean button_state1 = 0;
void setup() {
pinMode(button_pin, INPUT);
pinMode(button_pin1, INPUT);
radio.begin();                  //Starting the Wireless communication
radio.openWritingPipe(address); //Setting the address where we will send the data
radio.setPALevel(RF24_PA_MIN);  //You can set it as minimum or maximum depending on the distance between the transmitter and receiver.
radio.stopListening();   
Serial.begin(115200);//This sets the module as transmitter
}
void loop()
{
button_state = digitalRead(button_pin);
button_state1 = digitalRead(button_pin1);
radio.write(&button_state, sizeof(button_state));
radio.write(&button_state1, sizeof(button_state1));
Serial.println(button_state);
Serial.print(button_state1);
//Sending the message to receiver 
delay(1000);
}
