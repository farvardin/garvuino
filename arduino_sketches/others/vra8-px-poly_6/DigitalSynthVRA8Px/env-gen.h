#pragma once

#include "common.h"
#include "mul-q.h"

template <uint8_t T>
class EnvGen {
  static const uint8_t STATE_ATTACK  = 0;
  static const uint8_t STATE_DECAY   = 1;
  static const uint8_t STATE_RELEASE = 2;
  static const uint8_t STATE_IDLE    = 3;

  static const uint8_t ATTACK_UPDATE_INTERVAL  = 1;
  static const uint8_t RELEASE_UPDATE_INTERVAL = 1;

  static uint8_t  m_state;
  static uint16_t m_level;
  static uint8_t  m_count;
  static uint16_t m_attack_step;
  static uint8_t  m_decay_update_interval;
  static uint8_t  m_rest;

public:
  INLINE static void initialize() {
    m_state = STATE_RELEASE;
    m_level = 0;
    m_count = 3;
    set_attack(64);
    set_decay(64);
    m_rest = RELEASE_UPDATE_INTERVAL;
  }

  INLINE static void set_attack(uint8_t controller_value) {
    m_attack_step = (static_cast<uint16_t>(((127 - controller_value) << 1) + 1) *
                                          (((127 - controller_value) << 1) + 1) >> 4) + 4;
  }

  INLINE static void set_decay(uint8_t controller_value) {
    m_decay_update_interval = high_byte((controller_value << 1) *
                                        (controller_value << 1)) + 1;
  }

  INLINE static void note_on() {
    m_state = STATE_ATTACK;
    m_rest = ATTACK_UPDATE_INTERVAL;
  }

  INLINE static void note_off() {
    m_state = STATE_RELEASE;
    m_rest = RELEASE_UPDATE_INTERVAL;
  }

  INLINE static uint8_t clock() {
    m_count++;
    if ((m_count & (ENV_GEN_CONTROL_INTERVAL - 1)) == 0) {
      switch (m_state) {
      case STATE_ATTACK:
        m_rest--;
        if (m_rest == 0) {
          m_rest = ATTACK_UPDATE_INTERVAL;
          if (m_level >= ENV_GEN_LEVEL_MAX - m_attack_step) {
            m_level = ENV_GEN_LEVEL_MAX;
            m_state = STATE_DECAY;
            m_rest = m_decay_update_interval;
          } else {
            m_level += m_attack_step;
          }
        }
        break;
      case STATE_DECAY:
        m_rest--;
        if (m_rest == 0) {
          m_rest = m_decay_update_interval;
          if (m_level < 0x0100) {
            m_level = 0;
            m_rest = 255;
          } else if (m_decay_update_interval < 253) {
            m_level = mul_q16_q8(m_level, ENV_GEN_DECAY_FACTOR);
          }
        }
        break;
      case STATE_RELEASE:
        m_rest--;
        if (m_rest == 0) {
          m_rest = RELEASE_UPDATE_INTERVAL;
          if (m_level <= ENV_GEN_LEVEL_RELEASE_STEP) {
            m_level = 0;
            m_state = STATE_IDLE;
          } else {
            m_level -= ENV_GEN_LEVEL_RELEASE_STEP;
          }
        }
        break;
      default:  // case STATE_IDLE:
        m_level = 0;
        break;
      }
    }

    return high_byte(m_level);
  }
};

template <uint8_t T> uint8_t  EnvGen<T>::m_count;
template <uint8_t T> uint8_t  EnvGen<T>::m_state;
template <uint8_t T> uint16_t EnvGen<T>::m_level;
template <uint8_t T> uint16_t EnvGen<T>::m_attack_step;
template <uint8_t T> uint8_t  EnvGen<T>::m_decay_update_interval;
template <uint8_t T> uint8_t  EnvGen<T>::m_rest;
