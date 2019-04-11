// refs http://playground.arduino.cc/Code/PCMAudio

#pragma once

#include "common.h"

template <uint8_t T>
class AudioOut {
  static const int SPEAKER_PIN = 6;   // PD6 (OC0A)
  static const int LED_PIN     = 13;  // PB5

  static uint8_t m_count;
  static uint8_t m_maxTcnt;
  static uint8_t m_busy;
  static uint8_t m_busyCont;

public:
  INLINE static void open() {
    pinMode(SPEAKER_PIN, OUTPUT);
    pinMode(LED_PIN,     OUTPUT);

    // Timer/Counter0 (8-bit Fast PWM, Inverting, 62500 Hz)
    TCCR0A = 0xC3;
    TCCR0B = 0x01;
    OCR0A  = 0x80;

    // Timer/Counter1 (9-bit Fast PWM, 31250 Hz)
    TCCR1A = 0x02;
    TCCR1B = 0x09;

    m_count = 0;
    m_maxTcnt = 0;
    m_busy = 0;
    m_busyCont = 0;
  }

  INLINE static void write(int8_t level) {
#if defined(DEBUG)
    // Output Elapsed Time as Channel Pressure (of Channel 16)
    m_count++;
    if (m_count == 0x7F) {
      UDR0 = 0xDF;
    } else if (m_count == 0xFF) {
#if 0
      uint8_t tcnt = TCNT1 >> 2;
      if ((tcnt < 64) && (tcnt > m_maxTcnt)) {
        m_maxTcnt = tcnt;
      }
      tcnt = m_maxTcnt;
#elif 0
      uint8_t tcnt = m_busyCont;
      tcnt &= 0x7F;
#elif 0
      uint8_t tcnt = TCNT1 >> 2;
      if (tcnt >= 64) {
        tcnt = 99;   // Not Over
      }
#else
      uint8_t tcnt = TCNT1 >> 2;
#endif
      UDR0 = tcnt;
      m_count = 0;
    }
#endif
    if (TIFR1 & _BV(TOV1)) {
      // CPU BUSY
      PORTB = _BV(5);
#if defined(DEBUG)
      if (m_busy) {
        m_busyCont++;
      }
      m_busy = 1;
#endif
    } else {
      PORTB = 0x00;
#if defined(DEBUG)
      m_busy = 0;
#endif
      while ((TIFR1 & _BV(TOV1)) == 0);
    }
    TIFR1 = _BV(TOV1);
    OCR0A = 0x7F - level;
  }
};

template <uint8_t T> uint8_t AudioOut<T>::m_count;
template <uint8_t T> uint8_t AudioOut<T>::m_maxTcnt;
template <uint8_t T> uint8_t AudioOut<T>::m_busy;
template <uint8_t T> uint8_t AudioOut<T>::m_busyCont;
