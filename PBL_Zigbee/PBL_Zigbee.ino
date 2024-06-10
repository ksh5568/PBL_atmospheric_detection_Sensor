#include <Arduino.h>
#include <glcd.h>
#include <fonts/allFonts.h>

const unsigned long interval = 2000; // 2초 (단위: 밀리초)
unsigned long previousMillis = 0;

void setup() {
  // XBee의 기본 통신 속도는 9600bps입니다.
  Serial.begin(9600);
  GLCD.Init();
  GLCD.SelectFont(System5x7);
}

void loop() {
  if (Serial.available() > 0) // More conventional to check if greater than 0
  {
    String receivedData = Serial.readString(); // Clear the screen before writing new text
    GLCD.println(receivedData);

    String msg = receivedData;
  
    char sendData[500];
    // 일정 시간(interval)마다 데이터 전송
    unsigned long currentMillis = millis();  
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis; // 이전 시간 업데이트
      sprintf(sendData, "%s", msg.c_str());
      Serial.write(sendData);
    }
  }  
  delay(1000);
}
