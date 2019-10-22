int votes[4]={0,0,0,0};
char inbyte;        //LCD connections
  String pwd="VOTE";                         //Default Password
  String inpt="";
  boolean flag=false;
  boolean securitygranted=false;
  int i;
void setup() {
  pinMode(A2, INPUT_PULLUP);              //Setting pins as inputV
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(A6, INPUT_PULLUP);
  pinMode(A7,INPUT_PULLUP);
  Serial.begin(9600);                    //Begin serial communication
  Serial.println("PASSWORD TO BEGIN");
}
void loop() {
  while(flag==true)                     //Check flag for "V" command
  {
  if(digitalRead(A2) == LOW)
  {
  flag=false;
  votes[0]=votes[0]+1;
  }
  else if(digitalRead(A3) == LOW)
  {
  flag=false;
  votes[1]=votes[1]+1;
  }
  else if(digitalRead(A4) == LOW)
  {
  flag=false;
  votes[2]=votes[2]+1;
  }
  else if(digitalRead(A5) == LOW)
  {
  flag=false;
  votes[3]=votes[3]+1;
  }
  }
}
void serialEvent()
{
  while(Serial.available())
  {
   inbyte=(char)Serial.read();                   //Reading incoming character
   if(securitygranted==false)
   {
    inpt += inbyte;
    inbyte='\n';
   }
  }
  commandcheck();
}
void commandcheck()
{
  switch (securitygranted)                    //See for the security permissions
  {
    case false: {
      if(inpt==pwd)                          //Checking for password match
        {
          securitygranted=true;
          inbyte='\n';
          inpt="";
          Serial.println("ENTER VOTER ID FOR VOTING");
        }
       else if((inpt!=pwd)&&(inpt.length()>3))        //Mismatch
       {
          Serial.println(inpt);
          inpt="";
          inbyte='\n';
          Serial.println("WRONG PASSWORD");
          Serial.println("ENTER PASSWORD");
          Serial.end();
          delay(100);
          Serial.begin(9600);
       }
       break;
              }

   case true: {
      if(inbyte=='V')
        {
          flag=true;                             //Allowing voter to cast a single vote
          Serial.println("WAITING FOR VOTE");
          inbyte='\n';
        }
      else if(inbyte=='D')
        {
          for(i=0;i<=3;i++)                       //Displaying vote tally
          Serial.println(votes[i]);
          inbyte='\n';
        }
      else
        {Serial.println("VOTED");}
        break;
             }
  }
}
