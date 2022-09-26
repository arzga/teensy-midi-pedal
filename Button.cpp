#include <MIDI.h>
#include "Bounce2.h"

class Button {
  private:
  
  Bounce m_bounce = Bounce();
  int m_midi_channel = 1;
  int m_cc;
  bool m_on;

  public:

  Button(int pin, int midi_cc, bool on) {
    m_bounce.attach(pin, INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
    m_bounce.interval(25); // Use a debounce interval of 25 milliseconds
    m_cc = midi_cc;
    m_on = on;
  }

  bool update() {
    m_bounce.update(); // Update the Bounce instance
    
    if ( m_bounce.fell() ) {  // Call code if button transitions from HIGH to LOW
      usbMIDI.sendControlChange(m_cc, m_on ? 127 : 0, m_midi_channel);
      return true;
    }
    return false;
  }
};
