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
    Serial.println(receivedData);
   
    if (receivedData == "Pro"){
    GLCD.print("Propane Warning");
    }
    else if (receivedData == "Ben"){
     GLCD.print("Benzene Warning");
    }
    else if (receivedData == "Hex"){
    GLCD.print("Hexane Warning");
    }
    else if (receivedData == "H"){
    GLCD.print("H2 Warning");
    }
    else if (receivedData == "Alc"){
    GLCD.print("Alcohol Warning");
    }
    else if (receivedData == "L"){
    GLCD.print("LPG Warning");
    }
    else if (receivedData == "Ch"){
    GLCD.print("CH4 Warning");
    }
    else if (receivedData == "Co"){
    GLCD.print("CO Warning");
    }
    else if (receivedData == "Coo"){
    GLCD.print("CO2 Warning");
    }
    else if (receivedData == "Tol"){
    GLCD.print("Toluen Warning");
    }
    else if (receivedData == "Nh"){
    GLCD.print("NH4 Warning");
    }
    else if (receivedData == "Ace"){
    GLCD.print("Aceton Warning");
    }
  }
}
