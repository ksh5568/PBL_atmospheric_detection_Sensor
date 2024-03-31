#include <MQUnifiedsensor.h>
#include <Arduino.h>
#include <Wire.h>
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

File dataFile;
swRTC rtc;

const unsigned long interval = 1000; // 2초 (단위: 밀리초)
unsigned long previousMillis = 0;
String msg;
String sensorData;

struct GasThreshold {
  String name;
  float threshold;
};
  
void setup() {
  Serial.begin(9600);

  if (!SD.begin(53)) { // SD카드 모듈을 초기화합니다.
    Serial.println("initialization failed!"); // SD카드 모듈 초기화에 실패하면 에러를 출력합니다.
    while (1);
  }
  
  setupSensor(MQ2, RatioMQ2CleanAir, "MQ-2");
  setupSensor(MQ3, RatioMQ3CleanAir, "MQ-3");
  setupSensor(MQ5, RatioMQ5CleanAir, "MQ-5");
  setupSensor(MQ9, RatioMQ9CleanAir, "MQ-9");
  setupSensor(MQ135, RatioMQ135CleanAir, "MQ-135");

  swrtc_setup();

  dataFile = SD.open("dataFile.txt", FILE_WRITE);
  if (dataFile){
    dataFile.print("Date : ");
    dataFile.print(rtc.getDay(), DEC);       //일 출력 
    dataFile.print("/");
    dataFile.print(rtc.getMonth(), DEC);     //월 출력
    dataFile.print("/");
    dataFile.println(rtc.getYear(), DEC);

    dataFile.println("-------------------------------------------------------------------------------------------------------------------------------------------------------------");
    dataFile.println("|  Time  | Propane |  Benzene |  Hexane  |    H2    |  Alcohol |    LPG   |    CH4   |    CO    |    CO2   |  Toluen  |    NH4   |  Aceton  |");
    dataFile.println("-------------------------------------------------------------------------------------------------------------------------------------------------------------");
    dataFile.close();
    Serial.println("Data saved to SD card: dataFile.txt");
    } else {
    Serial.println("Error opening file");
    }

  Serial.print("Date :");
  Serial.print(rtc.getDay(), DEC);       //일 출력 
  Serial.print("/");
  Serial.print(rtc.getMonth(), DEC);     //월 출력
  Serial.print("/");
  Serial.println(rtc.getYear(), DEC);
  Serial.println("---------------------------------------------------------------------------------------------------------------------------------------------------");
  Serial.println("|  Time  | Propane |  Benzene |  Hexane  |    H2    |  Alcohol |    LPG   |    CH4   |    CO    |    CO2   |  Toluen  |    NH4   |  Aceton  |");
  Serial.println("---------------------------------------------------------------------------------------------------------------------------------------------------");

  delay(1000);
}
  
void loop() {

  sensorData ="";

  // MQ-2 센서 데이터 출력
  printSensorData(MQ2, 658.71, -2.168, "Propane");

  // MQ-3 센서 데이터 출력
  printSensorData(MQ3, 4.8387, -2.68, "Benzene");
  printSensorData(MQ3, 7585.3, -2.849, "Hexane");

  // MQ-5 센서 데이터 출력
  printSensorData(MQ5, 1163.8, -3.874, "H2");
  printSensorData(MQ5, 97124, -4.918, "Alcohol");

  // MQ-9 센서 데이터 출력
  printSensorData(MQ9, 1000.5, -2.186, "LPG");
  printSensorData(MQ9, 4269.6, -2.648, "CH4");
  printSensorData(MQ9, 599.65, -2.244, "CO");

  // MQ-135 센서 데이터 출력
  printSensorData(MQ135, 110.47, -2.862, "CO2");
  printSensorData(MQ135, 44.947, -3.445, "Toluen");
  printSensorData(MQ135, 102.2, -2.473, "NH4");
  printSensorData(MQ135, 34.668, -3.369, "Aceton");

  Serial.print("|  ");
  Serial.print(rtc.getHours(), DEC);     //시간 출력
  Serial.print(":");
  Serial.print(rtc.getMinutes(), DEC);   //분 출력
  Serial.print(":");
  Serial.print(rtc.getSeconds(), DEC); //초 출력
  Serial.print("  |");
  Serial.println(sensorData); // 데이터 출력

  // 데이터를 SD 카드에 저장
  dataFile = SD.open("dataFile.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("|  ");
    dataFile.print(rtc.getHours(), DEC);     //시간 출력
    dataFile.print(":"); 
    dataFile.print(rtc.getMinutes(), DEC);   //분 출력
    dataFile.print(":");
    dataFile.print(rtc.getSeconds(), DEC); //초 출력
    dataFile.print("  |");
    dataFile.println(sensorData);  
    dataFile.close();
    Serial.println("Data saved to SD card: dataFile.txt");
    } else {
    Serial.println("Error opening file");
    }

  delay(1000);
}
  
void setupSensor(MQUnifiedsensor& sensor, float ratioCleanAir, String sensorType) {
  sensor.setRegressionMethod(1); //_PPM = a*ratio^b
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
  
void printSensorData(MQUnifiedsensor& sensor, float a, float b, String dataType) {
  sensor.update();
  sensor.setA(a); 
  sensor.setB(b);
  float reading = sensor.readSensor();
  sensorData += padToWidth(String(reading, 2), 8) + " | ";

  CheckThresholdAndWarn(dataType, reading);
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
  
String padToWidth(String data, int width) {
  while(data.length() < width) {
    data = " " + data; // 왼쪽에 공백 추가
  }
  return data;
}

void swrtc_setup() {
  rtc.stopRTC();           //정지
  rtc.setTime(0,0,0);    //시간, 분, 초 초기화
  rtc.setDate(29,3,2024);  //일, 월, 년도 초기화 
  rtc.startRTC();          //시작
}

void CheckThresholdAndWarn(String dataType, float reading) {
  // 가스별 임계값 정의
  GasThreshold thresholds[] = {
    {"Propane", 500.0},
    {"Benzene", 500.0},
    {"Hexane", 500.0},
    {"H2", 500.0},
    {"Alcohol", 500.0},
    {"LPG", 1.0},
    {"CH4", 500.0},
    {"CO", 500.0},
    {"CO2", 500.0},
    {"Toluen", 600.0},
    {"NH4", 1000.0},
    {"Aceton", 500.0}
  };
  
  // 가스 종류 개수
  int numGases = sizeof(thresholds) / sizeof(GasThreshold);

  // 가스별 임계값 검사
  for (int i = 0; i < numGases; i++) {
    if (dataType == thresholds[i].name && reading > thresholds[i].threshold) {
      Send_Warning(dataType);
      break; // 매칭되는 가스를 찾으면 반복 종료
    }
  }
}
  
void Send_Warning(String type){
  char send_type[50];
  // 일정 시간(interval)마다 데이터 전송
  unsigned long currentMillis = millis();  
  if (currentMillis - previousMillis >= interval) {
  previousMillis = currentMillis; // 이전 시간 업데이트
  sprintf(send_type, "%s", type.c_str());
  Serial.write(send_type);
   } 
}
