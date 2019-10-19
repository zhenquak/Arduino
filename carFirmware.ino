#include <seeed_pwm.h>
#include <motordriver_4wd.h>
#include <Wire.h>

#define ENCODER1 2
#define ENCODER2 3
#define CMD_TOKEN '#'

bool check = true;
int speed1 = 0;
int speed2 = 0;
int dir1 = DIRF;
int dir2 = DIRF;
long unsigned int turns1 = 0;
long unsigned int turns2 = 0;
long unsigned int target1 = 0;
long unsigned int target2 = 0;
bool isTargetDefined = false;

void encoder1ISR(){
  ++turns1;
}

void encoder2ISR(){
  ++turns2;
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin(42);
  Serial.begin(57600);
  Wire.onReceive(receiveEventI2C);
  Wire.onRequest(requestEventI2C);
  MOTOR.init();
  attachInterrupt(digitalPinToInterrupt(ENCODER1),encoder1ISR,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER2),encoder2ISR,RISING);
  while(!Serial){}
}

void loop() {
  if(isNewDataAvailable()){
    if(checkForToken()){
      resolveCommand(false);
    }
  }

  if(isTargetDefined){
    pollMotors();
    isTargetDefined = !carStopped();
  }
}

void receiveEventI2C(int nBytes){
  resolveCommand(true);
}

void requestEventI2C(){
  byte msg[8];
  byte *turn1 = (byte *)&turns1;
  byte *turn2 = (byte *)&turns2;
  for(int i = 0 ; i  < 4 ; i++){
    msg[i] = turn1[i];
    msg[i+4] = turn2[i];
  }
  Wire.write(msg,8);
}

inline bool isNewDataAvailable(){
  return Serial.available() > 1 || (!check && Serial.available()>0 );
}

inline bool checkForToken(){
  if(check){
    return Serial.read() == CMD_TOKEN;
  }
  check = true;
  return true;
}

inline void testForToken(char a){
  check = !(a==CMD_TOKEN);
}

inline void resolveCommand(bool I2C){
  char command = readChar(I2C);
      
  if(command == 'h' || command == 'H'){ //halt
    resolveHaltMotor(I2C);
    isTargetDefined = false;
  }
  else if(command == 's'){ //set speed
    resolveSetSpeedBin(I2C);
    isTargetDefined = false;
  }
  else if(command == 'd' || command == 'D'){ //set direction
    resolveSetDirection(I2C);
    isTargetDefined = false;
  }
  else if(command == 'b'){ //set both speed and direction
    resolveSetDirAndSpeedBin(I2C);
    isTargetDefined = false;
  }
  else if(command == 'f'){
    resolveRunForwardBin(I2C);
    isTargetDefined = true;
  }
  else if(command == 'r'){
    resolveRunReverseBin(I2C);
    isTargetDefined = true;
  }
  else if(command == 't'){
    resolveTurnBin(I2C);
    isTargetDefined = true;
  }
  else if(!I2C){ //RS232 Serial commands only
    if(command == 'S'){ 
      resolveSetSpeedTex();
      isTargetDefined = false;
    }
    else if(command == 'B'){ 
      resolveSetDirAndSpeedTex();
      isTargetDefined = false;
    }
    else if(command == 'F'){
      resolveRunForwardTex();
      isTargetDefined = true;
    }
    else if(command == 'R'){
      resolveRunReverseTex();
      isTargetDefined = true;
    }
    else if(command == 'T'){
      resolveTurnTex();
      isTargetDefined = true;
    }
    else if(command == 'c' || command == 'C'){
      respondToConnection();
    }
    else if(command == 'e'){
      outputEncoderBinary();
    }
    else if(command == 'E'){
      outputEncoderText();
    }
  }
  else{
    testForToken(command);
  }
}

inline void resolveHaltMotor(bool I2C){
  char motor = readChar(I2C);

  if(motor == '1'){
    stopMotor1();
  }
  else if(motor == '2'){
    stopMotor2();
  }
  else if(motor == 'b'){
    stopMotor1();
    stopMotor2();
  }
  else {
    testForToken(motor);
  }
}

