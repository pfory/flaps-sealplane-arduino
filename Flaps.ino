// flaps for big sealplane
//
// PF 8.8.2014
// HW pins
//  7   - input signal from rec
//  9   - output for servo
//  10  - output for servo
//
// versions
//  1.0 - 13.8.2014

#include <Servo.h> 

Servo servo1;                                     //right flap
Servo servo2;                                     //left flap

//#define debug                                   //just for debug prints

#define NUM_RC_CHANNELS 1                         //You need to specify how many pins you want to use
#include "PinChangeInt.h"                         //If you need pinchangeint you need to include this header
const uint8_t RC_Channel_Pin[NUM_RC_CHANNELS]={7};//Here I specify I want to listen to pins A8 to A13 of my mega

uint16_t RC_Channel_Value[NUM_RC_CHANNELS];       //This variable will contain the values read from the RC signal
#include "RCLib.h"                                //This include needs all declarations above. Do not try to move it up or it won't compile

unsigned long recKrajA      = 1480;
unsigned long recKrajB      = 1870;
unsigned long servo1krajA   = 1100;               //right flap  - down, the less the more angle
unsigned long servo1krajB   = 2150;               //right flap  - up,   the bigger the more angle
unsigned long servo2krajA   = 700;                //left flap   - down, the less the more angle
unsigned long servo2krajB   = 1720;               //left flap -   up,   the bigger the more angle

void plot(int Data1)
{
  Serial.println(Data1); 
}

void setup() {
  servo1.attach(9);   //servo right flap on pin 9
  servo2.attach(10);  //servo left flap on pin 10
#ifdef debug
  Serial.begin(9600);
#endif
  SetRCInterrupts(); //This method will do all the config foe you.
}

void loop() {
  int flag;
  if(flag=getChannelsReceiveInfo()) //Here you read the RC flag contains all the channels that have a response
                                    // see duane's excellent articles on how this works
  {
    if (RC_Channel_Value[0] > recKrajB || RC_Channel_Value[0] < recKrajA) {
    } else {
    if (RC_Channel_Value[0] > 1700) RC_Channel_Value[0] = recKrajB;  //protoze na kanalu 6 se sirka pulsu dost meni a serva se pak klepou staci mi pro klapky
      //pouze krajni polohy
    if (RC_Channel_Value[0] <= 1700) RC_Channel_Value[0] = recKrajA;  //dtto
#ifdef debug
    Serial.print(RC_Channel_Value[0]);
    Serial.print("-");
#endif
    int poloha1 = ((RC_Channel_Value[0]-recKrajA)*(servo1krajB-servo1krajA))/(recKrajB-recKrajA)+servo1krajA;
    int poloha2 = ((RC_Channel_Value[0]-recKrajA)*(servo2krajB-servo2krajA))/(recKrajB-recKrajA)+servo2krajA;
    servo1.writeMicroseconds(poloha1);
#ifdef debug
    Serial.print(poloha2);
#endif
    poloha2 = servo2krajB - poloha2 + servo2krajA;
    servo2.writeMicroseconds(poloha2);
#ifdef debug
    Serial.print("-");
    Serial.println(poloha2);
#endif
    }
  }
}