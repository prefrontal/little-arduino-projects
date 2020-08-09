/*
Westival Sound Machine
Arduino + Papa Soundie SparkFun Board
Craig Bennett - July 2019
*/

#include "SparkFun_PapaSoundie.h"

PapaSoundie sfx = PapaSoundie();

void setup() {
  Serial.begin(115200);
  Serial.println("Getting ready");

  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  sfx.begin();
}

// the loop function runs over and over again forever
void loop() {
    static bool playState = false;

    bool buttonState1 = digitalRead(9);
    bool buttonState2 = digitalRead(10);

    if (not buttonState1 and not playState) {
      // Play sound file #1
      Serial.println("Playing 1!");
      sfx.playFileNumber(2);
      playState = true;
    }
    else if (not buttonState2 and not playState) {
      // Play sound file #2
      Serial.println("Playing 2!");
      sfx.playFileNumber(3);
      playState = true;
    }
    else if (buttonState1 and buttonState2 and playState) {
      // Stop playing if one of the play buttons is no longer enabled
      // There is no effective way to make the Papa Soundie pause
      // Instead, play a very short sound file of silence to stop
      Serial.println("Stopping...");
      sfx.playFileNumber(1);
      playState = false;
    }
    
    # Debouncing wait
    delay(500);
}
