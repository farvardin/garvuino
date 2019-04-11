#include "common.h"
#include "program_table.h"

template <uint8_t T>
class Voice {
  static uint8_t m_count;
  static uint8_t m_eg0_decay_sustain;
  static uint8_t m_eg1_decay_sustain;
  static uint8_t m_last_note_number;
  static uint8_t m_on_note[16];
  static uint8_t m_output_error;
  static uint8_t m_portamento;
  static boolean m_legato_portamento;
  static uint8_t m_key_assign;
  static uint8_t m_attack;
  static uint8_t m_decay;
  static uint8_t m_sustain;
  static uint8_t m_release;
  static uint8_t m_amp_env_gen;
  static uint8_t m_exp_by_vel;

public:
  INLINE static void initialize() {
    m_last_note_number = NOTE_NUMBER_INVALID;
    for (uint8_t i = 0; i < 16; ++i) {
      m_on_note[i] = 0x00;
    }
    m_output_error = 0;
    m_portamento = 0;
    m_legato_portamento = false;
    m_key_assign = KEY_ASSIGN_LOW;
    IOsc<0>::initialize();
    IFilter<0>::initialize();
    IAmp<0>::initialize();
    IEnvGen<0>::initialize();
    IEnvGen<1>::initialize();
    m_attack = 0;
    m_decay = 0;
    m_sustain = 127;
    m_release = 0;
    m_amp_env_gen = 127;
    update_env_gen();
    m_exp_by_vel = false;
#if defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
    ICVIn<0>::initialize();
#endif
  }

  INLINE static void note_on(uint8_t note_number, uint8_t velocity) {
    if (m_legato_portamento) {
      if (m_last_note_number != NOTE_NUMBER_INVALID) {
        IOsc<0>::set_portamento(m_portamento);
      } else {
        IOsc<0>::set_portamento(0);
        IFilter<0>::note_on();
        IOsc<0>::reset_lfo_phase_unless_async();
        IEnvGen<0>::note_on();
        IEnvGen<1>::note_on();
        if (m_exp_by_vel) {
          IFilter<0>::set_expression(velocity);
          IEnvGen<1>::set_expression(velocity);
        }
      }
    } else {
      IOsc<0>::set_portamento(m_portamento);
      if ((m_key_assign == KEY_ASSIGN_LAST) || (m_last_note_number == NOTE_NUMBER_INVALID)) {
        IFilter<0>::note_on();
        IOsc<0>::reset_lfo_phase_unless_async();
        IEnvGen<0>::note_on();
        IEnvGen<1>::note_on();
        if (m_exp_by_vel) {
          IFilter<0>::set_expression(velocity);
          IEnvGen<1>::set_expression(velocity);
        }
      }
    }

    set_on_note(note_number);
    if (m_key_assign == KEY_ASSIGN_LAST) {
      m_last_note_number = note_number;
      IOsc<0>::note_on<0>(m_last_note_number);
      IOsc<0>::note_on<1>(m_last_note_number);
    } else {
      if (m_key_assign == KEY_ASSIGN_DUO) {
        m_last_note_number = get_lowest_on_note();
        IOsc<0>::note_on<0>(m_last_note_number);
        IOsc<0>::note_on<1>(get_highest_on_note());
      } else {
        if (m_key_assign == KEY_ASSIGN_HIGH) {
          m_last_note_number = get_highest_on_note();
        } else {
          m_last_note_number = get_lowest_on_note();
        }

        IOsc<0>::note_on<0>(m_last_note_number);
        IOsc<0>::note_on<1>(m_last_note_number);
      }
    }
  }

