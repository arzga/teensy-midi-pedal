#include <vector>
#include "Button.cpp"
#include "MidiController.cpp"

#define BUTTON_START_PIN 14
#define LED_PIN 13
#define EXT_LED_PIN_START 16
#define MIDI_SEND_INTERVAL_MS 200

int num_controllers = 2;
int num_buttons = 2;

long last_midi_send_ms = 0;

std::vector<Button> buttons;
std::vector<MidiController> midi_controllers;

void setup() {
  midi_controllers.reserve(num_controllers);
  buttons.reserve(num_buttons);

  midi_controllers.emplace_back(67, false, LED_PIN);
  midi_controllers.emplace_back(68, false, EXT_LED_PIN_START);
  
  Button buttonA = Button(BUTTON_START_PIN);
  Button buttonB = Button(BUTTON_START_PIN + 1);

  buttonA.on_press = [&]() {
    midi_controllers[0].toggleState();
  };

  buttonB.on_press = [&]() {
    midi_controllers[1].toggleState();
  };

  buttons.push_back( buttonA );
  buttons.push_back( buttonB );
}

void loop() {
  for (unsigned int i=0; i<buttons.size(); i++) {
    buttons[i].update();
  }

  // Send MIDI state on regular intervals
  if (millis() - last_midi_send_ms > MIDI_SEND_INTERVAL_MS) {
    last_midi_send_ms = millis();
    for (unsigned int i=0; i<midi_controllers.size(); i++) {
      midi_controllers[i].sendStateAsMIDI();
    }
  }
}
