#define TRIGGER 5
#define ECHO    4
 
// NodeMCU Pin D1 > TRIGGER | Pin D2 > ECHO
 
void setup() {
 
  Serial.begin (9600);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
}
 
void loop() {
 
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  
   if (distance <= 30) {
    digitalWrite(D6, HIGH);
    
}
  else {
    digitalWrite(D6,LOW);
  }
  if (distance < 20) {
      digitalWrite(D7, HIGH);
      digitalWrite(D6, LOW);
      
      
} 
  else {
    digitalWrite(D7, LOW);
  }
  if (distance < 10) {
    digitalWrite(D8, HIGH);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
}
  else {
    digitalWrite(D8,LOW);
  }

 
  if (distance > 30 || distance <= 0){
    Serial.println("Out of range");
   
  }
  else {
    Serial.print(distance);
    Serial.println(" cm");
    
   
  }
  delay(500);
}
  
