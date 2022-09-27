#ifndef BUTTON_CPP
#define BUTTON_CPP

#include <functional>
#include "Bounce2.h"

class Button {
  private:
  
  Bounce m_bounce = Bounce();
  bool m_pressed = false;

  public:

  std::function<void ()> on_press;

  Button(int pin) {
    m_bounce.attach(pin, INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
    m_bounce.interval(25); // Use a debounce interval of 25 milliseconds
  }

  bool is_pressed() {
    return m_pressed;
  }

  void update() {
    m_bounce.update(); // Update the Bounce instance
    
    if ( m_bounce.fell() ) {  // Call code if button transitions from HIGH to LOW
      m_pressed = true;
      if (on_press) {
        on_press();
      }
    }

    if ( m_bounce.rose() ) {
      m_pressed = false;
    }
  }
};

#endif
