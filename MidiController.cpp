#ifndef MIDICONTROLLER_CPP
#define MIDICONTROLLER_CPP

#include <MIDI.h>

class MidiController {
  private:
  
  int m_midi_channel = 1;
  int m_cc;
  int m_state;
  bool m_continuous_cc;

  public:

  MidiController(int midi_cc, int initial_state, bool continuous_cc = true) {
    m_cc = midi_cc;
    m_state = initial_state;
    m_continuous_cc = continuous_cc;
  }

  int toggleState() {
    if (m_state > 64) {
      m_state = 0;
    } else {
      m_state = 127;
    }
    sendStateAsMIDI();
    return m_state;
  }

  void set(int value) {
    m_state = value;
    sendStateAsMIDI();
  }

  int getState() {
    return m_state;
  }

  void sendStateAsMIDI() {
    usbMIDI.sendControlChange(m_cc, m_state, m_midi_channel);
  }

  void update() {
    if (m_continuous_cc) {
      sendStateAsMIDI();
    }
  }
};

#endif
