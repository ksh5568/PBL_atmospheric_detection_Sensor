#include <MQUnifiedsensor.h>
#include <Arduino.h>

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

const unsigned long interval = 2000; // 2초 (단위: 밀리초)
unsigned long previousMillis = 0;

void setup() {
 
  Serial.begin(9600);
  MQ2Sensor_Setup();
  MQ3Sensor_Setup();
  MQ5Sensor_Setup();
  MQ9Sensor_Setup();
  MQ135Sensor_Setup();

  Serial.println("------------------------------------------------------------------------------------------------------------------------------------");
  Serial.print("| Propane |  Benzene |  Hexane  |    H2    |  Alcohol |    LPG   |    CH4   |    CO    |");
  Serial.println("    CO2   |  Toluen  |    NH4   |  Aceton  |");
  Serial.println("------------------------------------------------------------------------------------------------------------------------------------");
}


void loop() {
  //MQ2 Sensor
  MQ2Sensor_Print();

  //MQ3 Sensor
  MQ3Sensor_Print();

  //MQ5 Sensor
  MQ5Sensor_Print();
 
  //MQ9 Sensor
  MQ9Sensor_Print();

  //MQ135Sensor
  MQ135Sensor_Print();

  Serial.println();
  delay(1000);
}

void MQ2Sensor_Setup() {
  //MQ2Sensor Default
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
 
  MQ2.init();

  float calcR0_A = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ2.update(); // Update data, the arduino will read the voltage from the analog Pin_A
    calcR0_A += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  MQ2.setR0(calcR0_A/10);
  Serial.println("  done!.");

  if(isinf(calcR0_A)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0_A == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog Pin_A shorts to ground) please check your wiring and supply"); while(1);}
}

void MQ2Sensor_Print(){
  MQ2.update();

  MQ2.setA(658.71); MQ2.setB(-2.168);
  float Propane = MQ2.readSensor();

  Serial.print("|");
  String propaneStr = String(Propane, 2); // 소수점 둘째자리까지 문자열로 변환
  printPadded(propaneStr, 8); // 8자리 폭으로 출력
  Serial.print(" | ");

  float PropaneThreshold = 2000.0; 

  String Pro;

  if(Propane > PropaneThreshold) {
    Send_msg(Pro);
  }
}

void MQ3Sensor_Setup(){
  MQ3.setRegressionMethod(1); //_PPM =  a*ratio^b
 
  MQ3.init();
 
  float calcR0_B = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ3.update(); // Update data, the arduino will read the voltage from the analog Pin_B
    calcR0_B += MQ3.calibrate(RatioMQ3CleanAir);
    Serial.print(".");
  }
  MQ3.setR0(calcR0_B/10);
  Serial.println("  done!.");
 
  if(isinf(calcR0_B)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0_B == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog Pin_B_Bshorts to ground) please check your wiring and supply"); while(1);}
}


void MQ3Sensor_Print(){
  MQ3.update();
 
  MQ3.setA(4.8387); MQ3.setB(-2.68);
  float Benzene = MQ3.readSensor();

  MQ3.setA(7585.3); MQ3.setB(-2.849);
  float Hexane = MQ3.readSensor();

  String BenzeneStr = String(Benzene, 2); 
  printPadded(BenzeneStr, 8); 
  Serial.print(" | ");

  String HexaneStr = String(Hexane, 2); // 소수점 둘째자리까지 문자열로 변환
  printPadded(HexaneStr, 8); // 8자리 폭으로 출력
  Serial.print(" | ");

  // 각 센서 값에 대한 기준치 설정
  float BenzeneThreshold = 100.0; // Benzene 기준치 예시
  float HexaneThreshold = 50.0; // Hexane 기준치 예시

  String Ben;
  String Hex;

  if(Benzene > BenzeneThreshold) {
    Send_msg(Ben);
  }
  if(Hexane > HexaneThreshold) {
    Send_msg(Hex);
  }
}

void MQ5Sensor_Setup(){
  MQ5.setRegressionMethod(1); //_PPM =  a*ratio^b
 
  MQ5.init();

  float calcR0_C = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ5.update(); // Update data, the arduino will read the voltage from the analog Pin_C
    calcR0_C += MQ5.calibrate(RatioMQ5CleanAir);
    Serial.print(".");
  }
  MQ5.setR0(calcR0_C/10);
  Serial.println("  done!.");
 
  if(isinf(calcR0_C)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0_C == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog Pin_C shorts to ground) please check your wiring and supply"); while(1);}
}

void MQ5Sensor_Print(){
  MQ5.update();
 
  MQ5.setA(1163.8); MQ5.setB(-3.874);
  float H2 = MQ5.readSensor();

  MQ5.setA(97124); MQ5.setB(-4.918);
  float Alcohol = MQ5.readSensor();

  String H2Str = String(H2, 2); 
  printPadded(H2Str, 8); 
  Serial.print(" | ");

  String AlcoholStr = String(Alcohol, 2); 
  printPadded(AlcoholStr, 8); 
  Serial.print(" | ");

  float H2Threshold = 1000.0; // H2 기준치 예시
  float AlcoholThreshold = 400.0; // Alcohol 기준치 예시

  String H;
  String Alc;

  if(H2 > H2Threshold) {
    Send_msg(H);
  }
  if(Alcohol > AlcoholThreshold) {
    Send_msg(Alc); 
  }
}

