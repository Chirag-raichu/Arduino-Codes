int proximity=0;
int duration;
int distance;

void setup() {
  //Serial Port
  Serial.begin (9600);
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
}

void loop() {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
 
  /*if (distance >= 45 || distance <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }*/
 
  proximity=map(distance, 0, 45, 8, 0);
  //Serial.println(proximity);
 
  if (proximity <= 0){
    proximity=0;
  }
  
  else if (proximity >= 5 && proximity <= 6){
     tone(tonePin,5000, 200);
  }
  else if (proximity >= 7 && proximity <= 8){
    tone(tonePin, 1000, 200);
  }
  shiftOut(dataPin, clockPin, MSBFIRST, possible_patterns[proximity]);
  digitalWrite(latchPin, HIGH);

 
  delay(600);
  noTone(tonePin);
}
