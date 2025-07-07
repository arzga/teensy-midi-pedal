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

#define CONTROL_BUTTON_A 0
#define CONTROL_BUTTON_B 1
#define CONTROL_BUTTON_B_MOMENTARY_1S 2
#define CONTROL_BUTTON_B_MOMENTARY_2S 3
#define CONTROL_BUTTON_B_MOMENTARY_3S 4
#define CONTROL_POT_1 5
#define CONTROL_POT_2 6
#define CONTROL_POT_3 7
#define CONTROL_POT_4 8
#define NUM_CONTROLLERS 9


MidiController default_button_a = MidiController(67, 0);
MidiController default_button_b = MidiController(68, 0);
MidiController default_pot1 = MidiController(69, 64);
MidiController default_pot2 = MidiController(70, 64);
MidiController default_pot3 = MidiController(71, 64);
MidiController default_pot4 = MidiController(72, 64);

MidiController default_button_b_option1 = MidiController(104, 0, false); // Momentary
MidiController default_button_b_option2 = MidiController(105, 0, false); // Momentary
MidiController default_button_b_option3 = MidiController(106, 0, false); // Momentary

MidiController alt1_button_a = MidiController(86, 127);
MidiController alt1_button_b = MidiController(87, 0);

MidiController alt2_button_a = MidiController(73, 127);
MidiController alt2_button_b = MidiController(74, 127);
MidiController alt2_pot1 = MidiController(75, 64);
MidiController alt2_pot2 = MidiController(76, 64);
MidiController alt2_pot3 = MidiController(77, 64);
MidiController alt2_pot4 = MidiController(78, 64);

int num_controllers = NUM_CONTROLLERS;
int num_buttons = 2;
int num_pots = 4;
int num_leds = 2;
unsigned int num_options_a = 3;
unsigned int num_options_b = 3;

bool first_update = true;
bool midi_learn_mode = false;

int intro_led_sweep_ms = 2500;

long last_midi_send_ms = 0;

int page = 0;

std::vector<Button> buttons;
std::vector<Pot> pots;

std::vector<std::reference_wrapper<MidiController>> midi_controllers = {
  default_button_a,
  default_button_b,
  default_button_b_option1,
  default_button_b_option2,
  default_button_b_option3,
  default_pot1,
  default_pot2,
  default_pot3,
  default_pot4,
};

// Store references of Midi Controllers in pages using std::reference_wrapper
std::vector<std::vector<std::reference_wrapper<MidiController>>> pages = {
  {
    default_button_a,
    default_button_b,
    default_button_b_option1,
    default_button_b_option2,
    default_button_b_option3,
    default_pot1,
    default_pot2,
    default_pot3,
    default_pot4,
  },
  {
    alt1_button_a,
    alt1_button_b,
    default_button_b_option1,
    default_button_b_option2,
    default_button_b_option3,
    default_pot1,
    default_pot2,
    default_pot3,
    default_pot4,
  },
  {
    alt2_button_a,
    alt2_button_b,
    default_button_b_option1,
    default_button_b_option2,
    default_button_b_option3,
    alt2_pot1,
    alt2_pot2,
    alt2_pot3,
    alt2_pot4,
  },
};

std::vector<LedFX> leds;

