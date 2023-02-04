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
unsigned int num_pages = 2;

bool first_update = true;
bool midi_learn_mode = false;

int intro_led_sweep_ms = 2500;

long last_midi_send_ms = 0;

int page = 0;

std::vector<Button> buttons;
std::vector<Pot> pots;
std::vector<MidiController> midi_controllers;
std::vector<LedFX> leds;

void setup() {
  midi_controllers.reserve(num_controllers * num_pages);
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

  // Page 1
  midi_controllers.emplace_back(67, 0); // Btn 0
  midi_controllers.emplace_back(68, 0);
  midi_controllers.emplace_back(69, 64); // Pot 0
  midi_controllers.emplace_back(70, 64);
  midi_controllers.emplace_back(71, 64);
  midi_controllers.emplace_back(72, 64);

  // Page 2
  midi_controllers.emplace_back(80, 127); // Btn 0
  midi_controllers.emplace_back(81, 127);
  midi_controllers.emplace_back(82, 64); // Pot 0
  midi_controllers.emplace_back(83, 64);
  midi_controllers.emplace_back(84, 64);
  midi_controllers.emplace_back(85, 64);

  buttons[0].on_click = [&]() {
    controller_moved();
    unsigned int variant = (buttons[0].get_pressed_ms() + 250 ) / 1000;
    if (variant > 0) {
      // Change controller page
      page = min(variant - 1, num_pages - 1);
      // Set leds to display state of page controllers
      leds[0].setColor(midi_controllers[0 + (page * num_controllers)].getState() / 127.0);
      leds[1].setColor(midi_controllers[1 + (page * num_controllers)].getState() / 127.0);
    } else {
      int state = midi_controllers[0 + (page * num_controllers)].toggleState();
      leds[0].setColor(state / 127.0);
    }
  };

  buttons[1].on_click = [&]() {
    controller_moved();
    int state = midi_controllers[1 + page * num_controllers].toggleState();
    leds[1].setColor(state / 127.0);
  };

  pots[0].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[2 + page * num_controllers].set(midi_value);
    leds[0].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[1].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[3 + page * num_controllers].set(midi_value);
    leds[0].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[2].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[4 + page * num_controllers].set(midi_value);
    leds[1].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[3].on_move = [&](int midi_value) {
    controller_moved();
    midi_controllers[5 + page * num_controllers].set(midi_value);
    leds[1].setColorOverlay(midi_value / 127.0, 500);
  };
}

void controller_moved() {
  set_midi_learn(false);
  if (!first_update) {
    // Cancel intro
    intro_led_sweep_ms = 0;
  }
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

  if (buttons[0].is_pressed()) {
    // Only blink up to number of pages
    unsigned int pressed_ms = buttons[0].get_pressed_ms();
    if (pressed_ms > 500) {
      leds[0].setColorOverlay(TableInterpolator::interpolateLinearf(fadeIn, ARRAYLEN(fadeIn), pressed_ms, 500.0f, 1000.0f) * 0.5f, 0);
      leds[1].setColorOverlay(TableInterpolator::interpolateLinearf(fadeIn, ARRAYLEN(fadeIn), pressed_ms, 1500.0f, 2000.0f) * 0.5f, 0);
    }
  }

  if (buttons[1].is_pressed()) {
    leds[1].setColorOverlay(0.0f, 0);
  }

  if (buttons[0].is_pressed() && buttons[1].is_pressed()) {
    buttons[0].cancel();
    buttons[1].cancel();
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

  first_update = false;
  delay(delay_ms);
}
