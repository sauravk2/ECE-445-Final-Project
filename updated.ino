
 #include <Keypad.h>
 #include <LiquidCrystal.h>
 
const byte ROWS = 4;  //number of rows in our Keypad
const byte COLS = 3;  //number of columns in our Keypad

#define Rs 50
#define Enable 48
#define D4 46
#define D5 44
#define D6 42
#define D7 40 
  int DoorStatus = 0;
LiquidCrystal lcd(Rs, Enable, D4, D5, D6, D7);

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3' },
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
}; //array that stores the characters on our Keypad

byte rowPins[ROWS] = {37, 9, 11, 22}; //pins on Arduino that are used for the rows
byte colPins[COLS] = {34, 23, 32};  //pins on Arduino that are used for the columns
int piezopin = 12;
int ServoPin = 25;
int ServoPinfeeder = 27;
long lastTime = 0;

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); //create an object that represents the keypad we are using
String inputString;
String inputStringtime;//initialize an input string
String timeinput;
String timeinputtime;                                                               //initialize a string for the time input
                                                            //initalize to zero so that we can count minutes
long minutes = 32;                                                                // represents the RTC minutes; must be hard coded for now
long hours = 6;  
int timestatus = 1;
// represents the RTC hours; must be hard coded for now


#define Ultra_in 45
#define Ultra_out 47





void setup() {                
  // initialize the digital pins as outputs.
  lcd.begin(16,2);
  /*pinMode(Rs, OUTPUT);     
  pinMode(Enable, OUTPUT);     
  pinMode(D4, OUTPUT);  
  pinMode(D5, OUTPUT);  
  pinMode(D6, OUTPUT);  
  pinMode(D7, OUTPUT); */
  pinMode(10, OUTPUT);                                                            //pin on arduino that is connected to cage door motor
  pinMode(Ultra_in, INPUT);
  pinMode(Ultra_out, OUTPUT);
  Serial.begin(9600);

}


