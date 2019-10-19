// MOTOR LIBRARY
#include <motordriver_4wd.h>
#include <motordriver_4wd_dfs.h>
/****************************************************************************/
// DEFINE ENCODER CODE
int en1 = 2, en2 = 3; // define encoder pin
int current1, current2, previous1 = 0, previous2 = 0;
int count1 = 0, count2 = 0;
/****************************************************************************/
// DEFINE LINE FOLLOWING SENSOR VARIABLE
int mid, left, right;
/****************************************************************************/
//function prototype
void speed20();
void speed00();
void encoderCount();
void linefollow();
/****************************************************************************/
void setup() {
  MOTOR.init();  //DEFINE MOTOR PIN
  Serial.begin(57600); // SERIAL BEGIN SEND ENCODER BAUD RATE
  //speed20();  // SET MOTOR SPEED TO 55
  //speed00();  // SET MOTOR SPEED TO 0 (STOP)

  //DEFINE LINE FOLLOWING SENSOR PIN
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(3, INPUT);
}

/****************************************************************************/
void loop() {
  encoderCount();
  linefollow();
}

/****************************************************************************/
void linefollow() {
  left = digitalRead(12);
  mid = digitalRead(11);
  right = digitalRead(3);
  if ( left == 1 && right == 1 && mid == 0) { //CONSTANTLY STRAIGHT
    MOTOR.setSpeedDir1(25, DIRF);
    MOTOR.setSpeedDir2(25, DIRF);
  }
  if ( left == 0 && right == 1) {             //LEFT TURN
    MOTOR.setSpeedDir1(25, DIRR);
    MOTOR.setSpeedDir2(45, DIRF);
  }
  if ( left == 1 && right == 0) {             //RIGHT TURN
    MOTOR.setSpeedDir1(45, DIRF);
    MOTOR.setSpeedDir2(25, DIRR);
  }
  if ( left == 0 && right == 0) {             //STOP ALL MOTOR
    MOTOR.setSpeedDir1(0, DIRF);
    MOTOR.setSpeedDir2(0, DIRF);
  }
  if (right == 1 && mid == 1 && left == 0) {   //LEFT SHARP TURN
    MOTOR.setSpeedDir1(35, DIRR);
    MOTOR.setSpeedDir2(25, DIRF);
  }
  if ( left == 1 && mid == 1 && right == 0) {   //RIGHT SHARP TURN
    MOTOR.setSpeedDir1(25, DIRF);
    MOTOR.setSpeedDir2(35, DIRR);
  }
}

/****************************************************************************/
void encoderCount() {
//  current1 = digitalRead(en1);
  current2 = digitalRead(en2);
//  if (current1 == 1 && previous1 == 0) {
//    count1++;
//    //Serial.print(count1);
//    //Serial.print("\t");
//    Serial.print('A');
//  }
//  previous1 = current1;
  if (current2 == 1 && previous2 == 0) {  //COUNT ENCODER AND SEND 'B' TO UNO
    count2++;
    //Serial.println(count2);
    Serial.print('B');
  }
  previous2 = current2;
}

/****************************************************************************/
void speed20() {
  MOTOR.setSpeedDir1(20, DIRF);
  MOTOR.setSpeedDir2(20, DIRF);
}

/****************************************************************************/
void speed00() {
  MOTOR.setSpeedDir1(0, DIRF);
  MOTOR.setSpeedDir2(0, DIRF);
}
