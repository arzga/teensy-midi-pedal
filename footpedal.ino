#include <vector>
#include "Button.cpp"

#define BUTTON_START_PIN 14
#define LED_PIN 16

int num_buttons = 2;
int ledState = LOW;
std::vector<Button> buttons;

void setup() {
  
  buttons.reserve(num_buttons);

  buttons.push_back( Button(BUTTON_START_PIN, 67, false) );
  buttons.push_back( Button(BUTTON_START_PIN + 1, 67, true) );
  
  pinMode(LED_PIN,OUTPUT); // Setup the LED
  digitalWrite(LED_PIN,ledState); // Turn off the LED 
}

void loop() {
  bool pressed = false;
  for (int i=0; i<num_buttons; i++) {
    pressed |= buttons[i].update();
  }
  
  if (pressed) {
    ledState = !ledState; // Toggle LED state
    digitalWrite(LED_PIN,ledState); // Apply new LED state
  }
}
