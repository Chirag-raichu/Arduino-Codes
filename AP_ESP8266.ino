#include<dht.h>
dht DHT;
#define DHT11_PIN 7
void setup(){
  Serial.begin(9600);
}
void loop(){
  int chk=DHT.read11(DHT11_PIN);
  Serial.print("Temperature=");
  Serial.println(DHT.Temperature);
  Serial.print("Humidity=");
  Serial.println(DHT.Humidity);
  delay(1000);
}

