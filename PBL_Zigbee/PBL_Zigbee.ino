#include <Arduino.h>
#include <glcd.h>
#include <fonts/allFonts.h>

void setup()
{
  Serial.begin(9600);
  // Initialize the GLCD 
  GLCD.Init();
  GLCD.SelectFont(System5x7);
}

void loop()
{
  if (Serial.available())
  {
    String receivedData = Serial.readString();
    GLCD.print(receivedData + "Warning");
    Serial.print(receivedData + "Warning")
  }
}
