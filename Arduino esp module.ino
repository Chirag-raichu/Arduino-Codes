/*   Arduino IOT - Temperature (oC) and Humidity (%) on the web
  *Use the DHT-22 sensor to read temperature and humidity values
  *Send these values to www.thingSpeak.com with the ESP8266 serial Wifi module
  Dev: Michalis Vasilakis // Date:23/2/2016 // Update: 25/2/2015 // Ver. 1.3
  More info: http://www.ardumotive.com/iot-wifi-temp-and-humidity.html
  Tip: open the serial monitor for debugging                    */

//Libraires
#include <stdlib.h>
#include <DHT.h>

/*------------------------DHT SENSOR------------------------*/
#define DHTPIN 2        // DHT data pin connected to Arduino pin 2
#define DHTTYPE DHT11     // DHT 22 (or AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize the DHT sensor
/*----------------------------------------------------------*/

/*-----------------ESP8266 Serial WiFi Module---------------*/
#define SSID "chirag"     // "SSID-WiFiname" 
#define PASS "asdfghjkl"       // "password"
#define IP "184.106.153.149"// thingspeak.com ip
String msg = "42PVUTMY0UUYAEFN"; //change it with your key...
/*-----------------------------------------------------------*/

//Variables
float temp;
int hum;
String tempC;
int error;
void setup()
{
  Serial.begin(9600); //or use default 115200.
  
  Serial.println("AT");
  delay(5000);
  if(Serial.find("OK")){
    connectWiFi();
  }
}

void loop(){
  //Read temperature and humidity values from DHT sensor:
  start: //label 
  error=0;
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  char buffer[10];
  // there is a useful c function called dtostrf() which will convert a float to a char array 
  //so it can then be printed easily.  The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  tempC = dtostrf(temp, 4, 1, buffer); 

  updateTemp();
  //Resend if transmission is not completed 
  if (error==1){
    goto start; //go to label "start"
  }
  
  delay(1000); //Update every 1 hour
}

void updateTemp(){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = msg ;
  cmd += "&field1=";    //field 1 for temperature
  cmd += tempC;
  cmd += "&field2=";  //field 2 for humidity
  cmd += String(hum);
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }
  else{
    Serial.println("AT+CIPCLOSE");
    //Resend...
    error=1;
  }
}

 
boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}
