#include <vector>
#include "Button.cpp"
#include "Pot.cpp"
#include "MidiController.cpp"
#include "LedFX.cpp"

#define BUTTON_START_PIN 15
#define POT_START_PIN A5
#define LED_PIN 13
#define EXT_LED_PIN_START 14
#define MIDI_SEND_INTERVAL_MS 200

int num_controllers = 2;
int num_buttons = 2;
int num_leds = 2;
int num_pots = 1;

bool midi_learn_mode = false;

long last_midi_send_ms = 0;

std::vector<Button> buttons;
std::vector<Pot> pots;
std::vector<MidiController> midi_controllers;
std::vector<LedFX> leds;

void setup() {
  midi_controllers.reserve(num_controllers);
  buttons.reserve(num_buttons);
  leds.reserve(num_leds);
  pots.reserve(num_pots);

  leds.emplace_back(LED_PIN);
  leds.emplace_back(EXT_LED_PIN_START);

  pots.emplace_back(POT_START_PIN);

  midi_controllers.emplace_back(67, 0);
  midi_controllers.emplace_back(68, 0);

  midi_controllers.emplace_back(69, 0);
  midi_controllers.emplace_back(70, 0);
  midi_controllers.emplace_back(71, 0);
  midi_controllers.emplace_back(72, 0);
  
  buttons.emplace_back(BUTTON_START_PIN);
  buttons.emplace_back(BUTTON_START_PIN + 1);

  pots[0].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[2].set(midi_value);
    leds[1].setColorOverlay(midi_value / 127.0, 500);
  };

  buttons[0].on_press = [&]() {
    controller_moved();
    int state = midi_controllers[0].toggleState();
    leds[0].setColor(state / 127.0);
  };

  buttons[1].on_press = [&]() {
    controller_moved();
    int state = midi_controllers[1].toggleState();
    leds[1].setColor(state / 127.0);
  };
}

void controller_moved() {
  set_midi_learn(false);
}

void set_midi_learn(bool enabled) {
  if (midi_learn_mode == enabled) return;
  midi_learn_mode = enabled;
  last_midi_send_ms = millis();
}

void loop() {
  for (auto& button: buttons) {
    button.update();
  }

  if (buttons[0].is_pressed() && buttons[1].is_pressed()) {
    set_midi_learn(true);
  }

  for (auto& pot: pots) {
    pot.update(5);
  }

  // Send MIDI state on regular intervals
  if (!midi_learn_mode && millis() - last_midi_send_ms > MIDI_SEND_INTERVAL_MS) {
    last_midi_send_ms = millis();
    for (auto& midi_controller: midi_controllers) {
      midi_controller.sendStateAsMIDI();
    }
  }

  if (midi_learn_mode) {
    // Alternate leds
    leds[0].setColorOverlay((millis() % 1500) > 750 ? 1.0f : 0.0f, 0);
    leds[1].setColorOverlay(((millis() + 750) % 1500) > 750 ? 1.0f : 0.0f, 0);
  }

  for (auto& led: leds) {
    led.update(5);
  }
  
  delay(5);
}
