int b=0;
int a=0;
int c=0;
int motor1_state;
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define LED1 D6
#define LED2 D7

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "AISH"
#define WLAN_PASS       "password"

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

// Setup a feed called 'Bottle' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish Bottle = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Bottle");
Adafruit_MQTT_Publish Boxes = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Boxes");
Adafruit_MQTT_Publish Motor = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Motor");
Adafruit_MQTT_Publish bottlemissing = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/bottlemissing");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe motor1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Motor");
Adafruit_MQTT_Subscribe light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/L2");

/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&motor1);
  mqtt.subscribe(&light2);
}

uint32_t x=D0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(2500))) {
    if (subscription == &motor1) {
      Serial.print(F("Got: "));
      Serial.println((char *)motor1.lastread);
      int motor1_state=atoi((char *)motor1.lastread);
      Serial.println(motor1_state);
      //digitalWrite(LED1,motor1_state);
    }
    if (subscription == &light2) {
      Serial.print(F("Got: "));
      Serial.println((char *)light2.lastread);
      int light2_state=atoi((char *)light2.lastread);
      digitalWrite(LED2,light2_state);
    }
  }

  // Now we can publish stuff!
if(motor1_state==1){a=0;b=0;}else{
  if(digitalRead(x)==0){ 
    a++;delay(0);
    Serial.print(F("\nNumber of bottle="));
    Serial.print(a);
    Bottle.publish(a);
    if(a==5){
      b++;
      Serial.print(F("\nNumber of boxes="));
      Serial.print(b);
      Boxes.publish(b);
      a = 0;}}
  else{
    c++;
    Serial.print("\nBottle missing=");
    Serial.print(c);
    if(c==10){
    Motor.publish(0);
    c = 0;
    }}
  
  
  //Serial.print(b);
 // Serial.print("...");
  //if (Bottle.publish(b)) {
   //Serial.println(F("fILLED!"));
 // } else {
  // Serial.println(F("Failed"));
  //}
 // .publish(a);
  

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}}

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
