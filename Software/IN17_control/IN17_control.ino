#include <SHT2x.h>

#include <Wire.h>
#include <TEA5767Radio.h>
#include "Adafruit_DRV2605.h"


#define drv_address 0x5A
#define DS1307_ADDRESS 0x68


TEA5767Radio radio = TEA5767Radio();
Adafruit_DRV2605 drv;

int data[10];
int k;
int ledbrightness = 125;
int effect = 15;
int led[] = {6, 5, 3, 9};
int button[] = {10, 4, 7, 8}; // S4,S1,S2,S3

int RTC_INT = 2;
bool nextsecond = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  for (int i = 0; i <= 3; i++) {
    pinMode(led[i], OUTPUT);
  }
  for (int i = 0; i <= 3; i++) {
    pinMode(button[i], INPUT_PULLUP);
  }

  drv.begin();
  drv.selectLibrary(1);
  drv.setMode(DRV2605_MODE_INTTRIG);

  setRTCinterrupt();
  attachInterrupt(0, rtcsync, FALLING);
  long time = millis();
  long timemain = millis();
  analogWrite(led[1],ledbrightness);
  analogWrite(led[0],ledbrightness);
}

long time;
long timemain;
bool longbutton = 0;
void loop() {
  if (nextsecond) {
    printDate();
    nextsecond = 0;
  }
  
  int buttonOn = scan();

  if (buttonOn != 10) {
    Serial.println(buttonOn);
    digitalWrite(led[1],LOW);
    digitalWrite(led[0],LOW);
    detachInterrupt(0);
    switch (buttonOn) {
      case 0:
        counter();
        break;
      case 1:
        
        special_mode();
        break;
      case 2:
        //fm();
        alarm();
        break;
      case 3:
        //settime();
      
            if(longbutton){
              Serial.println("in to setting");
              longbutton = 0;
              //setting();
                break;
            }
        

        Temperature();
        break;
      default:
        break;
    }
    attachInterrupt(0, rtcsync, FALLING);
    analogWrite(led[1],ledbrightness);
    analogWrite(led[0],ledbrightness);
  }
}
void setting(){
  int flag = 1;
while(flag){
  int buttonOn = scan();
  if (buttonOn != 10) {
    Serial.println(buttonOn);
    switch (buttonOn) {
      case 0:
     
        break;
      case 1:
       
        break;
      case 2:
         digitalWrite(led[1],LOW);
    digitalWrite(led[0],LOW);
    flag = 0;
        break;
      case 3:
       
        
        break;
      default:
        break;
    }
  }
   analogWrite(led[1],ledbrightness);
    analogWrite(led[0],ledbrightness);
    delay(1000);
    digitalWrite(led[1],LOW);
    digitalWrite(led[0],LOW);
    delay(1000);
}


   
}
void rtcsync() {
  Serial.println("tick");
  nextsecond = 1;
}

