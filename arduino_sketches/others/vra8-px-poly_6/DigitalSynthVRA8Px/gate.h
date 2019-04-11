#pragma once

#include "common.h"
#include "mul-q.h"

template <uint8_t T>
class Gate {
  static uint8_t m_target_array[4];
  static uint8_t m_current_array[4];
  static uint8_t m_count;

public:
  INLINE static void initialize() {
    m_target_array[0] = 0;
    m_target_array[1] = 0;
    m_target_array[2] = 0;
    m_target_array[3] = 0;
    m_current_array[0] = 0;
    m_current_array[1] = 0;
    m_current_array[2] = 0;
    m_current_array[3] = 0;
    m_count = 1;
  }

  INLINE static void note_on(uint8_t eg_number, uint8_t velocity) {
    uint8_t target = high_byte((velocity << 1) * (velocity << 1)) >> 2;
    if (target == 0) {
      target = 1;
    }
    m_target_array[eg_number] = target;
  }

  INLINE static void note_off(uint8_t eg_number) {
    m_target_array[eg_number] = 0;
  }

  INLINE static void clock() {
    m_count++;
    if ((m_count & (GATE_CONTROL_INTERVAL - 1)) == 0) {
      update_level<0>();
      update_level<1>();
      update_level<2>();
      update_level<3>();
    }
  }

  template <uint8_t N>
  INLINE static uint8_t level() {
    return m_current_array[N];
  }

private:
  template <uint8_t N>
  INLINE static void update_level() {
    if (m_current_array[N] + GATE_LEVEL_STEP < m_target_array[N]) {
      m_current_array[N] += GATE_LEVEL_STEP;
    } else if (m_current_array[N] > m_target_array[N] + GATE_LEVEL_STEP) {
      m_current_array[N] -= GATE_LEVEL_STEP;
    } else {
      m_current_array[N] = m_target_array[N];
    }
  }
};

template <uint8_t T> uint8_t Gate<T>::m_target_array[4];
template <uint8_t T> uint8_t Gate<T>::m_current_array[4];
template <uint8_t T> uint8_t Gate<T>::m_count;
