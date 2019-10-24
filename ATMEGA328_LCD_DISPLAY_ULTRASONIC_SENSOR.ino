#include <LiquidCrystal.h>
int Contrast=35;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  
 
const int trigPin = 9;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;

 void setup()
 {
     analogWrite(6,Contrast);
     lcd.begin(16, 2);
     pinMode(trigPin, OUTPUT);
     pinMode(echoPin, INPUT);
     Serial.begin(9600);
  }
     void loop()
 { digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;
distanceInch = duration*0.0133/2;

lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
lcd.print("Distance: ");// Prints string "Distance" on the LCD
Serial.print("distance:");
Serial.println(distanceCm);
lcd.print(distanceCm); // Prints the distance value from the sensor
lcd.print("  cm");
delay(10);
lcd.setCursor(0,1);
lcd.print("VENKATESH");
delay(10);
 }
