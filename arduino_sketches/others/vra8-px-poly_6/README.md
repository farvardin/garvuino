# Digital Synth VRA8-Px v2.0.1

- 2017-07-22 ISGK Instruments
- <https://github.com/risgk/digital-synth-vra8-p/tree/vra8-px-v2>

## Concept

- 3 Voice Paraphonic Synthesizer for Arduino Uno
- A Variant of Digital Synth VRA8-P <https://github.com/risgk/digital-synth-vra8-p>

## History

### v2.0 New Features

- ORGAN Waveform Deleted, RESONANCE Steps Changed (17 -> 9)
- CC# of AMP EG, VELOCITY SENS, and CUTOFF V SENS Changed
- Improved Sound, New PRESETs, PITCH BEND Support, TRANSPOSE Support, MIDI Shield Official Support
- DETUNE S&H AMT, DETUNE EG AMT, PORTAMENTO, UNI TO MONO, CUTOFF NG AMT, EG ATTACK, AMP EG/HOLD

### v1.2 New Features

- Cutoff Velocity Sensitivity, Detune EG Amount

### v1.0 New Features

- Phase Resolution: 24 bit, Internal Signal Processing: 16 bit
- Audio Output (8 bit) with Error Diffusion, Sub Oscillators (Triangle Wave)
- Changeable Osc 2 (Detune) Level, Resonance with Q = 11.3 (Max), Velocity Sensitivity

## Features

- 3 Voice Paraphonic Synthesizer (Pseudo Polyphonic Synthesizer), MIDI Sound Module
- Serial MIDI In (38400 bps), PWM Audio Out (Pin 6), PWM Rate: 62500 Hz
    - We recommend adding a RC filter circuit to reduce PWM ripples
    - A cutoff frequency 10.6 kHz (R: 150 ohm, C: 100 nF) works well
    - A cutoff frequency 15.9 kHz (R: 100 ohm, C: 100 nF) works well, too
    - **CAUTION**: Click sounds may occur when you connect the audio out to an amp/a speaker or reset the board
    - **CAUTION**: The Arduino PWM audio output is a unipolar LINE OUT
        - Please connect this to a power amp/a headphone amp (not to a speaker/a headphone directly)
