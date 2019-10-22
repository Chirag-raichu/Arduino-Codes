#include <DS1307.h>

#include <Wire.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

DS1307 rtc(SDA, SCL);

float t;

#define buz 11

int Hor;

int Min;

int Sec;




void setup()

{  

  Wire.begin();

  rtc.begin();

  Serial.begin(9600);

  pinMode(buz, OUTPUT);

  lcd.begin(16,2);     

  lcd.setCursor(0,0);

  lcd.print("DIYHacking.com");

  lcd.setCursor(0,1);

  lcd.print("Arduino Alarm ");

  // The following lines can be uncommented to set the date and time

  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY

  //rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)

  //rtc.setDate(1, 1, 2014);   // Set the date to January 1st, 2014

  delay(2000);

}




void loop()

{

  t = rtc.getTime();

  Hor = t.hour;

  Min = t.min;

  Sec = t.sec;

  lcd.setCursor(0,0);

  lcd.print("Time: ");

  lcd.print(rtc.getTimeStr());

 lcd.setCursor(0,1);

 lcd.print("Date: ");

 lcd.print(rtc.getDateStr());




 if( Hor == 11 &&  (Min == 32 || Min == 33)) //Comparing the current time with the Alarm time

{

Buzzer();

Buzzer();

lcd.clear();

lcd.print("Alarm ON");

lcd.setCursor(0,1);

lcd.print("Alarming");

Buzzer();

Buzzer();

} 

 delay(1000); 

}




void Buzzer()

{

digitalWrite(buz,HIGH);

delay(500);

digitalWrite(buz, LOW);

delay(500);

}
