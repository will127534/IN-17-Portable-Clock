int n[]={1,0,3,5};   //1~4
int bcd[]={7,8,9,10};//A~D
int digit[4];
#include "TinyWireS.h"                  
void setup() {
  for(int i=0;i<=3;i++){
pinMode(n[i], OUTPUT);
pinMode(bcd[i], OUTPUT);
  }
   TinyWireS.begin(0x75);  
}

void loop() {
  //    writedigit(15);   
    //  delayMicroseconds(200);
       digitalWrite(n[3],LOW);
      delayMicroseconds(200);
     //  delayMicroseconds(10);
       writedigit( digit[0]);   
       digitalWrite(n[0],HIGH);
       delayMicroseconds(1500);
     
   //   writedigit(15); 
     // delayMicroseconds(100); 
      digitalWrite(n[0],LOW);
      delayMicroseconds(200);
      writedigit(digit[1]);
      digitalWrite(n[1],HIGH);
       delayMicroseconds(1500);
   //    writedigit(15); 
    //  delayMicroseconds(100); 
      digitalWrite(n[1],LOW);
      delayMicroseconds(200);
      writedigit(digit[2]);
      digitalWrite(n[2],HIGH);
      delayMicroseconds(1500);
    //   writedigit(15); 
    //  delayMicroseconds(100); 
      digitalWrite(n[2],LOW);
      delayMicroseconds(200);
      writedigit(digit[3]);
      digitalWrite(n[3],HIGH);
       delayMicroseconds(1500);
 
     if (TinyWireS.available()>0){   
       if (0xFE==TinyWireS.receive()){   
         for(int i=0;i<=3;i++){
         digit[i] = TinyWireS.receive(); 
         }
      }
}

}

void writedigit(int number){
  digitalWrite(bcd[0],bitRead(number,0));
  digitalWrite(bcd[1],bitRead(number,1));
  digitalWrite(bcd[2],bitRead(number,2));
  digitalWrite(bcd[3],bitRead(number,3));
}