inline void resolveSetSpeedBin(bool I2C){
  char motor = readChar(I2C);

  

  if(motor == '1'){
    readSpeed1Bin(I2C);
    updateMotor1();
  }
  else if(motor == '2'){
    readSpeed2Bin(I2C);
    updateMotor2();
  }
  else if(motor == 'b'){
    readSpeed1Bin(I2C);
    readSpeed2Bin(I2C);
    updateMotors();
  }
  else {
    testForToken(motor);
  }
}

inline void resolveSetSpeedTex(){
  char motor = readChar(false);

  if(motor == '1'){
    readSpeed1Tex();
    updateMotor1();
  }
  else if(motor == '2'){
    readSpeed2Tex();
    updateMotor2();
  }
  else if(motor == 'b'){
    readSpeed1Tex();
    readSpeed2Tex();
    updateMotors();
  }
  else {
    testForToken(motor);
  }
}

inline void resolveSetDirection(bool I2C){
  char motor = readChar(I2C);

  if(motor == '1'){
    readDirection1(I2C);
    updateMotor1();
  }
  else if(motor == '2'){
    readDirection2(I2C);
    updateMotor2();
  }
  else if(motor == 'b'){
    readDirection1(I2C);
    readDirection2(I2C);
    updateMotors();
  }
  else{
    testForToken(motor);
  }
}

inline void resolveSetDirAndSpeedBin(bool I2C){
  char motor = readChar(I2C);
  if(motor == '1'){ //this structure repeating is important to bypass mistakes
    readDirection1(I2C);
    readSpeed1Bin(I2C);
    updateMotor1();
  }
  else if(motor == '2'){
    readDirection2(I2C);
    readSpeed2Bin(I2C);
    updateMotor2();
  }
  else if(motor == 'b'){
    readDirection1(I2C);
    readDirection2(I2C);
    readSpeed1Bin(I2C);
    readSpeed2Bin(I2C);
    updateMotors();
  }
  else{
    testForToken(motor);
  }
}

inline void resolveSetDirAndSpeedTex(){
  char motor = readChar(false);

  if(motor == '1'){ //this structure repeating is important to bypass mistakes
    readDirection1(false);
    readSpeed1Tex();
    updateMotor1();
  }
  else if(motor == '2'){
    readDirection2(false);
    readSpeed2Tex();
    updateMotor2();
  }
  else if(motor == 'b'){
    readDirection1(false);
    readDirection2(false);
    readSpeed1Tex();
    readSpeed2Tex();
    updateMotors();
  }
  else{
    testForToken(motor);
  }
}

inline void resolveRunForwardBin(bool I2C){
  dir1 = DIRF;
  dir2 = DIRF;
  readTargetBin(I2C);
}
inline void resolveRunForwardTex(){
  dir1 = DIRF;
  dir2 = DIRF;
  readTargetTex();
}

inline void resolveRunReverseBin(bool I2C){
  dir1 = DIRR;
  dir2 = DIRR;
  readTargetBin(I2C);
}
inline void resolveRunReverseTex(){
  dir1 = DIRR;
  dir2 = DIRR;
  readTargetTex();
}

inline void resolveTurnBin(bool I2C){
  char dir = readChar(I2C);

  if(dir == '1'){
    dir1 = DIRR;
    dir2 = DIRF;
    readTargetBin(I2C);
  }
  else if(dir == '2'){
    dir1 = DIRF;
    dir2 = DIRR;
    readTargetBin(I2C);
  }
}

inline void resolveTurnTex(){
  char dir = readChar(false);

  if(dir == '1'){
    dir1 = DIRR;
    dir2 = DIRF;
    readTargetTex();
  }
  else if(dir == '2'){
    dir1 = DIRF;
    dir2 = DIRR;
    readTargetTex();
  }
}

inline void readTargetBin(bool I2C){
  readSpeed1Bin(I2C);
  readSpeed2Bin(I2C);
  readLongIntBin(&target1,I2C);
  readLongIntBin(&target2,I2C);
  updateMotors();
  target1 += turns1;
  target2 += turns2;  
}

