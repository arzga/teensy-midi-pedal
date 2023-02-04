#ifndef BUTTON_CPP
#define BUTTON_CPP

#include <functional>
#include "Bounce2.h"

class Button {
  private:
  
  Bounce m_bounce = Bounce();
  bool m_pressed = false;
  unsigned int m_press_time;

  public:

  std::function<void ()> on_down;
  std::function<void ()> on_click;

  Button(int pin) {
    m_bounce.attach(pin, INPUT_PULLUP); // Attach the debouncer to a pin with INPUT_PULLUP mode
    m_bounce.interval(25); // Use a debounce interval of 25 milliseconds
  }

  bool is_pressed() {
    return m_pressed;
  }

  unsigned int get_pressed_ms() {
    return millis() - m_press_time;
  }

  void cancel() {
    m_pressed = false;
  }

  void update() {
    m_bounce.update(); // Update the Bounce instance
    
    if ( m_bounce.fell() ) {  // Call code if button transitions from HIGH to LOW
      m_pressed = true;
      m_press_time = millis();
      if (on_down) {
        on_down();
      }
    }

    if ( m_bounce.rose() ) {
      if (m_pressed && on_click) {
        on_click();
      }

      m_pressed = false;
    }
  }
};

#endif
