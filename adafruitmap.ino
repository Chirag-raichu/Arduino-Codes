int a=0;
int duration, distance;
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Adafruit_MQTT_FONA.h>

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Adafruit_MQTT_FONA.h>


#include "Adafruit_MQTT.h"                                  // Adafruit MQTT library
#include "Adafruit_MQTT_Client.h"                           // Adafruit MQTT library

#include "ESP8266WiFi.h"                                    // ESP8266 library     

#include <TinyGPS++.h>                                      // Tiny GPS Plus Library
#include <SoftwareSerial.h>                                 // Software Serial Library so we can use Pins for communication with the GPS module


static const int RXPin = 4, TXPin = 5;                    // Ublox 6m GPS module to pins 1 and 2
static const uint32_t GPSBaud = 9600;                       // Ublox GPS default Baud Rate is 9600

TinyGPSPlus gps;                                            // Create an Instance of the TinyGPS++ object called gps
SoftwareSerial ss(RXPin, TXPin);                            // The serial connection to the GPS device

const double HOME_LAT = 11.076537;                          // Enter Your Latitude and Longitude here
const double HOME_LNG = 76.989558;                         // to track how far away the "Dog" is away from Home 
#define TRIGGER D6
#define ECHO D7

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Events@forge"
#define WLAN_PASS       "events@123"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "venkey4"
#define AIO_KEY         "ffc03238861748439567fc5e33c39e54"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'cm' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish cm = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/cm");
//Adafruit_MQTT_Publish Boxes = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Boxes");
//Adafruit_MQTT_Publish Motor = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Motor");
//Adafruit_MQTT_Publish bottlemissing = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/bottlemissing");
Adafruit_MQTT_Publish gpslat = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gpslat");
Adafruit_MQTT_Publish gpslng = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME "/feeds/gpslng");
Adafruit_MQTT_Publish gpslatlng = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME "/feeds/gpslatlng/csv");

// Setup a feed called 'onoff' for subscribing to changes.
//Adafruit_MQTT_Subscribe motor1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Motor");
//Adafruit_MQTT_Subscribe light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/L2");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void setup() {

  pinMode(TRIGGER,OUTPUT);
  pinMode(ECHO,INPUT);

  Serial.begin(115200);                                 // Setup Serial Comm for Serial Monitor @ 115200 baud
  WiFi.mode(WIFI_STA);                                  // Setup ESP8266 as a wifi station
  WiFi.disconnect();                                    // Disconnect if needed
  delay(100);                                           // short delay
  
  
  Serial.println("  Adafruit IO GPS");  
  Serial.println("      Tracker"); 
                                         // Pause X seconds  
    
  ss.begin(GPSBaud);                                    // Set Software Serial Comm Speed to 9600     
   
  Serial.print("Connecting to WiFi");
 

  WiFi.begin(WLAN_SSID, WLAN_PASS);                     // Start a WiFi connection and enter SSID and Password
      while (WiFi.status() != WL_CONNECTED) 
         {                                              // While waiting on wifi connection, display "..."
           delay(500);
           Serial.print(".");
          
         } 
           Serial.println("Connected");
  // Setup MQTT subscription for onoff feed.
//  mqtt.subscribe(&motor1);
  //mqtt.subscribe(&light2);
}

uint32_t x=D0;
void loop()
{
 sd();
 gg();
}

void sd()// ultrasonic sensor function
{
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1500)))

  // Now we can publish stuff!
  
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  a=100-distance;
  Serial.println(a);
  cm.publish(a);
  delay(3000);
}
void gg() //gpsfunction
{
  smartDelay(500);                                      // Update GPS data TinyGPS needs to be fed often
                                         // Run Procedure to connect to Adafruit IO MQTT  

  float Distance_To_Home;                               // variable to store Distance to Home  
  float GPSlat = (gps.location.lat());                  // variable to store latitude
  float GPSlng = (gps.location.lng());                  // variable to store longitude
  float GPSalt = (gps.altitude.feet());                 // variable to store altitude  
  Distance_To_Home = (unsigned long)TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),HOME_LAT, HOME_LNG);  //Query Tiny GPS to Calculate Distance to Home
    

  Serial.println(F("           GPS Tracking"));
  Serial.println("---------------------");
    

  Serial.print("GPS Lat: ");
  Serial.println(gps.location.lat(), 6);               // Display latitude to 6 decimal points
  Serial.print("GPS Lon: ");
  Serial.println(gps.location.lng(), 6);               // Display longitude to 6 decimal points
  Serial.print("Distance: ");
  Serial.println(Distance_To_Home);                    // Distance to Home measured in Meters    
  
  MQTT_connect();
  // ********************** Combine Data to send to Adafruit IO *********************************
  // Here we need to combine Speed, Latitude, Longitude, Altitude into a string variable buffer to send to Adafruit    
                                                            
            char gpsbuffer[30];                         // Combine Latitude, Longitude, Altitude into a buffer of size X
            char *p = gpsbuffer;                        // Create a buffer to store GPS information to upload to Adafruit IO                       

            dtostrf(Distance_To_Home, 3, 4, p);         // Convert Distance to Home to a String Variable and add it to the buffer
            p += strlen(p);
            p[0] = ','; p++;                      
            
            dtostrf(GPSlat, 3, 6, p);                   // Convert GPSlat(latitude) to a String variable and add it to the buffer
            p += strlen(p);
            p[0] = ','; p++;
                                                            
            dtostrf(GPSlng, 3, 6, p);                   // Convert GPSlng(longitude) to a String variable and add it to the buffer
            p += strlen(p);
            p[0] = ','; p++;  
                                                            
            dtostrf(GPSalt, 2, 1, p);                   // Convert GPSalt(altimeter) to a String variable and add it to the buffer
            p += strlen(p);
                                                                        
            p[0] = 0;                                   // null terminate, end of buffer array

            if ((GPSlng != 0) && (GPSlat != 0))         // If GPS longitude or latitude do not equal zero then Publish
              {
              Serial.println("Sending GPS Data ");     
              
              gpslatlng.publish(gpsbuffer);             // publish Combined Data to Adafruit IO
              Serial.println(gpsbuffer);  
              }
            
            gpslng.publish(GPSlng,6);                   // Publish the GPS longitude to Adafruit IO                 
            
            if (! gpslat.publish(GPSlat,6))             // Publish the GPS latitude to Adafruit IO
               {
                 Serial.println(F("Failed"));          // If it failed to publish, print Failed
               } else 
                  {
                   Serial.println(gpsbuffer);
                   Serial.println(F("Data Sent!"));                   
                   }  
       
        delay(1000);         
    
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
  
  // Wait a bit before scanning again
  Serial.print("Pausing...");
  
  smartDelay(500);                                      // Feed TinyGPS constantly
  delay(1000);
}


// **************** Smart delay - used to feed TinyGPS ****************

static void smartDelay(unsigned long ms)                 
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}







// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
