#pragma once

#include "common.h"
#include "osc-table.h"
#include "mul-q.h"
#include <math.h>

static const uint8_t OSC_MIX_TABLE_LENGTH = 31;

template <uint8_t T>
class Osc {
  static const uint8_t FLUCTUATION_INIT = 32;

  static int8_t         m_mix_0_target;
  static int8_t         m_mix_0_current;
  static int8_t         m_mix_1_target;
  static int8_t         m_mix_1_current;
  static int8_t         m_mix_sub_target;
  static int8_t         m_mix_sub_current;
  static int16_t        m_level_sub;
  static int8_t         m_mix_table[OSC_MIX_TABLE_LENGTH];
  static int8_t         m_pitch_offset_1;
  static int8_t         m_detune;
  static uint8_t        m_fluctuation;
  static uint8_t        m_portamento_coef;
  static int16_t        m_lfo_mod_level[2];
  static uint16_t       m_lfo_phase;
  static int8_t         m_lfo_wave_level;
  static int16_t        m_lfo_level;
  static uint16_t       m_lfo_rate_actual;
  static uint8_t        m_lfo_rate;
  static int8_t         m_lfo_rate_eg_amt;
  static uint8_t        m_lfo_depth[2];
  static boolean        m_pitch_lfo_target_both;
  static int8_t         m_pitch_lfo_amt;
  static uint8_t        m_waveform[2];
  static uint8_t        m_sub_waveform;
  static uint8_t        m_lfo_waveform;
  static uint8_t        m_lfo_sampled;
  static int16_t        m_pitch_bend;
  static uint8_t        m_pitch_bend_minus_range;
  static uint8_t        m_pitch_bend_plus_range;
  static int16_t        m_pitch_bend_normalized;
  static uint16_t       m_pitch_target[2];
  static uint16_t       m_pitch_current[2];
  static uint16_t       m_pitch_real[2];
  static const uint8_t* m_wave_table[3];
  static const uint8_t* m_wave_table_temp[2];
  static __uint24       m_freq[2];
  static __uint24       m_freq_temp[2];
  static __uint24       m_phase[2];
  static uint8_t        m_rnd_cnt;
  static uint16_t       m_rnd_temp;
  static uint8_t        m_rnd;
  static uint8_t        m_rnd_prev;
  static uint8_t        m_red_noise;
  static boolean        m_note_on[2];
  static boolean        m_pitch_eg_target_both;
  static int16_t        m_pitch_eg_amt;
  static __uint24       m_lfsr;

public:
  INLINE static void initialize() {
    for (uint8_t i = 0; i < OSC_MIX_TABLE_LENGTH; i++) {
      m_mix_table[i] = static_cast<uint8_t>(sqrtf(static_cast<float>(i) /
                                                  (OSC_MIX_TABLE_LENGTH - 1)) * 80);
    }
    m_mix_0_current = 0;
    m_mix_1_current = 0;
    m_mix_sub_current = 0;
    set_osc_mix(0);
    set_sub_osc_level(0);
    m_level_sub = 0;
    m_pitch_offset_1 = 0;
    m_detune = 0;
    m_fluctuation = FLUCTUATION_INIT;
    m_portamento_coef = 0;
    m_lfo_mod_level[0] = 0;
    m_lfo_mod_level[1] = 0;
    m_lfo_phase = 0;
    m_lfo_wave_level = 0;
    m_lfo_level = 0;
    m_lfo_rate_actual = 0;
    m_lfo_rate = 0;
    m_lfo_rate_eg_amt = 0;
    m_lfo_depth[0] = 0;
    m_lfo_depth[1] = 0;
    m_pitch_lfo_target_both = true;
    m_waveform[0] = OSC_WAVEFORM_SAW;
    m_waveform[1] = OSC_WAVEFORM_SAW;
    m_sub_waveform = SUB_WAVEFORM_SIN;
    m_lfo_waveform = LFO_WAVEFORM_TRI_ASYNC;
    m_lfo_sampled = 64;
    m_pitch_bend_normalized = 0;
    m_pitch_target[0] = NOTE_NUMBER_MIN << 8;
    m_pitch_target[1] = NOTE_NUMBER_MIN << 8;
    m_pitch_current[0] = m_pitch_target[0];
    m_pitch_current[1] = m_pitch_target[1];
    m_pitch_real[0] = m_pitch_current[0];
    m_pitch_real[1] = m_pitch_current[1];
    m_wave_table[0] = g_osc_saw_wave_tables[0];
    m_wave_table[1] = g_osc_saw_wave_tables[0];
    m_wave_table[2] = g_osc_sin_wave_table_h1;
    m_wave_table_temp[0] = g_osc_saw_wave_tables[0];
    m_wave_table_temp[1] = g_osc_saw_wave_tables[0];
    m_freq[0] = g_osc_freq_table[0];
    m_freq[1] = g_osc_freq_table[0];
    m_freq_temp[0] = g_osc_freq_table[0];
    m_freq_temp[1] = g_osc_freq_table[0];
    m_phase[0] = 0;
    m_phase[1] = 0;
    m_rnd_cnt = 0;
    m_rnd_temp = 1;
    m_rnd = 0;
    m_rnd_prev = 0;
    m_red_noise = 0;
    m_note_on[0] = false;
    m_note_on[1] = false;
    m_pitch_eg_target_both = true;
    m_pitch_eg_amt = 0;
    m_lfsr = 0x0001u;
    set_pitch_bend_minus_range(2);
    set_pitch_bend_plus_range(2);
  }

