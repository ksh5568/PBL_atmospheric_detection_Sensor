#include <Arduino.h>
#include <glcd.h>
#include <fonts/allFonts.h>

void setup()
{
    Serial.begin(9600);
    GLCD.Init();
    GLCD.SelectFont(System5x7);
}

void loop()
{
  if (Serial.available())
  {
    String receivedData = Serial.readString();
    GLCD.println(receivedData);
  }
}
