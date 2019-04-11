#pragma once

// refs http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

#include "common.h"
#include "mul-q.h"
#include "filter-table.h"

template <uint8_t T>
class Filter {
  static uint8_t        m_count;
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
  static uint8_t        m_cutoff;
  static uint8_t        m_cutoff_velocity;
  static uint8_t        m_mod_amt;
  static uint8_t        m_noise_gen_amt;
  static uint16_t       m_rnd;

  static const uint8_t AUDIO_FRACTION_BITS = 14;

public:
  INLINE static void initialize() {
    m_count = 2;
    m_x_1 = 0;
    m_x_2 = 0;
    m_y_1 = 0;
    m_y_2 = 0;
    m_cutoff_current = 127;
    m_cutoff_velocity = 64;
    set_cutoff(127);
    set_resonance(0);
    set_env_amt(64);
    set_noise_gen_amt(0);
    update_coefs(0);
    m_rnd = 1;
  }

  INLINE static void set_cutoff(uint8_t controller_value) {
    m_cutoff = controller_value;
  }

  INLINE static void set_resonance(uint8_t controller_value) {
    m_lpf_table = g_filter_lpf_tables[(controller_value + 4) >> 4];
  }

  INLINE static void set_env_amt(uint8_t controller_value) {
    m_mod_amt = controller_value;
  }

  INLINE static void set_noise_gen_amt(uint8_t controller_value) {
    m_noise_gen_amt = controller_value;
  }

  INLINE static void note_on(uint8_t cutoff_velocity) {
    m_cutoff_velocity = cutoff_velocity;
  }

  INLINE static int16_t clock(int16_t audio_input, uint8_t mod_input) {
    m_count++;
    uint8_t count_and_interval = m_count & (FILTER_CONTROL_INTERVAL - 1);
    if (count_and_interval == 0) {
      update_coefs(mod_input);
    } else if (count_and_interval == 3) {
      update_rnd();
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

    if (y_0 > ((1 << (AUDIO_FRACTION_BITS - 1)) - 1)) {
      y_0 = ((1 << (AUDIO_FRACTION_BITS - 1)) - 1);
    }
    if (y_0 < -(1 << (AUDIO_FRACTION_BITS - 1))) {
      y_0 = -(1 << (AUDIO_FRACTION_BITS - 1));
    }

    m_x_2 = m_x_1;
    m_y_2 = m_y_1;
    m_x_1 = x_0;
    m_y_1 = y_0;

    return y_0 << (16 - AUDIO_FRACTION_BITS);
  }

  INLINE static uint8_t get_rnd8() {
    return low_byte(m_rnd);
  }

private:
  INLINE static void update_coefs(uint8_t mod_input) {
    int16_t cutoff_candidate = m_cutoff + static_cast<int8_t>(m_cutoff_velocity - 64);
    cutoff_candidate += high_sbyte(((m_mod_amt - 64) << 1) * mod_input);
    cutoff_candidate += high_sbyte(((m_noise_gen_amt - 64) << 1) * get_rnd8());
    uint8_t cutoff_target;
    if (cutoff_candidate > 127) {
      cutoff_target = 127;
    } else if (cutoff_candidate < 0) {
      cutoff_target = 0;
    } else {
      cutoff_target = cutoff_candidate;
    }

    if (m_cutoff_current + FILTER_CUTOFF_THROUGH_RATE < cutoff_target) {
      m_cutoff_current += FILTER_CUTOFF_THROUGH_RATE;
    } else if (m_cutoff_current > cutoff_target + FILTER_CUTOFF_THROUGH_RATE) {
      m_cutoff_current -= FILTER_CUTOFF_THROUGH_RATE;
    } else {
      m_cutoff_current = cutoff_target;
    }

    const uint8_t* p = m_lpf_table + (m_cutoff_current << 2);
    uint32_t four_data = pgm_read_dword(p);
    m_b_2_over_a_0_low  = (four_data >>  0) & 0xFF;
    m_b_2_over_a_0_high = (four_data >>  8) & 0xFF;
    m_a_1_over_a_0_low  = (four_data >> 16) & 0xFF;
    m_a_1_over_a_0_high = (four_data >> 24) & 0xFF;
  }

  INLINE static void update_rnd() {
    m_rnd = m_rnd ^ (m_rnd << 5);
    m_rnd = m_rnd ^ (m_rnd >> 9);
    m_rnd = m_rnd ^ (m_rnd << 8);
  }
};

template <uint8_t T> uint8_t        Filter<T>::m_count;
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
template <uint8_t T> uint8_t        Filter<T>::m_cutoff;
template <uint8_t T> uint8_t        Filter<T>::m_cutoff_velocity;
template <uint8_t T> uint8_t        Filter<T>::m_mod_amt;
template <uint8_t T> uint8_t        Filter<T>::m_noise_gen_amt;
template <uint8_t T> uint16_t       Filter<T>::m_rnd;