  INLINE static void set_osc_mix(uint8_t controller_value) {
    if (controller_value >= 2) {
      controller_value -= 2;
    }

    if (controller_value > 123) {
      controller_value = 123;
    }

    m_mix_0_target = m_mix_table[(OSC_MIX_TABLE_LENGTH - 1) - (controller_value >> 2)];
    m_mix_1_target = m_mix_table[                             (controller_value >> 2)];
  }

  INLINE static void set_osc_waveforms(uint8_t controller_value) {
    if (controller_value < 16) {
      set_osc_waveform<0>(OSC_WAVEFORM_SAW);
      set_osc_waveform<1>(OSC_WAVEFORM_SAW);
    } else if (controller_value < 64) {
      set_osc_waveform<0>(OSC_WAVEFORM_SAW);
      set_osc_waveform<1>(OSC_WAVEFORM_SQ);
    } else if (controller_value < 112) {
      set_osc_waveform<0>(OSC_WAVEFORM_SQ);
      set_osc_waveform<1>(OSC_WAVEFORM_SAW);
    } else {
      set_osc_waveform<0>(OSC_WAVEFORM_SQ);
      set_osc_waveform<1>(OSC_WAVEFORM_SQ);
    }
  }

  template <uint8_t N>
  INLINE static void set_osc_waveform(uint8_t controller_value) {
    if (controller_value < 64) {
      m_waveform[N] = OSC_WAVEFORM_SAW;
    } else {
      m_waveform[N] = OSC_WAVEFORM_SQ;
    }
  }

  INLINE static void set_sub_waveform(uint8_t controller_value) {
    if (controller_value < 32) {
      m_sub_waveform = SUB_WAVEFORM_SIN;
    } else if (controller_value < 96) {
      m_sub_waveform = SUB_WAVEFORM_NOISE;
    } else {
      m_sub_waveform = SUB_WAVEFORM_SQ;
    }
  }

  INLINE static void set_lfo_waveform(uint8_t controller_value) {
    if        (controller_value < 16) {
      m_lfo_waveform = LFO_WAVEFORM_TRI_ASYNC;
    } else if (controller_value < 48) {
      m_lfo_waveform = LFO_WAVEFORM_TRI_SYNC;
    } else if (controller_value < 80) {
      m_lfo_waveform = LFO_WAVEFORM_SAW_DOWN;
    } else if (controller_value < 112) {
      m_lfo_waveform = LFO_WAVEFORM_RANDOM;
    } else {
      m_lfo_waveform = LFO_WAVEFORM_SQ;
    }
  }