void MQ9Sensor_Setup(){

  MQ9.setRegressionMethod(1); //_PPM =  a*ratio^b
 
  MQ9.init();

  float calcR0_D = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ9.update(); // Update data, the arduino will read the voltage from the analog Pin_D
    calcR0_D += MQ9.calibrate(RatioMQ9CleanAir);
    Serial.print(".");
  }
  MQ9.setR0(calcR0_D/10);
  Serial.println("  done!.");
 
  if(isinf(calcR0_D)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0_D == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog Pin_D shorts to ground) please check your wiring and supply"); while(1);}
}

void MQ9Sensor_Print(){
  MQ9.update();
 
  MQ9.setA(1000.5); MQ9.setB(-2.186);
  float LPG = MQ9.readSensor();

  MQ9.setA(4269.6); MQ9.setB(-2.648);
  float CH4 = MQ9.readSensor();

  MQ9.setA(599.65); MQ9.setB(-2.244);
  float CO = MQ9.readSensor();

  String LPGStr = String(LPG, 2); 
  printPadded(LPGStr, 8); 
  Serial.print(" | ");

  String CH4Str = String(CH4, 2); 
  printPadded(CH4Str, 8); 
  Serial.print(" | ");

  String COStr = String(CO, 2); 
  printPadded(COStr, 8); 
  Serial.print(" | ");

  float LPGThreshold = 2000.0; // LPG 기준치 예시
  float CH4Threshold = 5000.0; // CH4 기준치 예시
  float COThreshold = 30.0; // CO 기준치 예시

  String L;
  String Ch;
  String Co;

  if(LPG > LPGThreshold) {
    Send_msg(L);
  }
  if(CH4 > CH4Threshold) {
    Send_msg(Ch);
  }
  if(CO > COThreshold) {
    Send_msg(Co);
  }
}


void MQ135Sensor_Setup(){
  MQ135.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ135.setA(102.2); MQ135.setB(-2.473);
  MQ135.init();

  float calcR0_E = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ135.update(); // Update data, the arduino will read the voltage from the analog Pin_E
    calcR0_E += MQ135.calibrate(RatioMQ135CleanAir);
    Serial.print(".");
  }
  MQ135.setR0(calcR0_E/10);
  Serial.println("  done!.");
 
  if(isinf(calcR0_E)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0_E == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog Pin_E shorts to ground) please check your wiring and supply"); while(1);}
}

void MQ135Sensor_Print(){
  MQ135.update();
 
  MQ135.setA(110.47); MQ135.setB(-2.862);
  float CO2 = MQ135.readSensor();

  MQ135.setA(44.947); MQ135.setB(-3.445);
  float Toluen = MQ135.readSensor();

  MQ135.setA(102.2); MQ135.setB(-2.473);
  float NH4 = MQ135.readSensor();

  MQ135.setA(34.668); MQ135.setB(-3.369);
  float Aceton = MQ135.readSensor();

  String CO2Str = String(CO2, 2); 
  printPadded(CO2Str, 8); 
  Serial.print(" | ");

  String ToluenStr = String(Toluen, 2); 
  printPadded(ToluenStr, 8); 
  Serial.print(" | ");
  
  String NH4Str = String(NH4, 2); 
  printPadded(NH4Str, 8); 
  Serial.print(" | ");

  String AcetonStr = String(Aceton, 2); 
  printPadded(AcetonStr, 8); 
  Serial.print(" | ");

    // 기준치 설정
  float CO2Threshold = 1000.0; // CO2의 기준치 예시
  float ToluenThreshold = 300.0; // Toluen의 기준치 예시
  float NH4Threshold = 400.0; // NH4의 기준치 예시
  float AcetonThreshold = 250.0; // Aceton의 기준치 예시

  String Coo;
  String Tol;
  String Nh;
  String Ace;

  if(CO2 > CO2Threshold) {
    Send_msg(Coo);
  }
  if(Toluen > ToluenThreshold) {
    Send_msg(Tol);
  }
  if(NH4 > NH4Threshold) {
    Send_msg(Nh);
  }
  if(Aceton > AcetonThreshold) {
    Send_msg(Ace);
  }
  
}

void printPadded(String data, int width) {
  int padding = width - data.length(); // 필요한 패딩 계산
  for(int i = 0; i < padding; i++) {
    Serial.print(" "); // 공백으로 패딩 추가
  }
  Serial.print(data); // 데이터 출력
}

void Send_msg (String msg){
  unsigned long currentMillis = millis(); 
   
  // 일정 시간(interval)마다 데이터 전송
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
  Serial.write("msg");
}
}
