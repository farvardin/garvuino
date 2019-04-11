#include "common.h"

template <uint8_t T>
class Voice {
  static uint8_t m_count;
  static boolean m_unison_on;
  static uint8_t m_waveform;
  static uint8_t m_amp_env_amt_current;
  static uint8_t m_amp_env_amt_target;
  static boolean m_forced_hold;
  static boolean m_damper_pedal;
  static uint8_t m_note_number[3];
  static boolean m_note_hold[3];
  static uint8_t m_velocity[1];
  static uint8_t m_output_error;
  static uint8_t m_velocity_sensitivity;
  static int8_t  m_cutoff_velocity_sensitivity;

public:
  INLINE static void initialize() {
    m_count = 3;
    m_unison_on = false;
    m_waveform = OSC_WAVEFORM_SAW;
    m_amp_env_amt_current = 0;
    m_amp_env_amt_target = 0;
    m_forced_hold = false;
    m_damper_pedal = false;
    m_note_number[0] = NOTE_NUMBER_INVALID;
    m_note_number[1] = NOTE_NUMBER_INVALID;
    m_note_number[2] = NOTE_NUMBER_INVALID;
    m_note_hold[0] = false;
    m_note_hold[1] = false;
    m_note_hold[2] = false;
    m_velocity[0] = 127;
    m_output_error = 0;
    m_velocity_sensitivity = 0;
    m_cutoff_velocity_sensitivity = 0;
    IOsc<0>::initialize();
    IFilter<0>::initialize();
    IAmp<0>::initialize();
    IGate<0>::initialize();
    IEnvGen<0>::initialize();
  }

  INLINE static void set_unison(uint8_t controller_value) {
    if (controller_value >= 64) {
      IOsc<0>::set_mix((127 - controller_value) << 1);
      if (!m_unison_on) {
        m_unison_on = true;
        IOsc<0>::set_unison(m_unison_on);
        if (m_note_number[0] != NOTE_NUMBER_INVALID) {
          m_note_number[1] = NOTE_NUMBER_INVALID;
          m_note_number[2] = NOTE_NUMBER_INVALID;
          IOsc<0>::note_on(0, m_note_number[0]);
          IGate<0>::note_on(1, m_velocity[0]);
          IGate<0>::note_on(2, m_velocity[0]);
        } else {
          all_note_off();
        }
      }
    } else {
      IOsc<0>::set_mix(controller_value << 1);
      if (m_unison_on) {
        m_unison_on = false;
        IOsc<0>::set_unison(m_unison_on);
        if (m_note_number[0] != NOTE_NUMBER_INVALID) {
          IOsc<0>::note_on(0, m_note_number[0]);
          IGate<0>::note_off(1);
          IGate<0>::note_off(2);
        } else {
          all_note_off();
        }
      }
    }
  }

  INLINE static void set_waveform(uint8_t controller_value) {
    uint8_t waveform;
    if (controller_value < 64) {
      waveform = OSC_WAVEFORM_SAW;
    } else {
      waveform = OSC_WAVEFORM_SQ;
    }

    if (m_waveform != waveform) {
      m_waveform = waveform;
      IOsc<0>::set_waveform(m_waveform);
      if (m_note_number[0] != NOTE_NUMBER_INVALID) {
        IOsc<0>::note_on(0, m_note_number[0]);
      }
      if (m_note_number[1] != NOTE_NUMBER_INVALID) {
        IOsc<0>::note_on(1, m_note_number[1]);
      }
      if (m_note_number[2] != NOTE_NUMBER_INVALID) {
        IOsc<0>::note_on(2, m_note_number[2]);
      }
    }

    if (controller_value < 32) {
      IOsc<0>::set_sub((31 - controller_value) << 2);
    } else if (controller_value >= 96) {
      IOsc<0>::set_sub((controller_value - 96) << 2);
    } else {
      IOsc<0>::set_sub(0);
    }
  }

