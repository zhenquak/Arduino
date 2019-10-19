#include <motordriver_4wd.h>
#include <motordriver_4wd_dfs.h>

//int encoder = 2;
int leftPin = 12;
int rightPin = 13;
int current = LOW;
int previous = LOW;
int rightValue = LOW;
int leftValue = LOW; 
int count = 0;
int direct = 0;
int distance = 0;

void setup() {
  // put your setup code here, to run once:
  MOTOR.init();
  //right side
//  MOTOR.setSpeedDir1(30, DIRR);
//  //left side
//  MOTOR.setSpeedDir2(30 , DIRR);
  //count
  // pinMode(encoder, INPUT);  
   pinMode(rightPin,INPUT);
   pinMode(leftPin,INPUT);
   Serial.begin(9600);
}

void loop() {
  //count
 // current = digitalRead(encoder); 
  rightValue = digitalRead(rightPin);
  leftValue = digitalRead(leftPin);
  
  Serial.print(leftValue);
  
  Serial.print("\t");
  Serial.println(rightValue);
   
  if(current == HIGH && previous == LOW)
  {
    count ++;
   Serial.print("Count :");
   Serial.println(count);
  }
   
  previous = current;

 //if ( rightValue == HIGH && leftValue == HIGH ){
   // MOTOR.setSpeedDir1(30,DIRR);
    //MOTOR.setSpeedDir2(30,DIRR);
   //}
  
  if ( rightValue == LOW && leftValue == HIGH) {
    
    MOTOR.setSpeedDir1(0,DIRF);
    MOTOR.setSpeedDir2(50,DIRF); 
    
  }
  
  if ( leftValue == LOW && rightValue == HIGH ) {
    
    MOTOR.setSpeedDir1(50,DIRF);
    MOTOR.setSpeedDir2(0,DIRF);
 
  }

  if ( leftValue == LOW && rightValue == LOW ) {
    MOTOR.setSpeedDir1(0,DIRF);
    MOTOR.setSpeedDir2(0,DIRF);
  }
    
}
