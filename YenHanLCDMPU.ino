#include<Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ;  // Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;

int encoder = 3;
int current = LOW;
int previous = LOW;
int count = 0;
float distance;
int t=0;

void setup()
{
  // setup MPU
  Wire.begin();
  Wire.beginTransmission(MPU_addr); // hexadecimal 0x68 = binary 0b1101000
  Wire.write(0x6B); // Accessing the register 6B - Power Management (Sec. 4.28)
  Wire.write(0); ////Setting SLEEP register to 0. (Required; see Note on p. 9)
  Wire.endTransmission(true);
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop()
{
  printCurrentAngle();
  printRampAngle();
  printDistance();
  printTime();
}

void printCurrentAngle()
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // start register for Accel readings
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  AcX = Wire.read() << 8 | Wire.read();
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  int xAng = map(AcX, minVal, maxVal, -90, 90);
  int yAng = map(AcY, minVal, maxVal, -90, 90);
  int zAng = map(AcZ, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);


  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.print(y);

  delay(300);
}

void printDistance()
{
  current = digitalRead(encoder);
  if (current == HIGH && previous == LOW)
  {
    count++;
    distance = (count / 23.3) * 0.272;
    lcd.setCursor(0, 2);
    lcd.print("D:");
    lcd.print(distance);
  }

  previous = current;
}

void printTime()
{
  
}

void printRampAngle()
{
  
}
