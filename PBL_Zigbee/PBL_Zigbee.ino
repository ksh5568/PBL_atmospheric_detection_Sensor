#include <Arduino.h>
#include <glcd.h>
#include <fonts/allFonts.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  float PropaneThreshold = 2000.0; 
  float BenzeneThreshold = 100.0; // Benzene 기준치 예시
  float HexaneThreshold = 50.0; // Hexane 기준치 예시
  float H2Threshold = 1000.0; // H2 기준치 예시
  float AlcoholThreshold = 400.0; // Alcohol 기준치 예시
  float LPGThreshold = 2000.0; // LPG 기준치 예시
  float CH4Threshold = 5000.0; // CH4 기준치 예시
  float COThreshold = 30.0; // CO 기준치 예시
  float CO2Threshold = 1000.0; // CO2의 기준치 예시
  float ToluenThreshold = 300.0; // Toluen의 기준치 예시
  float NH4Threshold = 400.0; // NH4의 기준치 예시
  float AcetonThreshold = 250.0; // Aceton의 기준치 예시
  
  if (Serial.available())
  {
    String receivedType = Serial.readString();
    float receivedData = Serial.read();
    Serial.println(receivedType);
     if (receivedType = "Propane")
     {
      if (receivedData > PropaneThreshold){ 
        GLCD.print("Propane Warning");
      }
     }
     else if (receivedType = "Benzene"){
      if (receivedData > BenzeneThreshold){
        GLCD.print("Benzene Warning");  
      }
     }
     else if (receivedType = "Hexane"){
      if (receivedData > HexaneThreshold){
        GLCD.print("Hexane Warning");
      }
     }
     else if (receivedType = "H2"){
      if (receivedData > H2Threshold){
        GLCD.print("H2 Warning");
      }
    }
    else if (receivedType = "Alcohol"){
     if (receivedData > AlcoholThreshold){
       GLCD.print("Alcohol Warning");
     }
    }
    else if (receivedType = "LPG"){
      if (receivedData > LPGThreshold){
        GLCD.print("LPG Warning");
     }
    }
    else if (receivedType = "CH4"){
      if (receivedData > CH4Threshold){
        GLCD.print("CH4 Warning");
      }
    }
    else if (receivedType = "CO"){
      if (receivedData > COThreshold){
        GLCD.print("CO Warning");
      }
    }
    else if (receivedType = "CO2"){
      if (receivedData > CO2Threshold){
        GLCD.print("CO2 Warning");
      }
    }
    else if (receivedType = "Tolueun"){
      if (receivedData > ToluenThreshold){
        GLCD.print("Tolueun Warning");
      }
    }
    else if (receivedType = "NH4"){
      if (receivedData > NH4Threshold){
        GLCD.print("NH4 Warning");
      }
    }
    else if (receivedType = "Aceton"){
      if (receivedData > AcetonThreshold){
        GLCD.print("Aceton Warning");
      }
    }
  }
}
