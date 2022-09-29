#ifndef LEDFX_CPP
#define LEDFX_CPP

#include <Arduino.h>
#include "TableInterpolator.cpp"

class LedFX {
  private:

  int m_pin;
  float m_color = -1;
  float m_color_overlay;
  int m_overlay_millis = -1;
  
  public:
  
  LedFX(int pin) {
    m_pin = pin;
    pinMode(m_pin, OUTPUT);
    control_pin(-1);
  }

  void setColor(float color) {
    m_color = color;
    m_overlay_millis = 0;
    control_pin(color);
  }

  void setColorOverlay(float color, int overlay_millis) {
    m_color_overlay = color;
    m_overlay_millis = overlay_millis;
    control_pin(color);
  }

  void control_pin(float color) {
    if (color < 0) {
      analogWrite(m_pin, 0);
    } else {
      analogWrite(m_pin, (byte)(1+254*TableInterpolator::interpolateLinearf(ledResponse, ARRAYLEN(ledResponse), color, 0.0f, 1.0f)));
    }
  }

  void update(int delta_ms) {
    float effectiveColor = m_color;

    if (m_overlay_millis >= 0) {
      m_overlay_millis -= delta_ms;
      effectiveColor = m_color_overlay;
    }

    control_pin(effectiveColor);
  }
};

#endif
