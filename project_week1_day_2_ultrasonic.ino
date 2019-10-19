#include <motordriver_4wd.h>
#include <motordriver_4wd_dfs.h>
#include <Wire.h>
#define SLAVE_ADDRESS 0x04
//#include <NewPing.h>


//NewPing sonar(A3,A2);

int number;
//int state = 0;
/*int trig = A3;
int echo = A2;
int distance_from_obstacle;*/


void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  MOTOR.init();
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData); 
}

void receiveData(int byteCount) {
  while (Wire.available()) {
    number = Wire.read();
    Serial.println(number);
    if( number == 1)
  {
    MOTOR.setSpeedDir1(25, DIRF);
    MOTOR.setSpeedDir2(15, DIRR);
  }
    else if ( number == 2)
  {
    MOTOR.setSpeedDir1(15, DIRF);
    MOTOR.setSpeedDir2(15, DIRF);
  }
    else if ( number == 3)
  {
    MOTOR.setSpeedDir1(15, DIRR);
    MOTOR.setSpeedDir2(25, DIRF);
  }
  else if ( number == 4)
  {
    sendData();
    MOTOR.setSpeedDir1(0, DIRF);
    MOTOR.setSpeedDir2(0, DIRF);
    delay(2000);
    MOTOR.setSpeedDir1(15, DIRR);
    MOTOR.setSpeedDir2(15, DIRF);
   
  }
  else if ( number == 5)
  {
    MOTOR.setSpeedDir1(0, DIRF);
    MOTOR.setSpeedDir2(0, DIRF); 
    delay(4000);
    sendData();
    MOTOR.setSpeedDir1(15, DIRF);
    MOTOR.setSpeedDir2(15, DIRR);
  }
 }
}

void sendData()
{
Wire.write(number);
}

void loop() {
  //distance_from_obstacle = sonar.ping_cm();
  //MOTOR.setSpeedDir1(20, DIRF);
 // MOTOR.setSpeedDir2(20, DIRF);

 /* if ( number == 'w')
  {
    MOTOR.setSpeedDir1(30, DIRF);
    MOTOR.setSpeedDir2(30, DIRF);
  }
  else if ( number == 'r')
  {
    MOTOR.setSpeedDir1(30, DIRR);
    MOTOR.setSpeedDir2(30, DIRR);
  }
  else if ( number == 'l')
  {
    MOTOR.setSpeedDir1(0, DIRF);
    MOTOR.setSpeedDir2(40, DIRF);
  }
  else if ( number == 'o')
  {
    MOTOR.setSpeedDir1(40, DIRF);
    MOTOR.setSpeedDir2(0, DIRR);
  }
  else if ( number == 's')
  {
    MOTOR.setSpeedDir1(0, DIRF);
    MOTOR.setSpeedDir2(0, DIRR);
  }*/
   

}
  


   // end while

// 1callback for sending data
