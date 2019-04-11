#pragma once

// refs http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

#include "common.h"
#include "mul-q.h"
#include "filter-table.h"

template <uint8_t T>
class Filter {
  static const uint8_t* m_lpf_table;
  static uint8_t        m_b_2_over_a_0_low;
  static int8_t         m_b_2_over_a_0_high;
  static uint8_t        m_a_1_over_a_0_low;
  static int8_t         m_a_1_over_a_0_high;
  static int16_t        m_x_1;
  static int16_t        m_x_2;
  static int16_t        m_y_1;
  static int16_t        m_y_2;
  static uint8_t        m_cutoff_current;
  static int16_t        m_cutoff_candidate;
  static uint8_t        m_cutoff;
  static int8_t         m_cutoff_env_gen_amt;
  static int8_t         m_cutoff_lfo_amt;
  static uint8_t        m_cutoff_expression_decrease;
  static int8_t         m_cutoff_exp_amt;

  static const uint8_t AUDIO_FRACTION_BITS = 14;
  static const int16_t MAX_ABS_OUTPUT = 127 << (AUDIO_FRACTION_BITS - 8);

public:
  INLINE static void initialize() {
    m_x_1 = 0;
    m_x_2 = 0;
    m_y_1 = 0;
    m_y_2 = 0;
    m_cutoff_current = 127;
    set_cutoff(127);
    set_resonance(0);
    set_cutoff_env_amt(64);
    set_cutoff_lfo_amt(64);
    set_cutoff_exp_amt(0);
    set_expression(127);
    update_coefs_1st(0, 0);
    update_coefs_2nd();
    update_coefs_3rd();
  }

  INLINE static void set_cutoff(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff = value;
  }

  INLINE static void set_resonance(uint8_t controller_value) {
    m_lpf_table = g_filter_lpf_tables[(controller_value + 4) >> 4];
  }

  INLINE static void set_cutoff_env_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_env_gen_amt = (value - 64) << 1;
  }

  INLINE static void set_cutoff_lfo_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_lfo_amt = (value - 64) << 1;
  }

  INLINE static void set_cutoff_exp_amt(uint8_t controller_value) {
    uint8_t value = controller_value;
    if (value < 4) {
      value = 4;
    } else if (124 < value) {
      value = 124;
    }

    m_cutoff_exp_amt = (value - 64) << 1;
  }

  INLINE static void set_expression(uint8_t controller_value) {
    m_cutoff_expression_decrease = 254 - (controller_value << 1);
  }

  INLINE static void note_on() {
  }

  INLINE static int16_t clock(uint8_t count, int16_t audio_input, uint8_t env_gen_input, int16_t lfo_input) {
    if ((count & (FILTER_CONTROL_INTERVAL - 1)) == 6) {
      uint8_t idx = (count >> FILTER_CONTROL_INTERVAL_BITS) & 0x03;
      switch (idx) {
      case 0x0:
        update_coefs_1st(env_gen_input, lfo_input);
        break;
      case 0x1:
        update_coefs_2nd();
        break;
      case 0x2:
        update_coefs_3rd();
        break;
      }
    }

    int16_t b_2_over_a_0 = m_b_2_over_a_0_low | (m_b_2_over_a_0_high << 8);
    int16_t a_1_over_a_0 = m_a_1_over_a_0_low | (m_a_1_over_a_0_high << 8);
    int16_t a_2_over_a_0 = (b_2_over_a_0 << 2) - a_1_over_a_0 -
                           (1 << FILTER_TABLE_FRACTION_BITS);

    int16_t x_0  = audio_input >> (16 - AUDIO_FRACTION_BITS);
    int16_t tmp  = mul_q15_q15(x_0 + (m_x_1 << 1) + m_x_2, b_2_over_a_0);
    tmp         -= mul_q15_q15(m_y_1,                      a_1_over_a_0);
    tmp         -= mul_q15_q15(m_y_2,                      a_2_over_a_0);
    int16_t y_0  = tmp << (16 - FILTER_TABLE_FRACTION_BITS);

    if (y_0 > MAX_ABS_OUTPUT) {
      y_0 = MAX_ABS_OUTPUT;
    }
    if (y_0 < -MAX_ABS_OUTPUT) {
      y_0 = -MAX_ABS_OUTPUT;
    }

    m_x_2 = m_x_1;
    m_y_2 = m_y_1;
    m_x_1 = x_0;
    m_y_1 = y_0;

    return y_0 << (16 - AUDIO_FRACTION_BITS);
  }

