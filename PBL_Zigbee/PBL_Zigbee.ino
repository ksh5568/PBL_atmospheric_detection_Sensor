#include <Arduino.h>
#include <glcd.h>
#include <fonts/allFonts.h>

void setup()
{
  Serial.begin(9600);
  // Initialize the GLCD 
  GLCD.Init();

  // Select the font for the default text area
  GLCD.SelectFont(System5x7);
}

void loop()
{
  GLCD.CursorTo(0, 1);
  
  char test_A = Serial.read();
  if (test_A == 'A'){
   GLCD.print("Ready!"); 
  }
  
  if (Serial.available())
  {
    String receivedData = Serial.readString();
    GLCD.print(receivedData + "Warning");
  }
}