// the loop routine runs over and over again forever:
void loop() {
  if(timestatus == 1){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter the Time:");
    char keytime = customKeypad.getKey();
    if (keytime) {


    if (keytime >= '0' && keytime <= '9') {                                               // only act on numeric keys            
      inputStringtime += keytime;                                                         // append new character to input string
    } else if (keytime == '#') {                                                      // '#' is used for the user to confirm the time they entered
      if (inputStringtime.length()==4) {                                              // if the user confirms the time and the lenght of the string is 4
        timeinputtime = inputStringtime;
        lcd.setCursor(0, 1);
        lcd.print(timeinputtime[0]);
        lcd.print(timeinputtime[1]);
        lcd.print(":");
        lcd.print(timeinputtime[2]);
        lcd.print(timeinputtime[3]);  
        String l = String(timeinputtime[0]);                                                  //set string x equal to the left most digit (1st digit)
        String m = String(timeinputtime[1]);                                                  //set string y equal to the 2nd digit
        String n = l+m;                                                                   //append y to x and store in string z; this represents the digits for the hour
        int o = n.toInt();
        hours = o;
        String p = String(timeinputtime[2]);                                                 //set string a equal to the 3rd digit
        String q = String(timeinputtime[3]);                                                 //set string b equal to the 4th digit
        String r = p+q;                                                                  //append b to a and store in string c; this represents the digits for the minutes
        int s = r.toInt(); 
        minutes = s;//convert c to an integer
        lastTime = millis();
        timestatus = 0;
       
        
  }
    }
    else if (keytime == '*') {                                                        //"*" is used to rest the time the user entered
      inputStringtime = "";                                                            // reset both strings so they are empty and user can enter a new time
      timeinputtime=inputStringtime;// clear input
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter the Time:");
  
    }
    }
  
 
  }
  if(millis()-lastTime > 60000){                                      
    minutes++;                                                                    //increment the RTC minute if 60,000 ms have passed
    lastTime = millis();
  }
  if(minutes > 60){                                                               //once RTC minutes reaches 60, increment the RTC hour and reset the minutes to zero
    hours++;
    minutes = minutes-60;
  }
  if(hours >= 24){                                                               //once RTC minutes reaches 60, increment the RTC hour and reset the minutes to zero
    hours = 0;
  }
 char key = customKeypad.getKey();                                               //scan the character entered on the keypad by user
 
  if (key) {


    if (key >= '0' && key <= '9') {                                               // only act on numeric keys            
      inputString += key;                                                         // append new character to input string
    } else if (key == '#') {                                                      // '#' is used for the user to confirm the time they entered
      if (inputString.length()==4) {                                              // if the user confirms the time and the lenght of the string is 4
        timeinput = inputString;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Time of Opening:");
          lcd.setCursor(0, 1);
          lcd.print(timeinput[0]);
          lcd.print(timeinput[1]);
          lcd.print(":");
          lcd.print(timeinput[2]);
          lcd.print(timeinput[3]);  
        //printDisplay(timeinput,1000);
        //print any char on any digit
        //print any char on any digit//then set the time input string equal to the input string
        inputString = "";                                                           // clear input string when done; the correct time input will be stored
        // DO YOUR WORK HERE                                                         // in the time input string

      }
      else if (inputString.length()==0){
        Serial.println("Emergency Button Pressed");
        
        if(DoorStatus == 0){ //door closed so open it
          Serial.println("cage emergency open");
          for(int PWMopen = 500; PWMopen <= 2480; PWMopen = PWMopen + 20) {
             for(int counter = 0; counter <= 10; counter++){
               analogWrite(ServoPin,255);
               delayMicroseconds(PWMopen);
               analogWrite(ServoPin,0);
               delayMicroseconds(20000-PWMopen);
             } 
          }
               Serial.println("done");
          DoorStatus = 1;
        }

        
        else if(DoorStatus == 1){ //door open so close it
          Serial.println("Cage Emergency Close");
          for(int PWMclose = 2480; PWMclose > 400; PWMclose = PWMclose - 100) {
             for(int counter = 0; counter <= 3; counter++){
               analogWrite(ServoPin,255);
               delayMicroseconds(PWMclose);
              analogWrite(ServoPin,0);
              delayMicroseconds(5000-PWMclose);
        
              long dur;
              long dis;
              long tocm;
              digitalWrite(Ultra_out,LOW);
              delayMicroseconds(2);
              digitalWrite(Ultra_out,HIGH);
              delayMicroseconds(10);
              digitalWrite(Ultra_out,LOW);
              dur=pulseIn(Ultra_in,HIGH);
              tocm= dur / 29 / 2;
             //Serial.println(String(tocm));
              if((tocm <= 18)&&(tocm > 1)){
                 Serial.println("Stop! Motion is sensed. Cage is opening again.");
                 for(int PWMopen = PWMclose; PWMopen <= 2480; PWMopen = PWMopen + 30) {
                   for(int counter = 0; counter <= 5; counter++){
                     analogWrite(ServoPin,255);
                     delayMicroseconds(PWMopen);
                    analogWrite(ServoPin,0);
                     delayMicroseconds(10000-PWMopen);
                     PWMclose = 2480;
                    } 
                  }
                }
              } 
            }
                 Serial.println("done");
          DoorStatus = 0;
        }
        
      }
    } else if (key == '*') {                                                        //"*" is used to rest the time the user entered
      inputString = "";                                                            // reset both strings so they are empty and user can enter a new time
      timeinput=inputString;// clear input
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Time of Opening:");
          lcd.setCursor(0, 1);
          lcd.print(timeinput[0]);
          lcd.print(timeinput[1]);
          lcd.print(":");
          lcd.print(timeinput[2]);
          lcd.print(timeinput[3]);  
    }
  }
  

  


//Serial.print(hours);
//Serial.print(":");
//Serial.println(minutes);
 String x = String(timeinput[0]);                                                  //set string x equal to the left most digit (1st digit)
 String y = String(timeinput[1]);                                                  //set string y equal to the 2nd digit
 String z = x+y;                                                                   //append y to x and store in string z; this represents the digits for the hour
 int f = z.toInt();                                                                //convert string z to an integer
 String a = String(timeinput[2]);                                                 //set string a equal to the 3rd digit
 String b = String(timeinput[3]);                                                 //set string b equal to the 4th digit
 String c = a+b;                                                                  //append b to a and store in string c; this represents the digits for the minutes
 int d = c.toInt();   
 Serial.println(f);
  Serial.println(d);//convert c to an integer

 
   if ((hours == f) && (minutes ==d)) {    //check whether the hours of the RTC are equal to the hour entered by the user
      long Timestamp1 = millis();
      openCage();                                                                   //and if the minutes of the RTC are equal to the minutes entered by the user
      long offset = (millis() - Timestamp1) / 60000;
      minutes = minutes + offset;
       lastTime = millis();
      //delay(2);                                                                    //if they are then call the open cage function 
      //closeCage();
      //delay(60000);
    }
}
   
   

  