- Sampling Rate: 15625 Hz, Bit Depth: 8 bit, LPF Attenuation Slope: -12 dB/oct
- Recommending [Hairless MIDI<->Serial Bridge](http://projectgus.github.io/hairless-midiserial/) to connect PC
- Files
    - `DigitalSynthVRA8Px.ino` is a sketch for Arduino/Genuino Uno
    - `make-sample-wav-file.cc` is for Debugging on PC
        - Requiring GCC (G++) or other
        - `make-sample-wav-file-cc.bat` makes a sample WAV file (working on Windows)
    - `generate-*.rb` generate source files
        - Requiring a Ruby execution environment
- **CAUTION**: We recommend Arduino IDE 1.8.3

## VRA8-Px CTRL

- Parameter Editor (MIDI Controller) for VRA8-Px, Web App
- We recommend Google Chrome, which implements Web MIDI API
- VRA8-Px CTRL includes PRESET programs
- Recommending [loopMIDI](http://www.tobias-erichsen.de/software/loopmidi.html) (virtual loopback MIDI cable) to connect VRA8-Px
- **CAUTION**: Low CUTOFF with high RESONANCE can damage the speakers

## Controllers

    +----------------+------+----------------+---------------+----------------+------------------------------+
    | Controller     | INIT | Value 0        | Value 64      | Value 127      | Notes                        |
    +----------------+------+----------------+---------------+----------------+------------------------------+
    | UNISON/MIX     | 0    | OFF            | ON            | ON             | 3 Voice UNISON               |
    |                |      | +70.7%/+70.7%  | +70.7%/-70.7% | +70.7%/+70.7%  | Main/DETUNE MIX Level        |
    | WAVEFORM/SUB   | 0    | SAWtooth       | SQuare        | SQuare         | WAVEFORM                     |
    |                |      | 100%           | 0%            | 100%           | SUB Osc Level                |
    | DETUNE         | 48   | 0.06 Hz        | 1.0 Hz        | 3.8 Hz         |                              |
    | DETUNE S&H AMT | 32   | Avg. 0         | Avg. +32      | Avg. +63.5     | S&H: 488.3 Hz (Red Noise)    |
    | DETUNE EG AMT  | 48   | -126 (EG 100%) | 0             | +125 (EG 100%) | Modulates DETUNE             |
    | PORTAMENTO     | 32   | Off            | 0.09 s/Oct    | 0.8 s/Oct      | PORTAMENTO Time              |
    | UNI TO MONO    | 0    | OFF            | ON            | ON             | Change UNISON => MONO        |
    +----------------+------+----------------+---------------+----------------+------------------------------+
    | CUTOFF         | 64   | 0.22 kHz (A3)  | 1.2 kHz       | 7.04 kHz (A8)  | Range: 4 to 124              |
    | RESONANCE      | 64   | Q = 0.7        | Q = 2.8       | Q = 11.3       | 9 steps                      |
    | CUTOFF EG AMT  | 96   | -126 (EG 100%) | 0             | +125 (EG 100%) | EnvGen Modulates CUTOFF      |
    | CUTOFF NG AMT  | 64   | Avg. -32       | 0             | Avg. +31       | NoiseGen (White) Mods CUTOFF |
    +----------------+------+----------------+---------------+----------------+------------------------------+
    | EG ATTACK      | 0    | 4 ms           | 60 ms         | 4.2 s          | EnvGen ATTACK Time           |
    | EG DECAY       | 64   | 34 ms          | 2.2 s         | Infinity       | EnvGen DECAY Time            |
    | AMP EG/HOLD    | 0    | AMP EG OFF     | AMP EG ON     | AMP EG ON      | AMP EG ON: 64-127            |
    |                |      | HOLD OFF       | HOLD ON       | HOLD OFF       | HOLD ON: 32-95               |
    +----------------+------+----------------+---------------+----------------+------------------------------+
    | VELOCITY SENS  | 0    | 0%             | 50.4%         | 100%           | Gain VELOCITY SENSitivity    |
    | CUTOFF V SENS  | 127  | -128 (V 127)   | 0 (V 127)     | +126 (V 127)   | CUTOFF Velocity SENSitivity  |
    +----------------+------+----------------+---------------+----------------+------------------------------+

## MIDI Implementation Chart

      [Paraphonic Synthesizer]                                        Date: 2017-07-22       
      Model: Digital Synth VRA8-Px    MIDI Implementation Chart       Version: 2.0.1         
    +-------------------------------+---------------+---------------+-----------------------+
    | Function...                   | Transmitted   | Recognized    | Remarks               |
    +-------------------------------+---------------+---------------+-----------------------+
    | Basic        Default          | x             | 1             |                       |
    | Channel      Changed          | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Mode         Default          | x             | Mode 3        |                       |
    |              Messages         | x             | x             |                       |
    |              Altered          | ************* |               |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Note                          | x             | 0-127         |                       |
    | Number       : True Voice     | ************* | 24-84         | (w/o TRANSPOSE)       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Velocity     Note ON          | x             | o (V=1-127)   |                       |
    |              Note OFF         | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | After        Key's            | x             | x             |                       |
    | Touch        Ch's             | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Pitch Bend                    | x             | o             | Range: 2 (or 12)      |
    +-------------------------------+---------------+---------------+-----------------------+
    | Control                    16 | x             | o             | UNISON/MIX            |
    | Change                     17 | x             | o             | WAVEFORM/SUB          |
    |                            18 | x             | o             | DETUNE                |
    |                            19 | x             | o             | CUTOFF NG AMT         |
    |                            20 | x             | o             | CUTOFF                |
    |                            21 | x             | o             | RESONANCE             |
    |                            22 | x             | o             | CUTOFF EG AMT         |
    |                            23 | x             | o             | EG DECAY              |
    |                            24 | x             | o             | EG ATTACK             |
    |                            25 | x             | o             | DETUNE S&H AMT        |
    |                            26 | x             | o             | DETUNE EG AMT         |
    |                            27 | x             | o             | AMP EG/HOLD           |
    |                            28 | x             | o             | VELOCITY SENS         |
    |                            29 | x             | o             | CUTOFF V SENS         |
    |                            30 | x             | o             | PORTAMENTO            |
    |                            31 | x             | o             | UNI TO MONO           |
    +-------------------------------+---------------+---------------+-----------------------+
    | Program                       | x             | x             |                       |
    | Change       : True #         | ************* |               |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | System Exclusive              | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | System       : Song Pos       | x             | x             |                       |
    | Common       : Song Sel       | x             | x             |                       |
    |              : Tune           | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | System       : Clock          | x             | x             |                       |
    | Real Time    : Commands       | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Aux          : Local ON/OFF   | x             | x             |                       |
    | Messages     : All Notes OFF  | x             | o (123-127)   |                       |
    |              : Active Sense   | x             | x             |                       |
    |              : Reset          | x             | x             |                       |
    +-------------------------------+---------------+---------------+-----------------------+
    | Notes                         |                                                       |
    |                               |                                                       |
    +-------------------------------+-------------------------------------------------------+
      Mode 1: Omni On,  Poly          Mode 2: Omni On,  Mono          o: Yes                 
      Mode 3: Omni Off, Poly          Mode 4: Omni Off, Mono          x: No                  
