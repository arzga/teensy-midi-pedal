#ifndef POT_CPP
#define POT_CPP

#include <functional>
#include <Arduino.h>

class Pot {
  private:

  int m_pin;
  int m_midi_value = -1;
  float m_smooth_value = -1.0;
  int m_responsive_timer = 0;

  public:

  std::function<void (int midi_value)> on_move;

  Pot(int pin) {
    m_pin = pin;
  }

  void update(int delta_ms) {
    if (m_responsive_timer > 0) {
      m_responsive_timer -= delta_ms;
    }

    float current_value = analogRead(m_pin) / 1023.0;
    
    if (m_smooth_value < 0) {
      m_smooth_value = current_value;
    } else {
      float adjust_ratio = m_responsive_timer > 0 ? 0.1f : 0.03f;
      m_smooth_value = ((1.0f-adjust_ratio) * m_smooth_value) + (adjust_ratio * current_value);
    }
    
    int midi_value = round(m_smooth_value * 127);

    if (abs(midi_value - m_midi_value) > 1) {
      m_responsive_timer = 200;
    }
    
    if ( m_responsive_timer > 0 && midi_value != m_midi_value ) {
      m_midi_value = midi_value;
      if (on_move) {
        on_move(midi_value);
        /*
        //Serial.print(midi_value);
        //Serial.print(" ");
        Serial.print(current_value);
        Serial.print(" ");
        Serial.print(m_smooth_value);
        Serial.println();
        */
      }
    }
  }
};

#endif
