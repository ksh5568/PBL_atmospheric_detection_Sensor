#include <Arduino.h>
#include <glcd.h>
#include <fonts/allFonts.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available())
  {
    String receivedData = Serial.readString();
    Serial.println(receivedData);
   
    if (receivedData == "Pro"){
     

    }
    else if (receivedData == "Ben"){
     

    }
    else if (receivedData == "Hex"){
     

    }
    else if (receivedData == "H"){
     

    }
    else if (receivedData == "Alc"){
     

    }
    else if (receivedData == "L"){
     

    }
    else if (receivedData == "Ch"){
     

    }
    else if (receivedData == "Co"){
     

    }
    else if (receivedData == "Coo"){
     

    }
    else if (receivedData == "Tol"){
     

    }
    else if (receivedData == "Nh"){
     

    }
    else if (receivedData == "Ace"){
     

    }
  }
}
