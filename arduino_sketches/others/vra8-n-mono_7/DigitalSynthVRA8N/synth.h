#pragma once

// #define private public  // for tests

#include "common.h"

// associations of units
#define IOsc        Osc
#define IFilter     Filter
#define IAmp        Amp
#define IEnvGen     EnvGen
#if defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
#define ICVIn       CVIn
#endif
#define IVoice      Voice
#define ISynthCore  SynthCore

#include "osc.h"
#include "filter.h"
#include "amp.h"
#include "env-gen.h"
#if defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
#include "cv-in.h"
#endif
#include "voice.h"
#include "synth-core.h"

template <uint8_t T>
class Synth {
public:
  INLINE static void initialize() {
    ISynthCore<0>::initialize();
    ISynthCore<0>::program_change(PROGRAM_NUMBER_DEFAULT);
  }

  INLINE static void receive_midi_byte(uint8_t b) {
    ISynthCore<0>::receive_midi_byte(b);
  }

  INLINE static int8_t clock() {
    return ISynthCore<0>::clock();
  }
};
