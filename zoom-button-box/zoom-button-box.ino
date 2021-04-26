#include <Keyboard.h>

const int RXLED = 17;  // The RX LED has a defined Arduino pin
const int BUTTONPIN1 = 9;
const int BUTTONPIN2 = 8;
const int BUTTONPIN3 = 7;
const int BUTTONPIN4 = 6;

// current state of the button
int buttonState1 = 0;         
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

// previous state of the button
int lastButtonState1 = 0;
int lastButtonState2 = 0;
int lastButtonState3 = 0;
int lastButtonState4 = 0;

void setup() {
  pinMode(RXLED, OUTPUT);  // Set RX LED as an output
  digitalWrite(RXLED, HIGH);   // set the RX LED OFF

  pinMode(BUTTONPIN1, INPUT);
  digitalWrite(BUTTONPIN1, HIGH);  // Pull the button high

  pinMode(BUTTONPIN2, INPUT);
  digitalWrite(BUTTONPIN2, HIGH);  // Pull the button high

  pinMode(BUTTONPIN3, INPUT);
  digitalWrite(BUTTONPIN3, HIGH);  // Pull the button high

  pinMode(BUTTONPIN4, INPUT);
  digitalWrite(BUTTONPIN4, HIGH);  // Pull the button high

  Keyboard.begin();
}

void loop() {
  buttonState1 = digitalRead(BUTTONPIN1);
  buttonState2 = digitalRead(BUTTONPIN2);
  buttonState3 = digitalRead(BUTTONPIN3);
  buttonState4 = digitalRead(BUTTONPIN4);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      digitalWrite(RXLED, LOW);   // set the RX LED ON
      // MUTE TOGGLE COMBO
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('a');
      delay(100);              // wait
      Keyboard.releaseAll();
    }
  }

  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      digitalWrite(RXLED, LOW);   // set the RX LED ON
      // VIDEO TOGGLE COMBO
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('v');
      delay(100);              // wait
      Keyboard.releaseAll();
    }
  }

  if (buttonState3 != lastButtonState3) {
    if (buttonState3 == LOW) {
      digitalWrite(RXLED, LOW);   // set the RX LED ON
      // SHARE SCREEN
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('s');
      delay(100);              // wait
      Keyboard.releaseAll();
    }
  }

  if (buttonState4 != lastButtonState4) {
    if (buttonState4 == LOW) {
      digitalWrite(RXLED, LOW);   // set the RX LED ON
      // END MEETING COMBO
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press('w');
      delay(100);              // wait
      Keyboard.releaseAll();
    }
  }

  delay(100);              // wait to debounce
  digitalWrite(RXLED, HIGH);   // set the RX LED OFF

  // save the current state as the last state, for next time through the loop
  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  lastButtonState4 = buttonState4;
}