void setup() {
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

  // Define unique controller
  /*
  midi_controllers.push_back(default_button_a);
  midi_controllers.push_back(default_button_b);
  midi_controllers.push_back(default_button_b_option1);
  midi_controllers.push_back(default_button_b_option2);
  midi_controllers.push_back(default_button_b_option3);
  midi_controllers.push_back(default_pot1);
  midi_controllers.push_back(default_pot2);
  midi_controllers.push_back(default_pot3);
  midi_controllers.push_back(default_pot4);

  // Page 1
  pages.push_back(default_button_a);
  pages.push_back(default_button_b);
  pages.push_back(default_button_b_option1);
  pages.push_back(default_button_b_option2);
  pages.push_back(default_button_b_option3);
  pages.push_back(default_pot1);
  pages.push_back(default_pot2);
  pages.push_back(default_pot3);
  pages.push_back(default_pot4);

  // Page 2
  pages.push_back(alt1_button_a);
  pages.push_back(alt1_button_b);
  pages.push_back(default_button_b_option1);
  pages.push_back(default_button_b_option2);
  pages.push_back(default_button_b_option3);
  pages.push_back(default_pot1);
  pages.push_back(default_pot2);
  pages.push_back(default_pot3);
  pages.push_back(default_pot4);

  // Page 3
  pages.push_back(alt2_button_a);
  pages.push_back(alt2_button_b);
  pages.push_back(alt2_button_b_option1);
  pages.push_back(alt2_button_b_option2);
  pages.push_back(alt2_button_b_option3);
  pages.push_back(alt2_pot1);
  pages.push_back(alt2_pot2);
  pages.push_back(alt2_pot3);
  pages.push_back(alt2_pot4);
*/
  buttons[0].on_click = [&]() {
    controller_moved();
    unsigned int option = (buttons[0].get_pressed_ms() + 250 ) / 1000;
    if (option > 0) {
      // Change controller page
      page = min(option - 1, num_options_a - 1);
      // Set leds to display state of page controllers
      leds[0].setColor(pages[page][CONTROL_BUTTON_A].get().getState() / 127.0);
      leds[1].setColor(pages[page][CONTROL_BUTTON_B].get().getState() / 127.0);
    } else {
      int state = pages[page][CONTROL_BUTTON_A].get().toggleState();
      leds[0].setColor(state / 127.0);
    }
  };

  buttons[1].on_click = [&]() {
    controller_moved();
    unsigned int option = (buttons[1].get_pressed_ms() + 250 ) / 1000;
    if (option > 0) {
      // Move option to range [0..num_options[
      option = min(option - 1, num_options_b - 1);
      // Send midi button pulse
      pages[page][CONTROL_BUTTON_B_MOMENTARY_1S + option].get().set(127);
      pages[page][CONTROL_BUTTON_B_MOMENTARY_1S + option].get().set(0);
    } else {
      int state = pages[page][CONTROL_BUTTON_B].get().toggleState();
      leds[1].setColor(state / 127.0);
    }
  };

  pots[0].on_move = [&](int midi_value) {
    controller_moved();
    pages[page][CONTROL_POT_1].get().set(midi_value);
    leds[0].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[1].on_move = [&](int midi_value) {
    controller_moved();
    pages[page][CONTROL_POT_2].get().set(midi_value);
    leds[0].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[2].on_move = [&](int midi_value) {
    controller_moved();
    pages[page][CONTROL_POT_3].get().set(midi_value);
    leds[1].setColorOverlay(midi_value / 127.0, 500);
  };

  pots[3].on_move = [&](int midi_value) {
    controller_moved();
    pages[page][CONTROL_POT_4].get().set(midi_value);
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

  // Handle button hold feedback
  if (buttons[0].is_pressed() && buttons[1].is_pressed()) {
    buttons[0].cancel();
    buttons[1].cancel();
    set_midi_learn(true);
  } else if (buttons[0].is_pressed()) {
    // Only blink up to number of pages
    unsigned int pressed_ms = buttons[0].get_pressed_ms();
    if (pressed_ms > (num_options_a + 1) * 1000) {
      leds[0].setColorOverlay(0.0f, 0);
      leds[1].setColorOverlay(0.0f, 0);
    } else if (pressed_ms > 500) {
      leds[0].setColorOverlay(TableInterpolator::interpolateLinearf(fadeOut, ARRAYLEN(fadeOut), pressed_ms % 1000, 0, 500.0f), 0);
      leds[1].setColorOverlay(TableInterpolator::interpolateLinearf(fadeOut, ARRAYLEN(fadeOut), pressed_ms % 1000, 0, 500.0f), 0);
    }
  } else if (buttons[1].is_pressed()) {
    // Only blink up to number of momentary options
    unsigned int pressed_ms = buttons[1].get_pressed_ms();
    if (pressed_ms > (num_options_b + 1) * 1000) {
      leds[0].setColorOverlay(0.0f, 0);
      leds[1].setColorOverlay(0.0f, 0);
    } else if (pressed_ms > 500) {
      leds[0].setColorOverlay(TableInterpolator::interpolateLinearf(fadeOut, ARRAYLEN(fadeOut), pressed_ms % 1000, 0, 500.0f), 0);
      leds[1].setColorOverlay(TableInterpolator::interpolateLinearf(fadeOut, ARRAYLEN(fadeOut), pressed_ms % 1000, 0, 500.0f), 0);
    }
  } else {
    if (midi_learn_mode) {
      // Alternate leds
      leds[0].setColorOverlay((millis() % 1500) > 750 ? 1.0f : 0.0f, 0);
      leds[1].setColorOverlay(((millis() + 750) % 1500) > 750 ? 1.0f : 0.0f, 0);
    }
  }

  for (auto& pot: pots) {
    pot.update(delay_ms);
  }

  // Send MIDI state on regular intervals
  if (!midi_learn_mode && millis() - last_midi_send_ms > MIDI_SEND_INTERVAL_MS) {
    last_midi_send_ms = millis();
    for (auto& midi_controller: midi_controllers) {
      midi_controller.get().update();
    }
  }

  for (auto& led: leds) {
    led.update(delay_ms);
  }

  first_update = false;
  delay(delay_ms);
}