  INLINE static void note_off(uint8_t note_number) {
    clear_on_note(note_number);
    if (m_key_assign == KEY_ASSIGN_LAST) {
      if (m_last_note_number == note_number) {
        all_note_off();
      }
    } else {
      uint8_t active_on_note = 0;
      if (m_key_assign == KEY_ASSIGN_HIGH) {
        active_on_note = get_highest_on_note();
      } else {
        active_on_note = get_lowest_on_note();
      }

      if (active_on_note == NOTE_NUMBER_INVALID) {
        all_note_off();
      } else {
        if (m_legato_portamento) {
          if (m_last_note_number != NOTE_NUMBER_INVALID) {
            IOsc<0>::set_portamento(m_portamento);
          } else {
            IOsc<0>::set_portamento(0);
            IOsc<0>::reset_lfo_phase_unless_async();
            IEnvGen<0>::note_on();
            IEnvGen<1>::note_on();
          }
        } else {
          IOsc<0>::set_portamento(m_portamento);
          if (m_last_note_number == NOTE_NUMBER_INVALID) {
            IOsc<0>::reset_lfo_phase_unless_async();
            IEnvGen<0>::note_on();
            IEnvGen<1>::note_on();
          }
        }

        if (m_key_assign == KEY_ASSIGN_DUO) {
          m_last_note_number = active_on_note;
          IOsc<0>::note_on<0>(m_last_note_number);
          IOsc<0>::note_on<1>(get_highest_on_note());
        } else {
          m_last_note_number = active_on_note;
          IOsc<0>::note_on<0>(m_last_note_number);
          IOsc<0>::note_on<1>(m_last_note_number);
        }
      }
    }
  }

  INLINE static void all_note_off() {
    for (uint8_t i = 0; i < 16; ++i) {
      m_on_note[i] = 0x00;
    }
    m_last_note_number = NOTE_NUMBER_INVALID;
    IOsc<0>::note_off<0>();
    IOsc<0>::note_off<1>();
    IEnvGen<0>::note_off();
    IEnvGen<1>::note_off();
  }