  INLINE static void note_on(uint8_t note_number, uint8_t velocity) {
#if defined(TRANSPOSE)
    if ((note_number < NOTE_NUMBER_MIN - TRANSPOSE) ||
        (note_number > NOTE_NUMBER_MAX - TRANSPOSE)) {
      return;
    }
#else
    if ((note_number < NOTE_NUMBER_MIN) ||
        (note_number > NOTE_NUMBER_MAX)) {
      return;
    }
#endif

    uint8_t v = high_byte((velocity + 1) * (m_velocity_sensitivity << 1)) +
                          (127 - m_velocity_sensitivity);
    uint8_t cutoff_v = high_sbyte((static_cast<int8_t>(velocity - 64) << 1) *
                                  m_cutoff_velocity_sensitivity) + 64;

    if (m_unison_on) {
      m_note_number[0] = note_number;
      m_note_number[1] = NOTE_NUMBER_INVALID;
      m_note_number[2] = NOTE_NUMBER_INVALID;
      m_note_hold[0] = false;
      m_note_hold[1] = false;
      m_note_hold[2] = false;
      IOsc<0>::note_on(0, note_number);
      IGate<0>::note_on(0, v);
      IGate<0>::note_on(1, v);
      IGate<0>::note_on(2, v);
      m_velocity[0] = v;
    } else {
      if (m_note_number[0] == note_number) {
        m_note_hold[0] = false;
        IGate<0>::note_on(0, v);
        m_velocity[0] = v;
      } else if (m_note_number[1] == note_number) {
        m_note_hold[1] = false;
        IGate<0>::note_on(1, v);
      } else if (m_note_number[2] == note_number) {
        m_note_hold[2] = false;
        IGate<0>::note_on(2, v);
      } else if ((m_note_number[0] == NOTE_NUMBER_INVALID) || m_note_hold[0]) {
        m_note_number[0] = note_number;
        m_note_hold[0] = false;
        IOsc<0>::note_on(0, note_number);
        IGate<0>::note_on(0, v);
        m_velocity[0] = v;
      } else if ((m_note_number[1] == NOTE_NUMBER_INVALID) || m_note_hold[1]) {
        m_note_number[1] = note_number;
        m_note_hold[1] = false;
        IOsc<0>::note_on(1, note_number);
        IGate<0>::note_on(1, v);
      } else {
        m_note_number[2] = note_number;
        m_note_hold[2] = false;
        IOsc<0>::note_on(2, note_number);
        IGate<0>::note_on(2, v);
      }
    }

    IGate<0>::note_on(3, 127);
    IEnvGen<0>::note_on();
    IFilter<0>::note_on(cutoff_v);
  }

  INLINE static void note_off(uint8_t note_number) {
    if (m_unison_on) {
      if (m_note_number[0] == note_number) {
        if (m_forced_hold || m_damper_pedal) {
          m_note_hold[0] = true;
          m_note_hold[1] = true;
          m_note_hold[2] = true;
        } else {
          all_note_off();
        }
      }
    } else {
      if (m_note_number[0] == note_number) {
        if (m_forced_hold || m_damper_pedal) {
          m_note_hold[0] = true;
        } else {
          m_note_number[0] = NOTE_NUMBER_INVALID;
          IGate<0>::note_off(0);
        }
      } else if (m_note_number[1] == note_number) {
        if (m_forced_hold || m_damper_pedal) {
          m_note_hold[1] = true;
        } else {
          m_note_number[1] = NOTE_NUMBER_INVALID;
          IGate<0>::note_off(1);
        }
      } else if (m_note_number[2] == note_number) {
        if (m_forced_hold || m_damper_pedal) {
          m_note_hold[2] = true;
        } else {
          m_note_number[2] = NOTE_NUMBER_INVALID;
          IGate<0>::note_off(2);
        }
      }
    }

    if ((m_note_number[0] == NOTE_NUMBER_INVALID) &&
        (m_note_number[1] == NOTE_NUMBER_INVALID) &&
        (m_note_number[2] == NOTE_NUMBER_INVALID)) {
      IGate<0>::note_off(3);
      IEnvGen<0>::note_off();
    }
  }