  INLINE static void set_sub_osc_level(uint8_t controller_value) {
    uint8_t idx = controller_value >> 2;
    if (idx > 0) {
      idx -= 1;
    }
    m_mix_sub_target = m_mix_table[idx];
  }

  INLINE static void set_pitch_offset_1(uint8_t controller_value) {
    if (controller_value < 4) {
      m_pitch_offset_1 = -60;
    } else if (controller_value <= 124) {
      m_pitch_offset_1 = controller_value - 64;
    } else {
      m_pitch_offset_1 = 60;
    }
  }

  INLINE static void set_detune(uint8_t controller_value) {
    m_detune = (controller_value - 64) << 1;
  }

  INLINE static void set_portamento(uint8_t controller_value) {
    if (controller_value < 2) {
      m_portamento_coef = 0;
    } else {
      m_portamento_coef = (controller_value >> 1) + 191;
    }
  }

  INLINE static void set_lfo_rate(uint8_t controller_value) {
    m_lfo_rate = controller_value;
  }

  INLINE static void set_lfo_rate_eg_amt(uint8_t controller_value) {
    m_lfo_rate_eg_amt = (controller_value - 64) << 1;
  }

  template <uint8_t N>
  INLINE static void set_lfo_depth(uint8_t controller_value) {
    m_lfo_depth[N] = controller_value;
  }

  INLINE static void set_lfo_target_both(boolean lfo_target_both) {
    m_pitch_lfo_target_both = lfo_target_both;
  }

  template <uint8_t N>
  INLINE static void set_pitch_lfo_amt(uint8_t controller_value) {
    if (controller_value < 4) {
      m_pitch_lfo_amt = -60;
    } else if (controller_value < 124) {
      m_pitch_lfo_amt = controller_value - 64;
    } else {
      m_pitch_lfo_amt = 60;
    }
  }

  INLINE static void set_pitch_eg_amt(uint8_t controller_value) {
    if (controller_value < 2) {
      m_pitch_eg_amt = -30 << 8 << 1;
    } else if (controller_value < 32) {
      m_pitch_eg_amt = (controller_value - 32) << 8 << 1;
    } else if (controller_value < 97) {
      m_pitch_eg_amt = (controller_value - 64) << 3 << 1;
    } else if (controller_value < 127) {
      m_pitch_eg_amt = (controller_value - 96) << 8 << 1;
    } else {
      m_pitch_eg_amt = 30 << 8 << 1;
    }
  }

  INLINE static void set_pitch_eg_target_both(boolean pitch_eg_target_both) {
    m_pitch_eg_target_both = pitch_eg_target_both;
  }

  template <uint8_t N>
  INLINE static void note_on(uint8_t note_number) {
    m_pitch_target[N] = note_number << 8;
    m_note_on[N] = true;
  }

  template <uint8_t N>
  INLINE static void note_off() {
    m_note_on[N] = false;
  }

  INLINE static void reset_lfo_phase_unless_async() {
    if (m_lfo_waveform != LFO_WAVEFORM_TRI_ASYNC) {
      m_lfo_phase = 0xFFFF;
    }
  }

  INLINE static void set_pitch_bend_minus_range(uint8_t controller_value) {
    uint8_t range = controller_value;
    if (range > 30) {
      range = 30;
    }
    m_pitch_bend_minus_range = range;
    update_pitch_bend();
  }

  INLINE static void set_pitch_bend_plus_range(uint8_t controller_value) {
    uint8_t range = controller_value;
    if (range > 30) {
      range = 30;
    }
    m_pitch_bend_plus_range = range;
    update_pitch_bend();
  }

  INLINE static void set_pitch_bend(int16_t pitch_bend) {
    m_pitch_bend = pitch_bend;
    update_pitch_bend();
  }

  INLINE static uint8_t get_white_noise_7() {
    return m_rnd;
  }

