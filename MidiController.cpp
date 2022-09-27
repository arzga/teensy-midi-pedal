#ifndef MIDICONTROLLER_CPP
#define MIDICONTROLLER_CPP

#include <MIDI.h>

class MidiController {
  private:
  
  int m_midi_channel = 1;
  int m_cc;
  int m_state;

  public:

  MidiController(int midi_cc, int initial_state) {
    m_cc = midi_cc;
    m_state = initial_state;
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

  bool getState() {
    return m_state;
  }

  void sendStateAsMIDI() {
    usbMIDI.sendControlChange(m_cc, m_state, m_midi_channel);
  }
};

#endif
