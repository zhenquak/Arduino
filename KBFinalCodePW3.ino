#include <Wire.h>
#include <motordriver_4wd.h>
#include <motordriver_4wd_dfs.h>
#define uchar unsigned char
uchar t;
//void send_sensor_reading(short a1,short b1,short c1,short d1,short e1,short f1);
uchar sensor_reading[16];
float lineDist, P, I, D, error, previouserror, PIDvalue;
float Kp = 12;
float Kd = 0;
float Ki = 0.0005;
int initialspeed = 38;
int x;
int linecounter;
int sensorvalue;
int sensorvalueprevious;
int sense0;
int s;
int m;
int angle = 13;
int a = 0;
int b;
int c;
int d;
int i;

void setup()
{
  delay(1000);
  MOTOR.init();
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  t = 0;
  x = 0;
  b = 1;
  c = 0;
  d = 0;
  pinMode(13, INPUT);
}
void loop()
{
  Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
  while (Wire.available())   // slave may send less than requested
  {
    sensor_reading[t] = Wire.read(); // receive a byte as character
    if (t < 15)
      t++;
    else
      t = 0;
  }
   Serial.println(a);
  /*Serial.print("[0]");
    Serial.println(sr[0]);
    Serial.print("[2]:");
    Serial.println(sr[2]);
    Serial.print("[4]:");
    Serial.println(sr[4]);
    Serial.print("[6]:");
    Serial.println(sr[6]);
    Serial.print("[8]:");
    Serial.println(sr[8]);
    Serial.print("[10]:");
    Serial.println(sr[10]);
    Serial.print("[12]:");
    Serial.println(sr[12]);
    Serial.print("[14]:");
    Serial.println(sr[14]);
    delay(500);*/
    
  calculatePID();
  Stop();
  Motor();
  onRamp();

}

void calculatePID()
{
  lineDist = ( (sensor_reading[0] * -35) + (sensor_reading[2] * -25) + (sensor_reading[4] * -15) +
               (sensor_reading[6] * -5) + (sensor_reading[8] * 5) + (sensor_reading[10] * 15) +
               (sensor_reading[12] * 25) + (sensor_reading[14] * 35) )
               / (sensor_reading[0] + sensor_reading[2] + sensor_reading[4] + sensor_reading[6] + 
                 sensor_reading[8] + sensor_reading[10] + sensor_reading[12] + sensor_reading[14]);
  error = lineDist;
  P = error;
  I = I + error;
  D = error - previouserror;
  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previouserror = error;
}

void Motor()
{
  if (PIDvalue <= -initialspeed)
  {
    PIDvalue = -initialspeed;
  }

  if (PIDvalue >= initialspeed)
  {
    PIDvalue = initialspeed;
  }


  if (PIDvalue < 0)
  { //turn right
    MOTOR.setSpeedDir1(initialspeed + PIDvalue, DIRR); //Dir1=left
    MOTOR.setSpeedDir2(initialspeed , DIRF);  //Dir2=right        
  }
  else
  { //turn left
    MOTOR.setSpeedDir1(initialspeed, DIRR);  
    MOTOR.setSpeedDir2(initialspeed -PIDvalue, DIRF);    
  }
  if (sensor_reading[14] < 60) 
    do {
      MOTOR.setSpeedDir1(34, DIRF);
      MOTOR.setSpeedDir2(25, DIRF);    
      Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
      while (Wire.available())   // slave may send less than requested
      {
        sensor_reading[t] = Wire.read(); // receive a byte as character
        if (t < 15)
          t++;
        else
          t = 0;
      }      
    } while (sensor_reading[4] > 150);
  if (sensor_reading[0] < 60) //car bare right
    do {
      MOTOR.setSpeedDir1(25, DIRF); 
      MOTOR.setSpeedDir2(34, DIRF);
      Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
      while (Wire.available())   // slave may send less than requested
      {
        sensor_reading[t] = Wire.read(); // receive a byte as character
        if (t < 15)
          t++;
        else
          t = 0;
      }     
    } while (sensor_reading[10] > 150);
}
void Stop()
{
  if ((sensor_reading[0] < 60) && (sensor_reading[2] < 60)
      && (sensor_reading[4] < 60) && (sensor_reading[6] < 60)
      && (sensor_reading[8] < 60) && (sensor_reading[10] < 60)
      && (sensor_reading[12] < 60) && (sensor_reading[14] < 60))
  {
    do {
      MOTOR.setSpeedDir2(0, DIRR);
      MOTOR.setSpeedDir1(0, DIRR);
    } while ((sensor_reading[0] < 60) && (sensor_reading[2] < 60) && 
              (sensor_reading[4] < 60) && (sensor_reading[6] < 60) && 
              (sensor_reading[8] < 60) && (sensor_reading[10] < 60) && 
              (sensor_reading[12] < 60) && (sensor_reading[14] < 60));
  }
}
void onRamp()
{
  a = digitalRead(angle);
  if (a == 1)
  {
      do
      {
        Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
        while (Wire.available())   // slave may send less than requested
        {
          sensor_reading[t] = Wire.read(); // receive a byte as character
          if (t < 15)
            t++;
          else
            t = 0;
        }
        Motor();
        calculatePID();
        Stop();
        delay(800); //delay the car motion to climb up the ramp
        MOTOR.setSpeedDir2(0, DIRR);
        MOTOR.setSpeedDir1(0, DIRR); //stops on the ramp for 2s
        delay(2000);
        b = 1;
      } while (b == 0);
      
      do
      {
        Stop();
        sensorvalue = sensor_reading[2];
        sensorvalueprevious = sensorvalue;
        MOTOR.setSpeedDir1(30, DIRF);
        MOTOR.setSpeedDir2(30, DIRR); //turn in clockwise direction
         Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
        while (Wire.available())   // slave may send less than requested
        {
          sensor_reading[t] = Wire.read(); // receive a byte as character
          if (t < 15)
            t++;
          else
            t = 0;
        }
        Stop();
        if (sensorvalue < 100 && sensorvalueprevious > 100)
        {
          linecounter++;
        }
      }while(linecounter<4);
        
       do
      {
        
        Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
        while (Wire.available())   // slave may send less than requested
        {
          sensor_reading[t] = Wire.read(); // receive a byte as character
          if (t < 15)
            t++;
          else
            t = 0;
        }
        Motor();
        calculatePID();
        Stop();
        delay(2000);
        
        //stop at 50cm for 2s
        MOTOR.setSpeedDir1(0, DIRR);
        MOTOR.setSpeedDir2(0, DIRR);
        delay(2000);
        c = 1;
        }while (c == 0);

      do
      {
        Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
        while (Wire.available())   // slave may send less than requested
        {
          sensor_reading[t] = Wire.read(); // receive a byte as character
          if (t < 15)
            t++;
          else
            t = 0;
        }
        Motor();
        calculatePID();
        Stop();
        a=0;
        linecounter=0;
        b = 0;
      } while (b == 1); //force end
        Stop();
}
}

  
