#ifndef MIDIPROGRAMCHANGE_CPP
#define MIDIPROGRAMCHANGE_CPP

#include <MIDI.h>
#include "MidiEvent.cpp"

class MidiProgramChange : public MidiEvent {
  private:
    int m_midi_channel;   // 1-based
    int m_program;

  public:
    MidiProgramChange(int program, int midi_channel = 1) {
      m_program = program;
      m_midi_channel = midi_channel;
    }

    void sendMIDI() override {
      usbMIDI.sendProgramChange(m_program, m_midi_channel);
    }
};

#endif 