/*
Read data from a Qwiic SHTC3 temp/humidity sensor and display the value on a Qwiic alphanumeric display.
Date: 8/1/2020
By: Craig Bennett
*/

#include <Wire.h>
#include "SparkFun_Alphanumeric_Display.h"
#include "SparkFun_SHTC3.h"

SHTC3 mySHTC3;
HT16K33 display;

// Both devices ship with a 0x70 I2C address
// Gotta solder one of the pads on the display to change it to 0x71
uint8_t displayAddress = 0x71;

void setup()
{
  Serial.begin(9600);
  Serial.println("SHTC3 Temperature Display");

  // Initialize the I2C bus
  Wire.begin(); 

  if (mySHTC3.begin() != SHTC3_Status_Nominal) {
    Serial.println("SHTC3 did not start properly. Freezing.");
    while (1);
  }

  if (display.begin(displayAddress) != true)
  {
    Serial.println("Display did not start properly. Freezing.");
    while (1);
  }
}

void loop()
{
  SHTC3_Status_TypeDef tempUpdateResult = mySHTC3.update();

  if (tempUpdateResult != SHTC3_Status_Nominal) {
    Serial.println("Bad update result from SHTC3. Ignoring...");
  }
  
  float currentTemp = mySHTC3.toDegF();
  float currentHumidity = mySHTC3.toPercent();
  Serial.println(String(currentTemp) + "F " + String(currentHumidity) + "%\n");

  UpdateAnimation();
  String displayTemp = String(currentTemp);

  // There is no easy way to right-justify the temperature without a visible flicker as you shift right
  // Instead, add a space to the string here if needed, then update all characters at once
  if (currentTemp < 100) {
    displayTemp = " " + displayTemp;
  }

  // Display all the characters, ignoring the decimal in the string
  display.printChar(displayTemp.charAt(0), 0);
  display.printChar(displayTemp.charAt(1), 1);
  display.printChar(displayTemp.charAt(2), 2);
  display.printChar(displayTemp.charAt(4), 3);
  display.decimalOn();
  display.updateDisplay();
 
  delay(60000); // Wait for next reading
}


/*
Display a quick animation on the display to indicate device is busy
The display needs all characters defined to properly update the display. So, the strategy is to first put spaces
in for all characters, and then update the releavant character with a dash to show update progress.
*/
void UpdateAnimation() {
  for (int i = 0; i < 4; i++) {
    display.clear();
    display.printChar(' ', 0);
    display.printChar(' ', 1);
    display.printChar(' ', 2);
    display.printChar(' ', 3);
    
    display.printChar('-', i);
    display.updateDisplay();
    delay(250);
  }
}
