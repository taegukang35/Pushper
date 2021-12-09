#include<Wire.h>

// Set the necessary variables.
const int MPU_ADDR = 0x68;    // Address of MPU6050 for I2C communication
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;   // Acceleration and Gyro
double angleAcX;
double angleAcY;
const double RADIAN_TO_DEGREE = 180 / 3.14159;
int trig = 13;
int echo = 12; 
int speakerpin = 11; 
int piezo = 3;

//Pin mode setting
void setup() {
  initSensor();
  pinMode(trig,OUTPUT);
  pinMode(piezo, OUTPUT);
  pinMode(echo,INPUT);
  pinMode(sw, INPUT_PULLUP);
  Serial.begin(9600);
  delay(100);
}

void loop() {
  // Distance measurement(cm) with ultrasonic sensor
  digitalWrite(trig,HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig,LOW); 
  int distance = pulseIn(echo,HIGH)*17/1000; 
  getAngleXY(); //calculate angleAcX and angleAcY
  // Buzzer sounds when wrong posture is judged
  if (distance < 25 && angleAcX < -35){
    tone(piezo,500);
    delay(500);
    noTone(piezo);
    delay(500);
  } 
  // Send distance information through serial communication.
  Serial.print(distance);
  Serial.println();
  delay(200);
}

double getAngleXY() {
  // Finding the angle of a roll,pitch using trigonometric functions 
  getData();  
  angleAcX = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2)));
  angleAcX *= RADIAN_TO_DEGREE;
  angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
  angleAcY *= RADIAN_TO_DEGREE;
}

void initSensor() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);   // Address (address) for I2C communication
  Wire.write(0x6B);    // In order to start communication with MPU6050, address 0x6B    
  Wire.write(0);
  Wire.endTransmission(true);
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);   // Refers to the AcX register location (address)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);  //Request 14 bytes of data after AcX address
  AcX = Wire.read() << 8 | Wire.read(); //Concatenate two separate bytes into one and store them in each variable
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}
