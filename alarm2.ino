// Include header files
#include <Wire.h>
#include <LiquidCrystal.h>

// LCD pin definitions
#define RS 9
#define E 10
#define D4 8
#define D5 7
#define D6 6
#define D7 5

LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// Interrupt 0 is hardware pin 4 (digital pin 2)
int btnSet = 0;

// Interrupt 1 is hardware pin 5 (digital pin 3)
int btnSel = 1;

// Interrupt state
int togBtnSet = false;
int togBtnSel = false;

// Time and date variables
int tmpHour = 0;
int tmpMinute = 0;
int tmpDate = 0;
int tmpMonth = 0;
int tmpYear = 0;
int tmpDay = 0;
int tmpSecond = 0;

int counterVal = 0;

// Variable to keep track of where we are in the "menu"
int myMenu[6]; // 0=Hour, 1=Minutes, 2=date, 3=MOnth, 4=Year, 5=DOW
int menuCounter = 0;

// A array of the weekday
char* days[] = { "NA", "Mon", "Tue", "Wed", "Thu", "Fre", "Sat", "Sun" };

void setup() {
  // Interrupt declaration, execute increaseValue/nextItem function
  // when btnXXX is RISING
  attachInterrupt(btnSet, increaseValue, RISING);
  attachInterrupt(btnSel, nextItem, RISING);
  Wire.begin();
  lcd.begin(16,2);
  showWelcome();
}

// Interrupt function
void increaseValue()
{
  // Variables
  static unsigned long lastInterruptTime = 0;
  // Making a timestamp
  unsigned long interruptTime = millis();

  // If timestamp - lastInterruptTime is greater than 200
  if (interruptTime - lastInterruptTime > 200)
  {
    // Toggle the variable
    togBtnSet = !togBtnSet;
    // Increase the counterVal by 1
    counterVal++;
  }
  // Setting lastInterruptTime equal to the timestamp
  // so we know we moved on
  lastInterruptTime = interruptTime; 
}

// Next menuItem Interrupt function
void nextItem()
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  if (interruptTime - lastInterruptTime > 200)
  {
    togBtnSel = !togBtnSel;
    // Increase the menu counter so we move to next item
    menuCounter++;   
    // Placing the counterVal in the menucounters array position
    myMenu[menuCounter] = counterVal;
    // Reset counterVal, now we start at 0 on the next menuItem
    counterVal = 0;
  }
  lastInterruptTime = interruptTime;
}

// Function that convert decimal numbers to binary
byte decToBCD(byte val)
{
  return ((val/10*16) + (val));
}

// Short welcome message, now we know everything is OK
void showWelcome()
{
  lcd.setCursor(2,0);
  lcd.print("Hello world.");
  lcd.setCursor(3,1);
  lcd.print("I'm alive.");
  delay(500);
  lcd.clear();
}

// Funcion to set the hour
void setHour()
{
  lcd.setCursor(0,0);
  lcd.print("Set hour.       "); 
 // Checks if interrupt has occured = button pressed
 if (togBtnSet)
 {
  // Update array value with counterVal
  myMenu[menuCounter] = counterVal;
  lcd.setCursor(7,1);
  // Print the new value
  lcd.print(myMenu[menuCounter]); lcd.print("  ");
 }
 else
 {
  // Update array value with counterVal
  myMenu[menuCounter] = counterVal;
  lcd.setCursor(7,1);
  // Print the new value
  lcd.print(myMenu[menuCounter]); lcd.print("  ");
 }
}

// Function to set minutes
void setMinute()
{
  lcd.setCursor(0,0);
  lcd.print("Set minute.     ");
  if (togBtnSet)
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }
  else
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }
}

// Function to set date
void setDate()
{
  lcd.setCursor(0,0);
  lcd.print("Set date.       ");
  if (togBtnSet)
  {
    myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }
  else
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }  
}

// Function to set month
void setMonth()
{
  lcd.setCursor(0,0);
  lcd.print("Set month.      ");
  if (togBtnSet)
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }
  else
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }  
}

// Function to set year
void setYear()
{
  lcd.setCursor(0,0);
  lcd.print("Set year.       ");
  if (togBtnSet)
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }
  else
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }  
}

// Function to set the day of week
void setDOW()
{
  lcd.setCursor(0,0);
  lcd.print("Set day (1=mon).");
  if (togBtnSet)
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }
  else
  {
  myMenu[menuCounter] = counterVal;
    lcd.setCursor(7,1);
    lcd.print(myMenu[menuCounter]); lcd.print("  ");
  }  
}

// Write the data to the RTC
void writeRTC()
{
  Wire.beginTransmission(0x68);
  Wire.write(0); // Start address
  Wire.write(0x00); // seconds
  Wire.write(decToBCD(myMenu[1])); // convert tmpMinutes to BCD and write them
  Wire.write(decToBCD(myMenu[0]));  // convert tmpHour to BCD and write them
  Wire.write(decToBCD(myMenu[5]));  // convert tmpDay to BCD and write them
  Wire.write(decToBCD(myMenu[2]));  // convert tmpDate to BCD and write them
  Wire.write(decToBCD(myMenu[3]));  // convert tmpMonth to BCD and write them
  Wire.write(decToBCD(myMenu[4]));  // convert tmpYear to BCD and write them
  Wire.write(0b00010000); // enable 1Hz Square wave on PIN7
  Wire.endTransmission(); // close the transmission  
}

// Show the time
// You need to use the other program to see the RTC is working
void showTime()
{
  lcd.setCursor(0,0);
    lcd.print("    ");
    lcd.print(myMenu[0]); lcd.print(":"); // hour
    lcd.print(myMenu[1]); lcd.print(":"); lcd.print("00       "); // minute
  lcd.setCursor(3,1);
    lcd.print(days[myMenu[5]]); lcd.print(" "); // DOW   
    lcd.print(myMenu[2]); lcd.print("."); // date
    lcd.print(myMenu[3]); lcd.print("."); // month
    lcd.print(myMenu[4]); lcd.print("   "); // year
// Call the writeRTC function
writeRTC();
}

void loop() 
{
  if (menuCounter == 0)
  {
    setHour();
  }
  if (menuCounter == 1)
  {
    setMinute();
  }
  if (menuCounter == 2)
  {
    setDate();
  }
  if (menuCounter == 3)
  {
    setMonth();
  }
  if (menuCounter == 4)
  {
    setYear();
  }
  if (menuCounter == 5)
  {
    setDOW();
  }
  if (menuCounter == 6)
  {
    showTime();
  }
}
