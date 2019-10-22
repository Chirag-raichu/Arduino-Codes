// Inclaudes the Servo library
#include <Servo.h>. 
// Defines Tirg and Echo pins of the Ultrasonic Sensor
const int trigPin = 10;
const int echoPin = 11;
// Variables for the duration and the distance
long duration;
int distance;
Servo myServo; // Creates a servo object for controlling the servo motor
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600);
  myServo.attach(12); // Defines on which pin is the servo motor attached
}
void loop() {                
  // rotates the servo motor from 15 to 165 degrees
  for(int i=5;i<=110;i++){  
  myServo.write(i);
  delay(500);
  distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree
  if(distance < 100){
  digitalWrite(13, HIGH);
  }
  else{
  digitalWrite(13, LOW);}
  Serial.print("\t\t\t");
  Serial.print("angle:");
  Serial.print(i); // Sends the current degree into the Serial Port
  Serial.print("\t\t\t"); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  Serial.print("distance:");
  Serial.print(distance); // Sends the distance value into the Serial Port
  Serial.print("\n");// Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }
  // Repeats the previous lines from 165 to 15 degrees
  for(int i=110;i>5;i--){  
  myServo.write(i);
  delay(500);
  distance = calculateDistance();
  if(distance < 100){
  digitalWrite(6, HIGH);
  }
  else{
  digitalWrite(6, LOW);}
  Serial.print("\t\t\t");
  Serial.print("angle:");
  Serial.print(i);
  Serial.print("\t\t\t");
  Serial.print("distance:");
  Serial.print(distance);
  Serial.print("\n");
  }  
}
// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2;
  return distance;
}