private:
  INLINE static void update_coefs_1st(uint8_t env_gen_input, int16_t lfo_input) {
    m_cutoff_candidate = m_cutoff;
    m_cutoff_candidate -= high_sbyte(m_cutoff_exp_amt * m_cutoff_expression_decrease);
    m_cutoff_candidate += high_sbyte((m_cutoff_env_gen_amt * env_gen_input) << 1);
    int8_t lfo_mod_half = high_sbyte(mul_q15_q7(lfo_input, m_cutoff_lfo_amt) << 1);
    m_cutoff_candidate -= lfo_mod_half;
    m_cutoff_candidate -= lfo_mod_half;
  }

  INLINE static void update_coefs_2nd() {
    uint8_t cutoff_target;
    if (m_cutoff_candidate > 127) {
      cutoff_target = 127;
    } else if (m_cutoff_candidate < 0) {
      cutoff_target = 0;
    } else {
      cutoff_target = m_cutoff_candidate;
    }

    if (m_cutoff_current + FILTER_CUTOFF_THROUGH_RATE < cutoff_target) {
      m_cutoff_current += FILTER_CUTOFF_THROUGH_RATE;
    } else if (m_cutoff_current > cutoff_target + FILTER_CUTOFF_THROUGH_RATE) {
      m_cutoff_current -= FILTER_CUTOFF_THROUGH_RATE;
    } else {
      m_cutoff_current = cutoff_target;
    }
  }

  INLINE static void update_coefs_3rd() {
    const uint8_t* p = m_lpf_table + (m_cutoff_current << 2);
    uint32_t four_data = pgm_read_dword(p);
    m_b_2_over_a_0_low  = (four_data >>  0) & 0xFF;
    m_b_2_over_a_0_high = (four_data >>  8) & 0xFF;
    m_a_1_over_a_0_low  = (four_data >> 16) & 0xFF;
    m_a_1_over_a_0_high = (four_data >> 24) & 0xFF;
  }
};

template <uint8_t T> const uint8_t* Filter<T>::m_lpf_table;
template <uint8_t T> uint8_t        Filter<T>::m_b_2_over_a_0_low;
template <uint8_t T> int8_t         Filter<T>::m_b_2_over_a_0_high;
template <uint8_t T> uint8_t        Filter<T>::m_a_1_over_a_0_low;
template <uint8_t T> int8_t         Filter<T>::m_a_1_over_a_0_high;
template <uint8_t T> int16_t        Filter<T>::m_x_1;
template <uint8_t T> int16_t        Filter<T>::m_x_2;
template <uint8_t T> int16_t        Filter<T>::m_y_1;
template <uint8_t T> int16_t        Filter<T>::m_y_2;
template <uint8_t T> uint8_t        Filter<T>::m_cutoff_current;
template <uint8_t T> int16_t        Filter<T>::m_cutoff_candidate;
template <uint8_t T> uint8_t        Filter<T>::m_cutoff;
template <uint8_t T> int8_t         Filter<T>::m_cutoff_env_gen_amt;
template <uint8_t T> int8_t         Filter<T>::m_cutoff_lfo_amt;
template <uint8_t T> uint8_t        Filter<T>::m_cutoff_expression_decrease;
template <uint8_t T> int8_t         Filter<T>::m_cutoff_exp_amt;