  INLINE static uint8_t get_red_noise_8() {
    return m_red_noise;
  }

  INLINE static int16_t get_lfo_level() {
    return m_lfo_level;
  }

  INLINE static int16_t clock(uint8_t count, uint8_t eg_level) {
    if ((count & 0x01) == 1) {
#if !defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
      int8_t wave_0_sub = 0;
      if (m_sub_waveform == SUB_WAVEFORM_NOISE) {
        uint8_t lsb = m_lfsr & 0x000001u;
        m_lfsr >>= 1;
        m_lfsr ^= (-lsb) & 0xE10000u;
        if (lsb) {
          wave_0_sub = +(OSC_WAVE_TABLE_AMPLITUDE >> 2);
        } else {
          wave_0_sub = -(OSC_WAVE_TABLE_AMPLITUDE >> 2);
        }
      } else {
        wave_0_sub = get_wave_level(m_wave_table[2], m_phase[0] >> 8);
      }

      int8_t mix_sub = m_mix_sub_current;
      if (m_sub_waveform == SUB_WAVEFORM_SQ) {
        mix_sub = mix_sub >> 1;
      }
      m_level_sub = wave_0_sub * mix_sub;
#endif
    }
    else if ((count & (OSC_CONTROL_INTERVAL - 1)) == 0) {
      uint8_t idx = (count >> OSC_CONTROL_INTERVAL_BITS) & 0x0F;
      switch (idx) {
      case 0x0:
        update_freq_0th<0>();
        break;
      case 0x1:
        update_freq_1st<0>(eg_level);
        break;
      case 0x2:
        update_freq_2nd<0>();
        break;
      case 0x3:
        update_freq_3rd<0>();
        break;
      case 0x4:
        update_freq_4th<0>();
        break;
      case 0x5:
        update_rnd();
        m_rnd_cnt++;
        if ((m_rnd_cnt & 0x07) == 0x00) {
          m_red_noise = m_rnd_prev + m_rnd;
        }
        break;
      case 0x6:
        update_sub_waveform();
        update_mix();
        break;
      case 0x7:
        update_lfo_0th(eg_level);
        break;
      case 0x8:
        update_freq_0th<1>();
        break;
      case 0x9:
        update_freq_1st<1>(eg_level);
        break;
      case 0xA:
        update_freq_2nd<1>();
        break;
      case 0xB:
        update_freq_3rd<1>();
        break;
      case 0xC:
        update_freq_4th<1>();
        break;
      case 0xD:
        update_rnd();
        if ((m_rnd_cnt & 0x07) == 0x00) {
          m_red_noise = m_rnd_prev + m_rnd;
        }
        break;
      case 0xE:
        update_lfo_1st();
        break;
      case 0xF:
        update_lfo_2nd();
        break;
      }
    }

    m_phase[0] += m_freq[0];
    m_phase[1] += m_freq[1];

    int8_t wave_0_main   = get_wave_level(m_wave_table[0], static_cast<uint16_t>(m_phase[0] >> 8) << 1);
    int8_t wave_0_detune = get_wave_level(m_wave_table[1], static_cast<uint16_t>(m_phase[1] >> 8) << 1);

    // amp and mix
    int16_t level_main   = wave_0_main   * m_mix_0_current;
    int16_t level_detune = wave_0_detune * m_mix_1_current;
#if !defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
    int16_t result       = level_main + level_detune + m_level_sub;
#else
    int16_t result       = level_main + level_detune;
#endif

    return result;
  }

private:
  INLINE static const uint8_t* get_wave_table(uint8_t waveform, uint8_t note_number) {
    const uint8_t* result;
    if (waveform == OSC_WAVEFORM_SAW) {
      result = g_osc_saw_wave_tables[note_number - NOTE_NUMBER_MIN];
    } else {     // OSC_WAVEFORM_SQ
      result = g_osc_sq_wave_tables[note_number - NOTE_NUMBER_MIN];
    }
    return result;
  }

