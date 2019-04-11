// refs http://playground.arduino.cc/Code/PCMAudio

#pragma once

#include "common.h"

template <uint8_t T>
class AudioOut {
  static const int SPEAKER_PIN = 6;   // PD6 (OC0A)
  static const int LED_PIN     = 13;  // PB5

  static uint8_t m_count;

public:
  INLINE static void open() {
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(LED_PIN,     OUTPUT);

    // Timer/Counter0 (8-bit Fast PWM, Inverting, 62500 Hz)
    TCCR0A = 0xC3;
    TCCR0B = 0x01;
    OCR0A  = 0x80;

    // Timer/Counter1 (10-bit Fast PWM, 15625 Hz)
    TCCR1A = 0x03;
    TCCR1B = 0x09;

    m_count = 0;
  }

  INLINE static void write(int8_t level) {
#if defined(DEBUG)
    // Output Elapsed Time as Channel Pressure
    m_count++;
    if (m_count == 0x7F) {
      UDR0 = 0xDF;
    } else if (m_count == 0xFF) {
#if 0
      static uint8_t s_maxCnt = 0;
      uint8_t cnt = TCNT1 >> 3;
      if ((cnt < 64) && (cnt > s_maxCnt)) {
        s_maxCnt = cnt;
      }
      UDR0 = s_maxCnt;
      m_count = 0;
#else
      UDR0 = TCNT1 >> 3;
      m_count = 0;
#endif
    }
#endif
    if (TIFR1 & _BV(TOV1)) {
      // CPU BUSY
      PORTB |= _BV(5);
    } else {
      PORTB &= ~_BV(5);
    }
    while ((TIFR1 & _BV(TOV1)) == 0);
    TIFR1 = _BV(TOV1);
    OCR0A = 0x80 - level;
  }
};

template <uint8_t T> uint8_t AudioOut<T>::m_count;