void setIn17() {

  Wire.beginTransmission(0x75);
  Wire.write(0xFE);

  for (int i = 0; i <= 3; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();

}

int scan() {
  for (int i = 0; i <= 3; i++) {
    if (digitalRead(button[i]) == LOW) {
      uint32_t st = millis();
      while (digitalRead(button[i]) == LOW) {
        if(millis()-st>500){
            longbutton =1;
        }
      }

      return i;
    }
  }
  return 10;
}

void Temperature(){
int flag = 1;
 analogWrite(led[1],ledbrightness);
 uint32_t startmillis = millis();
  while(flag){
   if(millis()-startmillis>1000){
    float humidity = SHT2x.GetHumidity();
    float temperature = SHT2x.GetTemperature();
  Serial.print("Humidity(%RH): ");
  Serial.print(humidity);
  Serial.print("     Temperature(C): ");
  Serial.println(temperature);
  data[0] = int(temperature)/10;
  data[1] = int(temperature)%10;
  data[2] = int(humidity)/10;
  data[3] = int(humidity)%10;
  setIn17();
  startmillis = millis();
  }
  int buttonOn = scan();
  if (buttonOn != 10) {
    Serial.println(buttonOn);
    switch (buttonOn) {
      case 0:
     
        break;
      case 1:
       
        break;
      case 2:
       
        break;
      case 3:
        digitalWrite(led[1],LOW);
        flag = 0;
        break;
      default:
        break;
    }
  }
  }
  


}
void randommode() {
  for (int k = 0; k <= 300; k++) {
    randomSeed(analogRead(0));
    for (int i = 0; i <= 3; i++) {
      data[i] = random(9);
    }
    setIn17();
    delay(25);
  }
}

void randommodee(){
  
  for (int t = 0;t<=400;t++){
     randomSeed(analogRead(A6));
    if(t<80){
      for(int i=0;i<=3;i++){
       data[i] = random(9);
     } 
     setIn17();  
     delay(15);
   }
    else if(t>=80 && t<160){          //first stage 

      for(int i=0;i<=2;i++){
       data[i] = random(9);
     }
     setIn17();
     delay(15);
   }
    else if(t>=160 && t<240){       //second stage
     for(int i=0;i<=1;i++){
       data[i] = random(9);
     }
     setIn17();
     delay(15);
   }
   else if(t>=240 && t<320){
     for(int i=0;i<=0;i++){
       data[i] = random(9);
     }
     setIn17();
     delay(15);
   }
   else{
    setIn17();
    delay(15);
  }
}
}
void sequencemode(){
 for(int k=0;k<=5;k++){
  for(int j=0;j<=9;j++){
   for(int i=0;i<=3;i++){
     data[i] = j;
   } 
   setIn17();  
   delay(100);
 }
}
}
void wavemode() {  // 5 4 9 3 6 0 8 2 7 1
  int wave[22] = {5, 4, 9, 3, 6, 0, 8, 2, 7, 1, 7, 2, 8, 0, 6, 3, 9, 4, 5, 4, 9};
  for (int count = 0; count < 10; count++) {
    for (int k = 4; k <= 21; k++) {
      for (int i = 0; i < 4; i++) {
        data[i] = wave[k - i];
      }
      setIn17();
      delay(50);
    }
  }
}

void alarm(){
 int flag = 1;
 int minute = 0;
 int second = 0;
 int counting =0; 
 uint32_t lastcount = millis();
  while (flag) {
      data[3] = second % 10;;
      data[2] = second / 10;
      data[1] = minute % 10;
      data[0] = minute / 10;
      setIn17();
    
  int buttonOn = scan();
  if (buttonOn != 10) {
    Serial.println(buttonOn);
    switch (buttonOn) {
      case 0:
      if (counting ==0){
        minute++;
        second=0;
      }
        break;
      case 1:
         if (counting ==0){
        minute--;
        if(minute<0){
        minute = 0;
        }
        second=0;
      }
        break;
      case 2:
        counting=!counting;
        lastcount = millis();
        break;
      case 3:
        flag = 0;
        break;
      default:
        break;
    }
  }
  if(millis()-lastcount>=1000 && counting == 1){
   second--;
  if(second<0){
   minute--;
  second=59; 
  }
  if(minute<0){   //end
     drv.setWaveform(0, effect);  // play effect 
     drv.setWaveform(1, 0);       // end waveform
     drv.go();// play the effect!
   minute = 0;
  second = 0; 
  }
  lastcount = millis();
    digitalWrite(led[1],!digitalRead(led[1]));
    digitalWrite(led[0],!digitalRead(led[0]));
  }
  
}
}

void counter() {
  int flag = 1;
  int count_flag = 0;
  uint32_t starttime = 0;
  uint32_t displaytime = 0;
  int second = 0;
  int minute = 0;
  int mill = 0;
  analogWrite(led[2], ledbrightness);
  analogWrite(led[3], ledbrightness);
  while (flag) {
    if (count_flag == 1) {
      time = (millis() - starttime) / 100;
      mill =  time % 10;
      second = time / 10 % 60;
      minute = time / 600;
    }
    if (minute > 9) {
      starttime = millis();
    }
    if (millis() - displaytime > 50) {
      data[3] = mill;
      data[2] = second % 10;
      data[1] = second / 10;
      data[0] = minute;

      setIn17();
      displaytime = millis();
    }
    int buttonOn = scan();
    if (buttonOn == 1) {
      mill = 0;
      second = 0;
      minute = 0;
      data[3] = 0;
      data[2] = 0;
      data[1] = 0;
      data[0] = 10;
      setIn17();
    }
    if (buttonOn == 3) {
      flag = 0;
    }
    if (buttonOn == 0) {
      count_flag = !count_flag;
      starttime = millis();
    }
  }
  mill = 0;
  second = 0;
  minute = 0;
  digitalWrite(3, LOW);
  digitalWrite(9, LOW);
}


byte bcdToDec(byte val)  {
  // Convert binary coded decimal to normal decimal numbers
  return ( (val / 16 * 10) + (val % 16) );
}

void printDate() {

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());
  data[3] = minute % 10;
  data[2] = minute / 10 % 10;
  data[1] = hour % 10;
  data[0] = hour / 10 % 10;
  setIn17();
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

}

