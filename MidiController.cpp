#include <MIDI.h>

class MidiController {
  private:
  
  int m_midi_channel = 1;
  int m_cc;
  int m_led_pin;
  bool m_state;

  public:

  MidiController(int midi_cc, bool initial_state, int led_pin) {
    m_cc = midi_cc;
    m_state = initial_state;

    if (led_pin >= 0) {
      m_led_pin = led_pin;
      pinMode(m_led_pin, OUTPUT); // Setup the LED
      updateLed();
    }
  }

  void toggleState() {
    m_state = !m_state;
    updateLed();
    sendStateAsMIDI();
  }

  void updateLed() {
    if (m_led_pin >= 0) {
      digitalWrite(m_led_pin, m_state ? HIGH : LOW);
    }
  }

  void sendStateAsMIDI() {
    usbMIDI.sendControlChange(m_cc, m_state ? 127 : 0, m_midi_channel);
  }
};