  INLINE static int8_t get_wave_level(const uint8_t* wave_table, uint16_t phase) {
    uint8_t curr_index = high_byte(phase);
    uint8_t next_weight = low_byte(phase);
    uint16_t two_data = pgm_read_word(wave_table + curr_index);
    uint8_t curr_data = low_byte(two_data);
    uint8_t next_data = high_byte(two_data);

    // lerp
    int8_t result;
    if (static_cast<int8_t>(curr_data) < static_cast<int8_t>(next_data)) {
      result = curr_data + high_byte(static_cast<uint8_t>(next_data - curr_data) * next_weight);
    } else {
      result = curr_data - high_byte(static_cast<uint8_t>(curr_data - next_data) * next_weight);
    }

    return result;
  }

  INLINE static int8_t get_lfo_wave_level(uint16_t phase) {
    int8_t level = 0;

    switch (m_lfo_waveform) {
    case LFO_WAVEFORM_TRI_ASYNC:
    case LFO_WAVEFORM_TRI_SYNC:
      level = high_sbyte(phase);
      if (level < -64) {
        level = -64 - (level + 64);
      } else if (level < 64) {
        // do nothing
      } else {
        level = 64 - (level - 64);
      }
      level = -level;
      break;
    case LFO_WAVEFORM_SAW_DOWN:
      {
        uint8_t b = high_byte(phase);
        b >>= 1;
        level = b - 64;
      }
      break;
    case LFO_WAVEFORM_RANDOM:
      if (phase < m_lfo_rate_actual) {
        m_lfo_sampled = get_white_noise_7();
      }
      level = m_lfo_sampled - 64;
      break;
    case LFO_WAVEFORM_SQ:
      level = high_sbyte(phase);
      if (level >= 0) {
        level = 0;
      } else {
        level = -128;
      }
      break;
    }

    return level;
  }

