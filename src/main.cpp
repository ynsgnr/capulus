#include <Arduino.h>
#include <persist.h>

#define INPUT_BUTTON 14
#define LED_OUT 2

bool buttonState = true;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  delay(100);
  Serial.println("\nSetting up");
  pinMode(LED_OUT, OUTPUT);
  pinMode(INPUT_BUTTON, INPUT);
  load();
  char tempStr[3];
  itoa(temp, tempStr, 10);
  Serial.println(tempStr);
}

// the loop function runs over and over again forever
void loop() {
  int current = digitalRead(INPUT_BUTTON);
  if (current == HIGH){
    Serial.println("pressed");
    buttonState = not buttonState;
    temp = random(255);
    store();
  }
  if (buttonState){
    digitalWrite(LED_OUT, HIGH);
  }else{
    digitalWrite(LED_OUT, LOW);
  }
}