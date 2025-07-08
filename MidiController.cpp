#ifndef MIDICONTROLLER_CPP
#define MIDICONTROLLER_CPP

#include <MIDI.h>
#include "MidiEvent.cpp"

class MidiController : public MidiEvent {
  private:
  
  int m_midi_channel = 1;
  int m_cc;
  int m_value;
  bool m_continuous_cc;

  public:

  MidiController(int midi_cc, int initial_value, bool continuous_cc = true) {
    m_cc = midi_cc;
    m_value = initial_value;
    m_continuous_cc = continuous_cc;
  }

  int toggleValue() {
    if (m_value > 64) {
      m_value = 0;
    } else {
      m_value = 127;
    }
    sendMIDI();
    return m_value;
  }

  void setValue(int value) {
    m_value = value;
    sendMIDI();
  }

  int getValue() {
    return m_value;
  }

  void sendMIDI() override {
    if (m_continuous_cc) {
      usbMIDI.sendControlChange(m_cc, m_value, m_midi_channel);
    } else {
      // Send trigger pulse
      usbMIDI.sendControlChange(m_cc, 127, m_midi_channel);
      usbMIDI.sendControlChange(m_cc, 0, m_midi_channel);
    }
  }

  void update() override {
    // Only send updates for continuous controllers, not triggers
    if (m_continuous_cc) {
      sendMIDI();
    }
  }
};

#endif