  INLINE static void control_change(uint8_t controller_number, uint8_t controller_value) {
    switch (controller_number) {
    case EXPRESSION:
      if (!m_exp_by_vel) {
        IFilter<0>::set_expression(controller_value);
        IEnvGen<1>::set_expression(controller_value);
      }
      break;
    case MODULATION:
      IOsc<0>::set_lfo_depth<1>(controller_value);
      break;

    case FILTER_CUTOFF:
      IFilter<0>::set_cutoff(controller_value);
      break;
    case FILTER_RESO:
      IFilter<0>::set_resonance(controller_value);
      break;
    case CUTOFF_EG_AMT:
      IFilter<0>::set_cutoff_env_amt(controller_value);
      break;
    case EG_DECAY:
      m_decay = controller_value;
      update_decay_release();
      break;

    case OSC2_COARSE:
      IOsc<0>::set_pitch_offset_1(controller_value);
      break;
    case OSC2_FINE:
      IOsc<0>::set_detune(controller_value);
      break;
    case PORTAMENTO:
      m_portamento = controller_value;
      break;
    case EG_ATTACK:
      m_attack = controller_value;
      if (m_amp_env_gen >= 64) {
        IEnvGen<0>::set_attack(m_attack);
        IEnvGen<1>::set_attack(m_attack);
      } else {
        IEnvGen<0>::set_attack(m_attack);
      }
      break;

    case OSC_WAVE :
      IOsc<0>::set_osc_waveforms(controller_value);
      break;
    case OSC2_MIX:
      IOsc<0>::set_osc_mix(controller_value);
      break;
    case SUB_OSC_LEVEL:
      IOsc<0>::set_sub_osc_level(controller_value);
      break;
    case EG_SUSTAIN:
      {
        m_sustain = controller_value;

        if (m_amp_env_gen >= 64) {
          IEnvGen<0>::set_sustain(m_sustain);
          IEnvGen<1>::set_sustain(m_sustain);
        } else {
          IEnvGen<0>::set_sustain(m_sustain);
        }
      }
      break;

    case LFO_WAVE:
      IOsc<0>::set_lfo_waveform(controller_value);
      break;

    case SUB_OSC_WAVE:
      IOsc<0>::set_sub_waveform(controller_value);
      break;
    case LEGATO:
      if (controller_value < 64) {
        m_legato_portamento = false;
      } else {
        m_legato_portamento = true;
      }
      break;
    case AMP_EG_ON:
      m_amp_env_gen = controller_value;
      update_env_gen();
      break;

    case LFO_RATE:
      IOsc<0>::set_lfo_rate(controller_value);
      break;
    case LFO_DEPTH:
      IOsc<0>::set_lfo_depth<0>(controller_value);
      break;
    case PITCH_LFO_AMT:
      IOsc<0>::set_pitch_lfo_amt<0>(controller_value);
      break;
    case CO_LFO_AMT:
      IFilter<0>::set_cutoff_lfo_amt(controller_value);
      break;

    case PB_RANGE:
      IOsc<0>::set_pitch_bend_minus_range(controller_value);
      IOsc<0>::set_pitch_bend_plus_range(controller_value);
      break;
    case KEY_ASSIGN:
      if        (controller_value < 48) {
        m_key_assign = KEY_ASSIGN_LOW;
      } else if (controller_value < 80) {
        m_key_assign = KEY_ASSIGN_DUO;
      } else if (controller_value < 112) {
        m_key_assign = KEY_ASSIGN_HIGH;
      } else {
        m_key_assign = KEY_ASSIGN_LAST;
      }
      break;

    case EG_RELEASE:
      m_release = controller_value;
      update_decay_release();
      break;
    case PITCH_LFO_TGT:
      if (controller_value < 64) {
        IOsc<0>::set_lfo_target_both(true);
      } else {
        IOsc<0>::set_lfo_target_both(false);
      }
      break;
    case PITCH_EG_AMT:
      IOsc<0>::set_pitch_eg_amt(controller_value);
      break;
    case PITCH_EG_TGT:
      if (controller_value < 64) {
        IOsc<0>::set_pitch_eg_target_both(true);
      } else {
        IOsc<0>::set_pitch_eg_target_both(false);
      }
      break;
    case LFO_RT_EG_AMT:
      IOsc<0>::set_lfo_rate_eg_amt(controller_value);
      break;

    case CO_EXP_AMT:
      IFilter<0>::set_cutoff_exp_amt(controller_value);
      break;
    case AMP_EXP_AMT:
      IEnvGen<1>::set_amp_exp_amt(controller_value);
      break;
    case EXP_BY_VEL:
      if (controller_value < 64) {
        m_exp_by_vel = false;
      } else {
        m_exp_by_vel = true;
      }
      break;

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

  static void program_change(uint8_t program_number) {
    if (PROGRAM_NUMBER_MAX < program_number) {
      return;
    }

    control_change(OSC_WAVE     , g_preset_table_OSC_WAVE     [program_number]);
    control_change(OSC2_MIX     , g_preset_table_OSC2_MIX     [program_number]);
    control_change(OSC2_COARSE  , g_preset_table_OSC2_COARSE  [program_number]);
    control_change(OSC2_FINE    , g_preset_table_OSC2_FINE    [program_number]);

    control_change(SUB_OSC_WAVE , g_preset_table_SUB_OSC_WAVE [program_number]);
    control_change(SUB_OSC_LEVEL, g_preset_table_SUB_OSC_LEVEL[program_number]);
    control_change(PITCH_EG_AMT , g_preset_table_PITCH_EG_AMT [program_number]);
    control_change(PITCH_EG_TGT , g_preset_table_PITCH_EG_TGT [program_number]);

    control_change(FILTER_CUTOFF, g_preset_table_FILTER_CUTOFF[program_number]);
    control_change(FILTER_RESO  , g_preset_table_FILTER_RESO  [program_number]);
    control_change(CUTOFF_EG_AMT, g_preset_table_CUTOFF_EG_AMT[program_number]);
    control_change(AMP_EG_ON    , g_preset_table_AMP_EG_ON    [program_number]);

    control_change(EG_ATTACK    , g_preset_table_EG_ATTACK    [program_number]);
    control_change(EG_DECAY     , g_preset_table_EG_DECAY     [program_number]);
    control_change(EG_SUSTAIN   , g_preset_table_EG_SUSTAIN   [program_number]);
    control_change(EG_RELEASE   , g_preset_table_EG_RELEASE   [program_number]);

    control_change(LFO_WAVE     , g_preset_table_LFO_WAVE     [program_number]);
    control_change(LFO_RATE     , g_preset_table_LFO_RATE     [program_number]);
    control_change(PITCH_LFO_AMT, g_preset_table_PITCH_LFO_AMT[program_number]);
    control_change(CO_LFO_AMT   , g_preset_table_CO_LFO_AMT   [program_number]);

    control_change(LFO_DEPTH    , g_preset_table_LFO_DEPTH    [program_number]);
    control_change(LFO_RT_EG_AMT, g_preset_table_LFO_RT_EG_AMT[program_number]);
    control_change(PITCH_LFO_TGT, g_preset_table_PITCH_LFO_TGT[program_number]);
    control_change(CC15         , g_preset_table_CC15         [program_number]);

    control_change(PB_RANGE     , g_preset_table_PB_RANGE     [program_number]);
    control_change(CC86         , g_preset_table_CC86         [program_number]);
    control_change(CO_EXP_AMT   , g_preset_table_CO_EXP_AMT   [program_number]);
    control_change(AMP_EXP_AMT  , g_preset_table_AMP_EXP_AMT  [program_number]);

    control_change(PORTAMENTO   , g_preset_table_PORTAMENTO   [program_number]);
    control_change(LEGATO       , g_preset_table_LEGATO       [program_number]);
    control_change(KEY_ASSIGN   , g_preset_table_KEY_ASSIGN   [program_number]);
    control_change(EXP_BY_VEL   , g_preset_table_EXP_BY_VEL   [program_number]);
  }

  INLINE static int8_t clock() {
    m_count++;

#if defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
    ICVIn<0>::clock(m_count);
#endif

    uint8_t env_gen_output_0 = IEnvGen<0>::clock(m_count);
    int16_t osc_output = IOsc<0>::clock(m_count, env_gen_output_0);
    int16_t lfo_output = IOsc<0>::get_lfo_level();
    int16_t filter_output = IFilter<0>::clock(m_count, osc_output, env_gen_output_0, lfo_output);
    uint8_t env_gen_output_1 = IEnvGen<1>::clock(m_count);
    int16_t amp_output = IAmp<0>::clock(filter_output, env_gen_output_1);

    // error diffusion
    int16_t output = amp_output + m_output_error;
    m_output_error = low_byte(output);
    return high_sbyte(output);
  }

private:
  INLINE static void set_on_note(uint8_t note_number) {
    m_on_note[note_number >> 3] |= (1 << (note_number & 0x07));
  }

  INLINE static void clear_on_note(uint8_t note_number) {
    m_on_note[note_number >> 3] &= ~(1 << (note_number & 0x07));
  }

  INLINE static uint8_t get_highest_on_note() {
    uint8_t highest_on_note = NOTE_NUMBER_INVALID;
    for (int8_t i = 15; i >= 0; --i) {
      if (m_on_note[i] != 0x00) {
        if        (m_on_note[i] & 0x80) {
          highest_on_note = (i << 3) + 7;
        } else if (m_on_note[i] & 0x40) {
          highest_on_note = (i << 3) + 6;
        } else if (m_on_note[i] & 0x20) {
          highest_on_note = (i << 3) + 5;
        } else if (m_on_note[i] & 0x10) {
          highest_on_note = (i << 3) + 4;
        } else if (m_on_note[i] & 0x08) {
          highest_on_note = (i << 3) + 3;
        } else if (m_on_note[i] & 0x04) {
          highest_on_note = (i << 3) + 2;
        } else if (m_on_note[i] & 0x02) {
          highest_on_note = (i << 3) + 1;
        } else if (m_on_note[i] & 0x01) {
          highest_on_note = (i << 3) + 0;
        }
        break;
      }
    }
    return highest_on_note;
  }

  INLINE static uint8_t get_lowest_on_note() {
    uint8_t lowest_on_note = NOTE_NUMBER_INVALID;
    for (uint8_t i = 0; i < 16; ++i) {
      if (m_on_note[i] != 0x00) {
        if        (m_on_note[i] & 0x01) {
          lowest_on_note = (i << 3) + 0;
        } else if (m_on_note[i] & 0x02) {
          lowest_on_note = (i << 3) + 1;
        } else if (m_on_note[i] & 0x04) {
          lowest_on_note = (i << 3) + 2;
        } else if (m_on_note[i] & 0x08) {
          lowest_on_note = (i << 3) + 3;
        } else if (m_on_note[i] & 0x10) {
          lowest_on_note = (i << 3) + 4;
        } else if (m_on_note[i] & 0x20) {
          lowest_on_note = (i << 3) + 5;
        } else if (m_on_note[i] & 0x40) {
          lowest_on_note = (i << 3) + 6;
        } else if (m_on_note[i] & 0x80) {
          lowest_on_note = (i << 3) + 7;
        }
        break;
      }
    }
    return lowest_on_note;
  }

  INLINE static void update_decay_release() {
    if (m_amp_env_gen >= 64) {
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(m_decay);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
        IEnvGen<1>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
        IEnvGen<1>::set_release(0);
      }
    } else {
      IEnvGen<0>::set_decay(m_decay);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
      }
    }
  }

