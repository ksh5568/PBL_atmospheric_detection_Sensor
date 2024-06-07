#include <MQUnifiedsensor.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <swRTC.h>

//Basic Setting
#define Board "Arduino MEGA 2560"
#define Voltage_Resolution 5
#define ADC_Bit_Resolution 10 // For arduino UNO/MEGA/NANO

//Ratio CleanAir Default
#define RatioMQ2CleanAir 9.83
#define RatioMQ3CleanAir 60
#define RatioMQ5CleanAir 6.5
#define RatioMQ9CleanAir 9.6
#define RatioMQ135CleanAir 3.6

//Sensor Type
#define Type_A "MQ-2"
#define Type_B "MQ-3"
#define Type_C "MQ-5"
#define Type_D "MQ-9"
#define Type_E "MQ-135"

//Analog input
#define Pin_A A0
#define Pin_B A1
#define Pin_C A2
#define Pin_D A3
#define Pin_E A4

//Declare Sensor
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin_A, Type_A);
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin_B, Type_B);
MQUnifiedsensor MQ5(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin_C, Type_C);
MQUnifiedsensor MQ9(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin_D, Type_D);
MQUnifiedsensor MQ135(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin_E, Type_E);

swRTC rtc;

const unsigned long interval = 2000;
unsigned long previousMillis = 0;
String msg;

// LED pins
const int redLED = 5;
const int yellowLED = 4;
const int whiteLED = 3;
int hazardCount = 0;

void setup() {
  Serial.begin(9600);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(whiteLED, OUTPUT);

  setupSensor(MQ2, RatioMQ2CleanAir, "MQ-2");
  setupSensor(MQ3, RatioMQ3CleanAir, "MQ-3");
  setupSensor(MQ5, RatioMQ5CleanAir, "MQ-5");
  setupSensor(MQ9, RatioMQ9CleanAir, "MQ-9");
  setupSensor(MQ135, RatioMQ135CleanAir, "MQ-135");

  swrtc_setup();

  Serial.print("Date :");
  Serial.print(rtc.getDay(), DEC);
  Serial.print("/");
  Serial.print(rtc.getMonth(), DEC);
  Serial.print("/");
  Serial.println(rtc.getYear(), DEC);
}
  
void loop() {
  hazardCount = 0;  // Reset hazard count on each loop iteration
  MQ2.update();
  MQ3.update();
  MQ5.update();
  MQ9.update();
  MQ135.update();

  Serial.print("|  ");
  Serial.print(rtc.getHours(), DEC);
  Serial.print(":");
  Serial.print(rtc.getMinutes(), DEC);
  Serial.print(":");
  Serial.print(rtc.getSeconds(), DEC);
  Serial.println("  |");

  // Evaluate each sensor
  printSensorData(MQ2, 1000.0, 658.71, -2.168, "Propane");
  printSensorData(MQ3, 1.0, 4.8387, -2.68, "Benzene");
  printSensorData(MQ3, 50.0, 7585.3, -2.849, "Hexane");
  printSensorData(MQ5, 40000, 1163.8, -3.874, "H2");
  printSensorData(MQ5, 1000.0, 97124, -4.918, "Alcohol");
  printSensorData(MQ9, 1000.0, 1000.5, -2.186, "LPG");
  printSensorData(MQ9, 5000.0, 4269.6, -2.648, "CH4");
  printSensorData(MQ9, 35.0, 599.65, -2.244, "CO");
  printSensorData(MQ135, 5000.0, 110.47, -2.862, "CO2");
  printSensorData(MQ135, 100.0, 44.947, -3.445, "Toluen");
  printSensorData(MQ135, 25.0, 102.2, -2.473, "NH4");
  printSensorData(MQ135, 750.0, 34.668, -3.369, "Aceton");

  // Update LED status based on hazard count
  if (hazardCount >= 3) {
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(whiteLED, LOW);
  } else if (hazardCount > 0) {
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(whiteLED, LOW);
  } else {
    digitalWrite(whiteLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(yellowLED, LOW);
  }

  delay(1000); // Sampling frequency
}

void setupSensor(MQUnifiedsensor& sensor, float ratioCleanAir, String sensorType) {
  sensor.setRegressionMethod(1); // _ppm = a * ratio^b
  sensor.init();
  
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    sensor.update();
    calcR0 += sensor.calibrate(ratioCleanAir);
    Serial.print(".");
  }
  sensor.setR0(calcR0 / 10);
  Serial.println("  done!.");
  
  checkSensorConnection(calcR0);
}

void checkSensorConnection(float calcR0) {
  if (isinf(calcR0)) {
    Serial.println("Warning: Connection issue, R0 is infinite (Open circuit detected) please check your wiring and supply");
    while (1);
  }
  if (calcR0 == 0) {
    Serial.println("Warning: Connection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply");
    while (1);
  }
}

void printSensorData(MQUnifiedsensor &sensor, float threshold, float a, float b, String gasType) {
  String sensorData;
  sensor.update();
  sensor.setA(a);
  sensor.setB(b);
  float ppm = sensor.readSensor();
  if (ppm > threshold) {  // 예시로 설정된 위험 수치입니다. 실제 수치로 교체해야 합니다.
    hazardCount++;
  }
  sensorData += gasType + " : " + ppm + "\n";
  
  msg = sensorData;
  Serial.println(sensorData);
  char sendData[100];
   // 일정 시간(interval)마다 데이터 전송d
  unsigned long currentMillis = millis();  
   if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // 이전 시간 업데이트
    sprintf(sendData, "%s", msg.c_str());
    Serial.write(sendData);
   }
}

void swrtc_setup() {
  rtc.stopRTC();          // 정지
  rtc.setTime(0, 0, 0);   // 시간, 분, 초 초기화
  rtc.setDate(16, 4, 2024); // 일, 월, 년도 초기화
  rtc.startRTC();         // 시작
}
