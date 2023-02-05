#ifndef LEDFX_CPP
#define LEDFX_CPP

#include <Arduino.h>
#include "TableInterpolator.cpp"

class LedFX {
  private:

  int m_pin;
  float m_color = -1;
  float m_color_overlay;
  int m_transition_ms = 0;
  int m_overlay_millis = -1;
  float m_effectiveColor = 0;
  
  public:
  
  LedFX(int pin) {
    m_pin = pin;
    pinMode(m_pin, OUTPUT);
    control_pin(-1);
  }

  void setColor(float color) {
    m_color = color;
    m_overlay_millis = 0;
    // control_pin(color);
  }

  void setColorOverlay(float color, int overlay_millis, int transition_ms = 0) {
    m_color_overlay = color;
    m_overlay_millis = overlay_millis;
    m_transition_ms = transition_ms;
    // control_pin(color);
  }

  void control_pin(float color) {
    if (color < 0) {
      analogWrite(m_pin, 0);
    } else {
      analogWrite(m_pin, (byte)(1+254*TableInterpolator::interpolateLinearf(ledResponse, ARRAYLEN(ledResponse), color, 0.0f, 1.0f)));
    }
  }

  void update(int delta_ms) {
    if (m_overlay_millis >= 0) {
      m_overlay_millis -= delta_ms;
      if (m_transition_ms > 0) {
        if (m_effectiveColor < m_color_overlay) {
          m_effectiveColor = min(m_effectiveColor + ((float)delta_ms / m_transition_ms), m_color_overlay);
        } else {
          m_effectiveColor = max(m_effectiveColor - ((float)delta_ms / m_transition_ms), m_color_overlay);
        }
      } else {
        m_effectiveColor = m_color_overlay;
      }
    } else {
      m_effectiveColor = m_color;
    }
    control_pin(m_effectiveColor);

  }
};

#endif
