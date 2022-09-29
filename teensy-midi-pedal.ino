#include <vector>
#include "Button.cpp"
#include "Pot.cpp"
#include "MidiController.cpp"
#include "LedFX.cpp"
#include "TableInterpolator.cpp"

#define BUTTON_L_PIN A0
#define BUTTON_R_PIN A9
#define POT_START_PIN A1
#define EXT_LED_L_PIN 3
#define EXT_LED_R_PIN 2
#define MIDI_SEND_INTERVAL_MS 200
#define LED_PIN 13

int num_controllers = 6;
int num_buttons = 2;
int num_pots = 4;
int num_leds = 2;

bool midi_learn_mode = false;

int intro_led_sweep_ms = 2500;

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

  leds.emplace_back(EXT_LED_L_PIN);
  leds.emplace_back(EXT_LED_R_PIN);

  pots.emplace_back(POT_START_PIN);
  pots.emplace_back(POT_START_PIN + 1);
  pots.emplace_back(POT_START_PIN + 2);
  pots.emplace_back(POT_START_PIN + 3);

  buttons.emplace_back(BUTTON_L_PIN);
  buttons.emplace_back(BUTTON_R_PIN);

  midi_controllers.emplace_back(67, 0); // Btn 0
  midi_controllers.emplace_back(68, 0);
  midi_controllers.emplace_back(69, 0); // Pot 0
  midi_controllers.emplace_back(70, 0);
  midi_controllers.emplace_back(71, 0);
  midi_controllers.emplace_back(72, 0);
  
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

  pots[0].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[2].set(midi_value);
    leds[0].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[1].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[3].set(midi_value);
    leds[0].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[2].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[4].set(midi_value);
    leds[1].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[3].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[5].set(midi_value);
    leds[1].setColorOverlay(midi_value / 127.0, 500);
  };
}

void controller_moved() {
  set_midi_learn(false);
  intro_led_sweep_ms = 0;
}

void set_midi_learn(bool enabled) {
  if (midi_learn_mode == enabled) return;
  midi_learn_mode = enabled;
  last_midi_send_ms = millis();
}

void loop() {
  int delay_ms = 5;

  if (intro_led_sweep_ms > 0) {
    intro_led_sweep_ms -= delay_ms;
    leds[0].setColorOverlay(TableInterpolator::interpolateLinearf(gaussian, ARRAYLEN(gaussian), intro_led_sweep_ms, 250.0f, 2500.0f), 0);
    leds[1].setColorOverlay(TableInterpolator::interpolateLinearf(gaussian, ARRAYLEN(gaussian), intro_led_sweep_ms, 0.0f, 2250.0f), 0);
  }
  
  for (auto& button: buttons) {
    button.update();
  }

  if (buttons[0].is_pressed() && buttons[1].is_pressed()) {
    set_midi_learn(true);
  }

  for (auto& pot: pots) {
    pot.update(delay_ms);
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
    led.update(delay_ms);
  }

  delay(delay_ms);
}