  template <uint8_t N>
  INLINE static void update_freq_0th() {
    if (m_note_on[N]) {
      m_pitch_current[N] = m_pitch_target[N] - mul_q15_q8(m_pitch_target[N] - m_pitch_current[N], m_portamento_coef);
    }

    int16_t t = TRANSPOSE << 8;
    m_pitch_real[N] = (64 << 8) + m_pitch_current[N] + m_pitch_bend_normalized + t;

    uint8_t coarse = high_byte(m_pitch_real[N]);
    if (coarse <= (NOTE_NUMBER_MIN + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MIN << 8;
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MAX << 8;
    } else {
      m_pitch_real[N] -= (64 << 8);
    }
  }

  template <uint8_t N>
  INLINE static void update_freq_1st(uint8_t eg_level) {
    m_pitch_real[N] += (64 << 8) + high_sbyte((m_fluctuation >> 2) * static_cast<int8_t>(get_red_noise_8() - 127));

    int16_t pitch_eg_mod_level = 0;
    if ((N == 1) || m_pitch_eg_target_both) {
      pitch_eg_mod_level = mul_q15_q8(m_pitch_eg_amt, eg_level);
    }
    m_pitch_real[N] += pitch_eg_mod_level;
  }

  template <uint8_t N>
  INLINE static void update_freq_2nd() {
    m_pitch_real[N] += m_lfo_mod_level[N];

    if (N == 1) {
      /* For OSC 2 */
      m_pitch_real[N] += (m_pitch_offset_1 << 8) + m_detune + m_detune;
    }

    uint8_t coarse = high_byte(m_pitch_real[N]);
    if (coarse <= (NOTE_NUMBER_MIN + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MIN << 8;
    } else if (coarse >= (NOTE_NUMBER_MAX + 64)) {
      m_pitch_real[N] = NOTE_NUMBER_MAX << 8;
    } else {
      m_pitch_real[N] -= (64 << 8);
    }

    m_pitch_real[N] += 128;  // For g_osc_tune_table[]
  }

  template <uint8_t N>
  INLINE static void update_freq_3rd() {
    uint8_t coarse = high_byte(m_pitch_real[N]);
    m_freq_temp[N] = g_osc_freq_table[coarse - NOTE_NUMBER_MIN];
    m_wave_table_temp[N] = get_wave_table(m_waveform[N], coarse);
  }

  template <uint8_t N>
  INLINE static void update_freq_4th() {
    uint8_t fine = low_byte(m_pitch_real[N]);
    uint16_t freq_div_512 = m_freq_temp[N] >> 8;
    freq_div_512 >>= 1;
    int16_t freq_offset = (freq_div_512 * g_osc_tune_table[fine >> (8 - OSC_TUNE_TABLE_STEPS_BITS)]) + (N << 1);
    m_freq[N] = m_freq_temp[N] + freq_offset;
    m_wave_table[N] = m_wave_table_temp[N];
  }

  INLINE static void update_sub_waveform() {
#if !defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
    if (m_sub_waveform == SUB_WAVEFORM_SQ) {
      uint8_t coarse = high_byte(m_pitch_real[0]);
      m_wave_table[2] = get_wave_table(OSC_WAVEFORM_SQ, coarse);
    } else {
      m_wave_table[2] = g_osc_sin_wave_table_h1;
    }
#endif
  }

  INLINE static void update_rnd() {
    m_rnd_temp = m_rnd_temp ^ (m_rnd_temp << 5);
    m_rnd_temp = m_rnd_temp ^ (m_rnd_temp >> 9);
    m_rnd_temp = m_rnd_temp ^ (m_rnd_temp << 8);
    m_rnd_prev = m_rnd;
    m_rnd = low_byte(m_rnd_temp) >> 1;
  }

  INLINE static void update_lfo_0th(uint8_t eg_level) {
    int8_t lfo_rate_mod = high_sbyte(m_lfo_rate_eg_amt * eg_level);
    int16_t lfo_rate = m_lfo_rate + lfo_rate_mod + lfo_rate_mod;
    if (lfo_rate > 127) {
      lfo_rate = 127;
    } else if (lfo_rate < 0) {
      lfo_rate = 0;
    }

    if (lfo_rate >= 32) {
      m_lfo_rate_actual = (high_byte((lfo_rate << 1) *
                                     (lfo_rate << 1)) + 2) * 12;
    } else {
      m_lfo_rate_actual = ((lfo_rate >> 1) + 2) * 12;
    }
  }

  INLINE static void update_lfo_1st() {
    m_lfo_phase += m_lfo_rate_actual;
    m_lfo_wave_level = get_lfo_wave_level(m_lfo_phase);
    uint8_t lfo_depth = m_lfo_depth[0] + m_lfo_depth[1];
    if (lfo_depth > 127) {
      lfo_depth = 127;
    }

    if (lfo_depth == 127) {
      lfo_depth = 128;
    }

    m_lfo_level = (lfo_depth * m_lfo_wave_level) << 1;
  }

  INLINE static void update_lfo_2nd() {
    m_lfo_mod_level[1] = -mul_q15_q7(m_lfo_level, m_pitch_lfo_amt);
    if (m_pitch_lfo_target_both) {
      m_lfo_mod_level[0] = m_lfo_mod_level[1];
    } else {
      m_lfo_mod_level[0] = 0;
    }
  }

  INLINE static void update_pitch_bend() {
    int16_t b = m_pitch_bend + 1;
    b >>= 3;
    if (b < 0) {
      m_pitch_bend_normalized = (b * m_pitch_bend_minus_range) >> 2;
    } else {
      m_pitch_bend_normalized = (b * m_pitch_bend_plus_range) >> 2;
    }
  }

  INLINE static void update_mix() {
#if !defined(EXPERIMENTAL_ENABLE_VOLTAGE_CONTROL)
    if (m_mix_sub_current < m_mix_sub_target) {
      m_mix_sub_current++;
    } else if (m_mix_sub_current > m_mix_sub_target) {
      m_mix_sub_current--;
    }
#endif

    if (m_mix_0_current < m_mix_0_target) {
      m_mix_0_current++;
    } else if (m_mix_0_current > m_mix_0_target) {
      m_mix_0_current--;
    }

    if (m_mix_1_current < m_mix_1_target) {
      m_mix_1_current++;
    } else if (m_mix_1_current > m_mix_1_target) {
      m_mix_1_current--;
    }
  }
};

template <uint8_t T> int8_t          Osc<T>::m_mix_0_target;
template <uint8_t T> int8_t          Osc<T>::m_mix_0_current;
template <uint8_t T> int8_t          Osc<T>::m_mix_1_target;
template <uint8_t T> int8_t          Osc<T>::m_mix_1_current;
template <uint8_t T> int8_t          Osc<T>::m_mix_sub_target;
template <uint8_t T> int8_t          Osc<T>::m_mix_sub_current;
template <uint8_t T> int16_t         Osc<T>::m_level_sub;
template <uint8_t T> int8_t          Osc<T>::m_mix_table[OSC_MIX_TABLE_LENGTH];
template <uint8_t T> int8_t          Osc<T>::m_pitch_offset_1;
template <uint8_t T> int8_t          Osc<T>::m_detune;
template <uint8_t T> uint8_t         Osc<T>::m_fluctuation;
template <uint8_t T> uint8_t         Osc<T>::m_portamento_coef;
template <uint8_t T> int16_t         Osc<T>::m_lfo_mod_level[2];
template <uint8_t T> uint16_t        Osc<T>::m_lfo_phase;
template <uint8_t T> int8_t          Osc<T>::m_lfo_wave_level;
template <uint8_t T> int16_t         Osc<T>::m_lfo_level;
template <uint8_t T> uint16_t        Osc<T>::m_lfo_rate_actual;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_rate;
template <uint8_t T> int8_t          Osc<T>::m_lfo_rate_eg_amt;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_depth[2];
template <uint8_t T> boolean         Osc<T>::m_pitch_lfo_target_both;
template <uint8_t T> int8_t          Osc<T>::m_pitch_lfo_amt;
template <uint8_t T> uint8_t         Osc<T>::m_waveform[2];
template <uint8_t T> uint8_t         Osc<T>::m_sub_waveform;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_waveform;
template <uint8_t T> uint8_t         Osc<T>::m_lfo_sampled;
template <uint8_t T> int16_t         Osc<T>::m_pitch_bend;
template <uint8_t T> uint8_t         Osc<T>::m_pitch_bend_minus_range;
template <uint8_t T> uint8_t         Osc<T>::m_pitch_bend_plus_range;
template <uint8_t T> int16_t         Osc<T>::m_pitch_bend_normalized;
template <uint8_t T> uint16_t        Osc<T>::m_pitch_target[2];
template <uint8_t T> uint16_t        Osc<T>::m_pitch_current[2];
template <uint8_t T> uint16_t        Osc<T>::m_pitch_real[2];
template <uint8_t T> const uint8_t*  Osc<T>::m_wave_table[3];
template <uint8_t T> const uint8_t*  Osc<T>::m_wave_table_temp[2];
template <uint8_t T> __uint24        Osc<T>::m_freq[2];
template <uint8_t T> __uint24        Osc<T>::m_freq_temp[2];
template <uint8_t T> __uint24        Osc<T>::m_phase[2];
template <uint8_t T> uint8_t         Osc<T>::m_rnd_cnt;
template <uint8_t T> uint16_t        Osc<T>::m_rnd_temp;
template <uint8_t T> uint8_t         Osc<T>::m_rnd;
template <uint8_t T> uint8_t         Osc<T>::m_rnd_prev;
template <uint8_t T> uint8_t         Osc<T>::m_red_noise;
template <uint8_t T> boolean         Osc<T>::m_note_on[2];
template <uint8_t T> boolean         Osc<T>::m_pitch_eg_target_both;
template <uint8_t T> int16_t         Osc<T>::m_pitch_eg_amt;
template <uint8_t T> __uint24        Osc<T>::m_lfsr;