void serialEvent() {
  //noInterrupts();
  detachInterrupt(0);
  Serial.println("Get Data");
 Serial.println(Serial.available());

  while (Serial.available()) {
    byte second =  Serial.parseInt();
    //delay(50);
    Serial.print(second);
    byte minute =  Serial.parseInt();
    //delay(50);
    Serial.print(minute);
    byte hour =  Serial.parseInt();
   // delay(50);
    Serial.print(hour);
    byte weekDay =  Serial.parseInt();
   // delay(50);
    Serial.print(weekDay);
    if (weekDay == 0) {
      weekDay = 7;
    }
    byte monthDay =  Serial.parseInt();
   // delay(50);
    Serial.print(monthDay);
    byte month =  Serial.parseInt();
   // delay(50);
    Serial.print(month);
    byte year =  Serial.parseInt();

    Serial.print(year);

    Serial.print("..");
 Serial.println(Serial.available());
    Serial.print("Get time:");
    Serial.print(month);
    Serial.print("/");
    Serial.print(monthDay);
    Serial.print("/");
    Serial.print(year);
    Serial.print(" ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.println(second);
    Wire.beginTransmission(DS1307_ADDRESS);
    Wire.write(0x00); //stop Oscillator

    Wire.write(decToBcd(second));
    Wire.write(decToBcd(minute));
    Wire.write(decToBcd(hour));
    Wire.write(decToBcd(weekDay));
    Wire.write(decToBcd(monthDay));
    Wire.write(decToBcd(month));
    Wire.write(decToBcd(year));

    Wire.write(0x00); //start

    Wire.endTransmission();
    while(Serial.available()){
      int dummy = Serial.read();// statement
  }
  }
  
  attachInterrupt(0, rtcsync, FALLING);
}

byte decToBcd(byte val) {
  // Convert normal decimal numbers to binary coded decimal
  return ( (val / 10 * 16) + (val % 10) );
}


void special_mode() {
  int flag = 1;
  int select = 0;
  data[3] = select;
  data[2] = 10;
  data[1] = 10;
  data[0] = 10;
  setIn17();
  while (flag) {
    int button = scan();
    if (button == 0) {
      select++;
      data[3] = select;
      setIn17();
    }
    if (button == 1) {
      select--;
      if(select<0){
       select = 0; 
      }
      data[3] = select;
      setIn17();
    }
    if (button == 2) {
      switch (select) {
        case 0:
          randommodee();
          break;
        case 1:
          sequencemode();
          break;
        case 2:
          wavemode();
          break;
        case 3:
          //settime();
          break;
        default:
          break;
      }
      flag = 0;
    }
    if (button == 3) {
      flag = 0;
    }
  }
}

void fm() {

  int flag = 1;
  int channal = 950;  //98.1mhz=>981
  data[3] = channal % 10;
  data[2] = channal / 10 % 10;
  data[1] = channal / 100 % 10;
  data[0] = channal / 1000 % 10;
  digitalWrite(led[0], HIGH);
  setIn17();
  while (flag) {
    int buttonOn = scan();
    if (buttonOn == 1) {
      channal += 10;
      data[3] = channal % 10;
      data[2] = channal / 10 % 10;
      data[1] = channal / 100 % 10;
      data[0] = channal / 1000 % 10;
      setIn17();
      setfm(channal);
    }
    if (buttonOn == 2) {
      channal -= 10;
      data[3] = channal % 10;
      data[2] = channal / 10 % 10;
      data[1] = channal / 100 % 10;
      data[0] = channal / 1000 % 10;
      setIn17();
      setfm(channal);
    }
    if (buttonOn == 3) {
      channal += 1;
      data[3] = channal % 10;
      data[2] = channal / 10 % 10;
      data[1] = channal / 100 % 10;
      data[0] = channal / 1000 % 10;
      setIn17();
      setfm(channal);
    }

    if (buttonOn == 0) {
      flag = 0;
      digitalWrite(led[0], LOW);
    }
  }

}


void setfm(int feq) {
  radio.setFrequency(feq / 10.0);


}

void setRTCinterrupt() {
  Wire.beginTransmission(0x68);          //Tell devices on the bus we are talking to the DS3231
  Wire.write(0x0e);
  Wire.write(0);                      //Tell the device which address we want to read or write
  Wire.endTransmission();
}
