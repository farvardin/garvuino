#pragma once

#include "common.h"

#define USE_INPUT_A0
#define USE_INPUT_A1
#define USE_INPUT_A2
#define USE_INPUT_A3

template <uint8_t T>
class CVIn {
    static const uint8_t CV_IN_CONTROL_INTERVAL_BITS = 3;
    static const uint8_t CV_IN_CONTROL_INTERVAL      = 0x01 << CV_IN_CONTROL_INTERVAL_BITS;

public:
  INLINE static void initialize() {
  }

  INLINE static void clock(uint8_t count) {
    if ((count & (CV_IN_CONTROL_INTERVAL - 1)) == 1) {
      uint8_t idx = (count >> CV_IN_CONTROL_INTERVAL_BITS) & 0x07;

      uint16_t value;
      switch (idx) {
      case 0x0:
#if defined(USE_INPUT_A0)
        adcStart<0>();
#endif
        break;
      case 0x1:
#if defined(USE_INPUT_A0)
        value = adcRead();  // Read A0
#endif
#if defined(USE_INPUT_A1)
        adcStart<1>();
#endif
        break;
      case 0x2:
#if defined(USE_INPUT_A1)
        value = adcRead();  // Read A1
        IOsc<0>::set_osc_mix(value >> 3);
#endif
#if defined(USE_INPUT_A2)
        adcStart<2>();
#endif
        break;
      case 0x3:
#if defined(USE_INPUT_A2)
        value = adcRead();  // Read A2
        IFilter<0>::set_cutoff(value >> 3);
#endif
#if defined(USE_INPUT_A3)
        adcStart<3>();
#endif
        break;
      case 0x4:
#if defined(USE_INPUT_A3)
        value = adcRead();  // Read A3
        IFilter<0>::set_resonance(value >> 3);
#endif
        break;
      case 0x5:
        break;
      case 0x6:
        break;
      case 0x7:
        break;
      }
    }
  }

private:
  // Start the conversion.
  template <uint8_t N>
  INLINE static void adcStart() {
    ADMUX = _BV(REFS0) | N;  // analogReference(DEFAULT)
    ADCSRA = _BV(ADEN) | _BV(ADSC) | 0b111;
  }

  // CAUTION: Call after the conversion is finishd.
  INLINE static uint16_t adcRead() {
    uint8_t adcLow  = ADCL;
    uint8_t adcHigh = ADCH;
    return ((adcHigh << 8) | adcLow);
  }
};