void openCage() {

  int speed1 = 10;
    for(int PWMopen = 500; PWMopen <= 2480; PWMopen = PWMopen + 20) {
    for(int counter = 0; counter <= speed1; counter++){
      analogWrite(ServoPin,255);
      delayMicroseconds(PWMopen);
      analogWrite(ServoPin,0);
      delayMicroseconds(20000-PWMopen);
    } 
  }
    DoorStatus = 1;
    Serial.println("The cage is now open!");
    //delay(60000);
    //Serial.println("2 hours is up. Beep!");
    //Serial.println(millis());
    long WaitTime = millis();
    while(millis() < (WaitTime + 60000)){
      char key = customKeypad.getKey();
      if (key) {


       if (key == '#') {                                                      // '#' is used for the user to confirm the time they entered
          if (inputString.length()==0){
        Serial.println("Emergency Button Pressed");
        if(DoorStatus == 0){ //door closed so open it
          for(int PWMopen = 500; PWMopen <= 2480; PWMopen = PWMopen + 20) {
             for(int counter = 0; counter <= 10; counter++){
              analogWrite(ServoPin,255);
              delayMicroseconds(PWMopen);
              analogWrite(ServoPin,0);
              delayMicroseconds(20000-PWMopen);
             } 
          }
               Serial.println("done");
          DoorStatus = 1;
        }
        else if(DoorStatus == 1){ //door open so close it
          Serial.println("cage door is moving up");
          for(int PWMclose = 2480; PWMclose > 400; PWMclose = PWMclose - 50) {
             for(int counter = 0; counter <= 3; counter++){
               analogWrite(ServoPin,255);
               delayMicroseconds(PWMclose);
              analogWrite(ServoPin,0);
              delayMicroseconds(5000-PWMclose);
        
        long dur;
        long dis;
        long tocm;
        digitalWrite(Ultra_out,LOW);
        delayMicroseconds(2);
        digitalWrite(Ultra_out,HIGH);
        delayMicroseconds(10);
        digitalWrite(Ultra_out,LOW);
        dur=pulseIn(Ultra_in,HIGH);
        tocm= dur / 29 / 2;
        Serial.println(String(tocm));
        if((tocm <= 18)&&(tocm > 1)){
          Serial.println("Stop! Motion is sensed. Cage is opening again.");
            for(int PWMopen = PWMclose; PWMopen <= 2480; PWMopen = PWMopen + 30) {
              for(int counter = 0; counter <= 5; counter++){
                 analogWrite(ServoPin,255);
                 delayMicroseconds(PWMopen);
                 analogWrite(ServoPin,0);
                 delayMicroseconds(20000-PWMopen);
                 PWMclose = 2480;
              } 
           }
         }
      } 
     }
     Serial.println("done");
     DoorStatus = 0;
     timestatus = 0;
      return;
        }
        
      }
    } 
  }
       
    }
    
    
    long timer = millis();
    
    
    tone(piezopin, 800, 2000);
    //int val;
    //val = analogRead(A3);
  while(analogRead(A3) > 150)
  {
    
   long timer1 = (millis());
   if((timer1) > timer + 10000){
   tone(piezopin, 800, 2000);
   //delay(900000);
   Serial.println("Beep! Come Back!");
   timer = abs(millis());
   
   //Serial.println(val);
   //delay(30000); 
   }
  }
  closeCage();
  return;
    

    
  //}
}
void closeCage() { //function that closes cage from any position)
    for(int PWMclose = 2480; PWMclose > 400; PWMclose = PWMclose - 50) {
      for(int counter = 0; counter <= 3; counter++){
        analogWrite(ServoPin,255);
        delayMicroseconds(PWMclose);
        analogWrite(ServoPin,0);
        delayMicroseconds(5000-PWMclose);
        //Serial.println("cage door is moving up");
        long dur;
        long dis;
        long tocm;
        digitalWrite(Ultra_out,LOW);
        delayMicroseconds(2);
        digitalWrite(Ultra_out,HIGH);
        delayMicroseconds(10);
        digitalWrite(Ultra_out,LOW);
        dur=pulseIn(Ultra_in,HIGH);
        tocm= dur / 29 / 2;
        //Serial.println(String(tocm));
        if((tocm <= 18)&&(tocm > 1)){
          Serial.println("Stop! Motion is sensed. Cage is opening again.");
            for(int PWMopen = PWMclose; PWMopen <= 2480; PWMopen = PWMopen + 30) {
              for(int counter = 0; counter <= 5; counter++){
                 analogWrite(ServoPin,255);
                 delayMicroseconds(PWMopen);
                 analogWrite(ServoPin,0);
                 delayMicroseconds(20000-PWMopen);
                 PWMclose = 2480;
              } 
           }
         }
      } 
     }
    //DoorStatus = 0;
  //Serial.println("The cage is now closed");
 
  //Serial.println("Activate Feeder");
  for(int counter = 0;counter < 40;counter++) {
    analogWrite(ServoPinfeeder,255);
    delayMicroseconds(2300);
    analogWrite(ServoPinfeeder,0);
    delayMicroseconds(10000-2300);
  }
  delay(500);
   for(int counter = 0;counter < 40;counter++) {
    analogWrite(ServoPinfeeder,255);
    delayMicroseconds(1800);
    analogWrite(ServoPinfeeder,0);
    delayMicroseconds(10000-1800);
  }
  DoorStatus = 0;
  timestatus = 0;
  //return;

}

/*long microsecondsToCentimeters(long microseconds)
{
return microseconds / 29 / 2;
}*/

/*void openCageTest() {
 
  int speed1 = 10;
    for(int PWMopen = 500; PWMopen <= 2480; PWMopen = PWMopen + 30) {
    for(int counter = 0; counter <= 5; counter++){
      analogWrite(ServoPin,255);
      delayMicroseconds(PWMopen);
      analogWrite(ServoPin,0);
      delayMicroseconds(10000-PWMopen);
    } 
  }
}*/

 
