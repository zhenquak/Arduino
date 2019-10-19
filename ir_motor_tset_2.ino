#include <motordriver_4wd.h>
#include <motordriver_4wd_dfs.h>
int current = LOW;
int previous = LOW;
int count = 0;
int x;
int sidesensor = 12;
int leftsensor = 3;
int rightsensor = 13;
int forward;
int left;
int left1;
int right;
int right1;
int stopp;
int r;
int l;
int m;

//dir1 = left
//dir2 = right
void setup() {
  pinMode(rightsensor,INPUT);
  pinMode(leftsensor,INPUT);
  pinMode(sidesensor,INPUT);
  Serial.begin(9600);
   MOTOR.init();
  //MOTOR.setSpeedDir2(30, DIRF);forward
  //MOTOR.setSpeedDir1(30, DIRR);forward
 
  
  
}

void loop() {
  r = digitalRead(rightsensor);
  l = digitalRead(leftsensor);
  m = digitalRead(sidesensor);
  if(r==1&&l==1&&m==1)
  {
    MOTOR.setSpeedDir1(30,DIRR);
    MOTOR.setSpeedDir2(30,DIRF);
  }
  if(r==1&&l==0)
  {//turn left
    MOTOR.setSpeedDir1(80,DIRR);
    MOTOR.setSpeedDir2(80,DIRF);
  }
  if(r==0&&l==1)
  {//turn right
    MOTOR.setSpeedDir1(80,DIRF);
    MOTOR.setSpeedDir2(80,DIRR);
  }
  if(r==1&&l==1&&m==0)
  {//turn right 90 degree
    Serial.print("test");
    MOTOR.setSpeedDir1(30,DIRF);
    MOTOR.setSpeedDir2(30,DIRR);
    delay(200);
    MOTOR.setSpeedDir1(45,DIRF);
    MOTOR.setSpeedDir2(27,DIRF);
    delay(350);
    Serial.println(m);
  }
  if(r==0&&l==0&&m==1)
  {//stop
    MOTOR.setSpeedDir1(0,DIRR);
    MOTOR.setSpeedDir2(0,DIRF);
  }
}