  INLINE static void all_note_off() {
    m_note_number[0] = NOTE_NUMBER_INVALID;
    m_note_hold[0] = false;
    IGate<0>::note_off(0);
    m_note_number[1] = NOTE_NUMBER_INVALID;
    m_note_hold[1] = false;
    IGate<0>::note_off(1);
    m_note_number[2] = NOTE_NUMBER_INVALID;
    m_note_hold[2] = false;
    IGate<0>::note_off(2);
    IGate<0>::note_off(3);
    IEnvGen<0>::note_off();
  }

  INLINE static void control_change(uint8_t controller_number, uint8_t controller_value) {
    switch (controller_number) {
    case FILTER_CUTOFF:
      IFilter<0>::set_cutoff(controller_value);
      break;
    case FILTER_RESO:
      IFilter<0>::set_resonance(controller_value);
      break;
    case FILTER_EG_AMT:
      IFilter<0>::set_env_amt(controller_value);
      break;
    case FILTER_NG_AMT:
      uint8_t noise_gen_amt;
      if (controller_value < 32) {
        noise_gen_amt = controller_value << 1;
      } else if (controller_value < 96) {
        noise_gen_amt = 64;
      } else {
        noise_gen_amt = ((controller_value - 96) << 2) + 64;
      }
      IFilter<0>::set_noise_gen_amt(noise_gen_amt);
      break;
    case UNISON:
      set_unison(controller_value);
      break;
    case OSC_WAVEFORM:
      set_waveform(controller_value);
      break;
    case OSC_DETUNE:
      IOsc<0>::set_detune(controller_value);
      break;
    case DETUNE_NG_AMT:
      IOsc<0>::set_detune_noise_gen_amt(controller_value);
      break;
    case DETUNE_EG_AMT:
      IOsc<0>::set_detune_env_amt(controller_value);
      break;
    case UNISON_OPTION:
      IOsc<0>::set_unison_option(controller_value);
      break;
    case PORTAMENTO:
      IOsc<0>::set_portamento(controller_value);
      break;
    case EG_DECAY:
      IEnvGen<0>::set_decay(controller_value);
      break;
    case EG_ATTACK:
      IEnvGen<0>::set_attack(controller_value);
      break;
    case AMP_EG:
      if (controller_value < 64) {
        m_amp_env_amt_target = 0;
      } else {
        m_amp_env_amt_target = AMP_ENV_AMT_MAX;
      }
      if ((32 <= controller_value) && (controller_value < 96)) {
        set_forced_hold(true);
      } else {
        set_forced_hold(false);
      }
      break;
    case VELOCITY_SENS:
      m_velocity_sensitivity = controller_value;
      break;
    case CUTOFF_V_SENS:
      m_cutoff_velocity_sensitivity = (controller_value - 64) << 1;
      break;
#if 0
    case DAMPER_PEDAL:
      if (controller_value < 64) {
        set_damper_pedal(false);
      } else {
        set_damper_pedal(true);
      }
      break;
#endif
    case ALL_NOTES_OFF:
    case OMNI_MODE_OFF:
    case OMNI_MODE_ON:
    case MONO_MODE_ON:
    case POLY_MODE_ON:
      all_note_off();
      break;
    }
  }

  INLINE static void pitch_bend(uint8_t lsb, uint8_t msb) {
    uint16_t pitch_bend = (msb << 7) + lsb - 8192;
    IOsc<0>::set_pitch_bend(pitch_bend);
  }

