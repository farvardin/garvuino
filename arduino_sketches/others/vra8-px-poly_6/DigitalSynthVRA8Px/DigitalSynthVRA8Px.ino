//#define DEBUG

#define SERIAL_SPEED (38400)    // for Serial MIDI
//#define SERIAL_SPEED (31250)    // for MIDI Shield, don't use this yet

#define PITCH_BEND_RANGE (2)
//#define PITCH_BEND_RANGE (12)

//#define TRANSPOSE (-24)



#include "common.h"
#include "synth.h"
#include "serial-in.h"
#include "audio-out.h"

void setup() {
  noInterrupts();
  Synth<0>::initialize();
  SerialIn<0>::open(SERIAL_SPEED);
  AudioOut<0>::open();
}

void loop() {
  while (true) {
    if (SerialIn<0>::available()) {
      uint8_t b = SerialIn<0>::read();
      Synth<0>::receive_midi_byte(b);
    }
    int8_t level = Synth<0>::clock();
    AudioOut<0>::write(level);
  }
}
