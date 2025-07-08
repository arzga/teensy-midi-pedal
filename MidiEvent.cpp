#ifndef MIDIEVENT_CPP
#define MIDIEVENT_CPP

class MidiEvent {
  public:
    virtual void sendMIDI() {};               // Send current state as MIDI

    // The following are valid for MidiController only
    virtual void update() {};                 // Called periodically to send current state
    virtual void setValue(int value) {};      // Valid for MidiController only
    virtual int getValue() { return 0; }      // Valid for MidiController only
    virtual int toggleValue() { return 0; }   // Valid for MidiController only
};

#endif 