  INLINE static int8_t clock() {
    m_count++;
    if ((m_count & (0x10 - 1)) == 0) {
      update_amp_env_amt();
    }

    uint8_t gate_output_array[4];
    IGate<0>::clock();
    gate_output_array[0] = IGate<0>::level<0>();
    gate_output_array[1] = IGate<0>::level<1>();
    gate_output_array[2] = IGate<0>::level<2>();
    gate_output_array[3] = IGate<0>::level<3>();
    uint8_t env_gen_output = IEnvGen<0>::clock();
    int16_t osc_output = IOsc<0>::clock(gate_output_array[0],
                                        gate_output_array[1],
                                        gate_output_array[2],
                                        env_gen_output);
    int16_t filter_output = IFilter<0>::clock(osc_output, env_gen_output);
    uint8_t gain_control = high_byte((env_gen_output * m_amp_env_amt_current) +
                                     ((gate_output_array[3] << 2) *
                                      (AMP_ENV_AMT_MAX - m_amp_env_amt_current)));
    int16_t amp_output = IAmp<0>::clock(filter_output, gain_control);

    // error diffusion
    int16_t output = amp_output + m_output_error;
    m_output_error = low_byte(output);
    return high_sbyte(output);
  }

private:
  INLINE static void update_amp_env_amt() {
    if (m_amp_env_amt_current < m_amp_env_amt_target) {
      m_amp_env_amt_current += AMP_ENV_AMT_STEP;
    } else if (m_amp_env_amt_current > m_amp_env_amt_target) {
      m_amp_env_amt_current -= AMP_ENV_AMT_STEP;
    }
  }

  INLINE static void set_forced_hold(boolean on) {
    if (on) {
      m_forced_hold = true;
    } else {
      if (m_forced_hold) {
        m_forced_hold = false;
        if (!m_damper_pedal) {
          turn_hold_off();
        }
      }
    }
  }

  INLINE static void set_damper_pedal(uint8_t on) {
    if (on) {
      m_damper_pedal = true;
    } else {
      if (m_damper_pedal) {
        m_damper_pedal = false;
        if (!m_forced_hold) {
          turn_hold_off();
        }
      }
    }
  }

  INLINE static void turn_hold_off() {
    if (m_note_hold[0]) {
      m_note_number[0] = NOTE_NUMBER_INVALID;
      m_note_hold[0] = false;
      IGate<0>::note_off(0);
    }
    if (m_note_hold[1]) {
      m_note_number[1] = NOTE_NUMBER_INVALID;
      m_note_hold[1] = false;
      IGate<0>::note_off(1);
    }
    if (m_note_hold[2]) {
      m_note_number[2] = NOTE_NUMBER_INVALID;
      m_note_hold[2] = false;
      IGate<0>::note_off(2);
    }

    if ((m_note_number[0] == NOTE_NUMBER_INVALID) &&
        (m_note_number[1] == NOTE_NUMBER_INVALID) &&
        (m_note_number[2] == NOTE_NUMBER_INVALID)) {
      IGate<0>::note_off(3);
      IEnvGen<0>::note_off();
    }
  }
};

template <uint8_t T> uint8_t Voice<T>::m_count;
template <uint8_t T> boolean Voice<T>::m_unison_on;
template <uint8_t T> uint8_t Voice<T>::m_waveform;
template <uint8_t T> uint8_t Voice<T>::m_amp_env_amt_current;
template <uint8_t T> uint8_t Voice<T>::m_amp_env_amt_target;
template <uint8_t T> boolean Voice<T>::m_forced_hold;
template <uint8_t T> boolean Voice<T>::m_damper_pedal;
template <uint8_t T> uint8_t Voice<T>::m_note_number[3];
template <uint8_t T> boolean Voice<T>::m_note_hold[3];
template <uint8_t T> uint8_t Voice<T>::m_velocity[1];
template <uint8_t T> uint8_t Voice<T>::m_output_error;
template <uint8_t T> uint8_t Voice<T>::m_velocity_sensitivity;
template <uint8_t T> int8_t  Voice<T>::m_cutoff_velocity_sensitivity;
