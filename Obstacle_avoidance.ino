#include <motordriver_4wd.h>
#include <motordriver_4wd_dfs.h>


#include <NewPing.h>

int trig = A3;
int echo = A2;
int distance_from_obstacle;


NewPing sonar(A3,A2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  MOTOR.init();

}

void loop() {

  distance_from_obstacle = sonar.ping_cm();
//  Serial.print("Distance: ");
//  Serial.print(distance_from_obstacle);
//  Serial.println("cm");
//  delay(100);



   if(distance_from_obstacle < 25)
  {
//    MOTOR.setSpeedDir1(0, DIRR);
//    MOTOR.setSpeedDir2(0, DIRR);
//    delay(100);
    MOTOR.setSpeedDir1(0, DIRR);
    MOTOR.setSpeedDir2(0, DIRR);
    delay(200);
  }

  else if( distance_from_obstacle < 20)
  {
//    MOTOR.setSpeedDir1(0, DIRR);
//    MOTOR.setSpeedDir2(0, DIRR);
//    delay(100);
    MOTOR.setSpeedDir1(0, DIRR);
    MOTOR.setSpeedDir2(0, DIRR);
  //  delay(200);
  }
    else if( distance_from_obstacle < 15)
  {
//    MOTOR.setSpeedDir1(0, DIRR);
//    MOTOR.setSpeedDir2(0, DIRR);
//    delay(100);
    MOTOR.setSpeedDir1(0, DIRR);
    MOTOR.setSpeedDir2(0, DIRR);
   // delay(200);
  }

  
  else
  {
    MOTOR.setSpeedDir1(30, DIRF);
    MOTOR.setSpeedDir2(30, DIRF);
  }

  
}