inline void readTargetTex(){
  readSpeed1Tex();
  readSpeed2Tex();
  target1 = readInt3Digits();
  target2 = readInt3Digits();
  updateMotors();
  target1 += turns1;
  target2 += turns2;  
}

inline void respondToConnection(){
  char message[] = "#okxx";
  message[3] = readChar(false);
  message[4] = readChar(false);

  Serial.write(message,6);
}

inline void respondToConnectionI2C(){
  Serial.write('o');
  Serial.write('k');
}

inline void outputEncoderBinary(){
  char motor = readChar(false);

  if(motor == '1'){
    Serial.write((byte *)&turns1,4);
  }
  if(motor == '2'){
    Serial.write((byte *)&turns2,4);
  }
  if(motor == 'b'){
    Serial.write((byte *)&turns1,4);
    Serial.write((byte *)&turns2,4);
  }
  else{
    testForToken(motor);
  }
}

inline void outputEncoderI2C(){
  Wire.write((byte *)&turns1,4);
  Wire.write((byte *)&turns2,4);
}

inline void outputEncoderText(){
  char motor = readChar(false);
  if(motor == '1'){
    Serial.println(turns1);
  }
  if(motor == '2'){
    Serial.println(turns2);
  }
  if(motor == 'b'){
    Serial.println(turns1);
    Serial.println(turns2);
  }
  else{
    testForToken(motor);
  }
}

inline void readDirection1(bool I2C){
  char dir = readChar(I2C);

  if(dir == 'f'){
    dir1 = DIRF;
  }
  else if(dir == 'r'){
    dir1 = DIRR;
  }
  else{
    testForToken(dir);
  }
  
}

inline void readDirection2(bool I2C){
  char dir = readChar(I2C);

  if(dir == 'f'){
    dir2 = DIRF;
  }
  else if(dir == 'r'){
    dir2 = DIRR;
  }
  else{
    testForToken(dir);
  }
}

inline void readSpeed1Bin(bool I2C){
  readIntBin(&speed1, I2C);
}

inline void readSpeed2Bin(bool I2C){
  readIntBin(&speed2, I2C);
}

inline void readSpeed1Tex(){
  speed1 = readInt3Digits();
}

inline void readSpeed2Tex(){
  speed2 = readInt3Digits();
}

inline void updateMotor1(){
  MOTOR.setSpeedDir1(speed1,dir1);
}

inline void updateMotor2(){
  MOTOR.setSpeedDir2(speed2,dir2);
}

inline void updateMotors(){
  updateMotor1();
  updateMotor2();
}

inline void stopMotor1(){
  MOTOR.setStop1();
  speed1 = 0;
  dir1 = DIRF;
}

inline void stopMotor2(){
  MOTOR.setStop2();
  speed2 = 0;
  dir2 = DIRF;
}

inline void pollMotors(){
  pollMotor1();
  pollMotor2();
}

inline void pollMotor1(){
  if(reachedTarget1()){
    stopMotor1();
  }
}

inline void pollMotor2(){
  if(reachedTarget2()){
    stopMotor2();
  }
}

inline bool reachedTarget1(){
  return turns1 >= target1;
}

inline bool reachedTarget2(){
  return turns2 >= target2;
}

inline bool carStopped(){
  return speed1 == 0 && speed2 == 0;
}
//blocks until a number of 3 digits are available on serial port
inline int readInt3Digits(){
  while(Serial.available() < 3) {}
  return Serial.parseInt();
}

inline void readIntBin(int *num, bool I2C){
  byte *numB = (byte*)num;
  numB[0] = readChar(I2C);
  numB[1] = readChar(I2C);
}

inline void readLongIntBin(unsigned long int *num, bool I2C){
  byte *numB = (byte*)num;
  numB[0] = readChar(I2C);
  numB[1] = readChar(I2C);
  numB[2] = readChar(I2C);
  numB[3] = readChar(I2C);
}

//blocks until another byte is available on serial port
inline char readChar(bool I2C){
  if(I2C){
    return Wire.read();
  }
  else{
    while(Serial.available()<1){}
    return Serial.read();
  }
}