  INLINE static void update_env_gen() {
    if (m_amp_env_gen >= 64) {
      IEnvGen<0>::set_attack(m_attack);
      IEnvGen<1>::set_attack(m_attack);
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(m_decay);
      IEnvGen<0>::set_sustain(m_sustain);
      IEnvGen<1>::set_sustain(m_sustain);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
        IEnvGen<1>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
        IEnvGen<1>::set_release(0);
      }
    } else {
      IEnvGen<0>::set_attack(m_attack);
      IEnvGen<1>::set_attack(0);
      IEnvGen<0>::set_decay(m_decay);
      IEnvGen<1>::set_decay(0);
      IEnvGen<0>::set_sustain(m_sustain);
      IEnvGen<1>::set_sustain(127);
      if (m_release >= 64) {
        IEnvGen<0>::set_release(m_decay);
      } else {
        IEnvGen<0>::set_release(0);
      }
      IEnvGen<1>::set_release(0);
    }
  }
};

template <uint8_t T> uint8_t Voice<T>::m_count;
template <uint8_t T> uint8_t Voice<T>::m_eg0_decay_sustain;
template <uint8_t T> uint8_t Voice<T>::m_eg1_decay_sustain;
template <uint8_t T> uint8_t Voice<T>::m_last_note_number;
template <uint8_t T> uint8_t Voice<T>::m_on_note[16];
template <uint8_t T> uint8_t Voice<T>::m_output_error;
template <uint8_t T> uint8_t Voice<T>::m_portamento;
template <uint8_t T> boolean Voice<T>::m_legato_portamento;
template <uint8_t T> uint8_t Voice<T>::m_key_assign;
template <uint8_t T> uint8_t Voice<T>::m_attack;
template <uint8_t T> uint8_t Voice<T>::m_decay;
template <uint8_t T> uint8_t Voice<T>::m_sustain;
template <uint8_t T> uint8_t Voice<T>::m_release;
template <uint8_t T> uint8_t Voice<T>::m_amp_env_gen;
template <uint8_t T> uint8_t Voice<T>::m_exp_by_vel;
