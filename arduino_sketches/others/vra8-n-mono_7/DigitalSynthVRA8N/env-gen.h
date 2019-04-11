#pragma once

#include "common.h"
#include "mul-q.h"

template <uint8_t T>
class EnvGen {
  static const uint8_t STATE_ATTACK  = 0;
  static const uint8_t STATE_SUSTAIN = 1;
  static const uint8_t STATE_IDLE    = 2;

  static uint8_t  m_state;
  static uint16_t m_level;
  static uint8_t  m_levelOut;
  static uint8_t  m_attack_update_coef;
  static uint8_t  m_decay_update_coef;
  static uint8_t  m_release_update_coef;
  static uint16_t m_sustain;
  static uint8_t  m_rest;
  static uint8_t  m_expression;
  static uint8_t  m_amp_exp_amt;
  static uint8_t  m_expression_coef;

public:
  INLINE static void initialize() {
    m_state = STATE_IDLE;
    m_level = 0;
    m_levelOut = 0;
    set_attack(0);
    set_decay(0);
    set_sustain(127);
    set_release(0);
    set_expression(127);
    set_amp_exp_amt(127);
  }

  INLINE static void set_attack(uint8_t controller_value) {
    m_attack_update_coef = ((controller_value >> 1) << 1) + 2;
  }

  INLINE static void set_decay(uint8_t controller_value) {
    m_decay_update_coef = ((controller_value >> 1) << 1) + 2;
  }

  INLINE static void set_release(uint8_t controller_value) {
    m_release_update_coef = ((controller_value >> 1) << 1) + 2;
  }

  INLINE static void set_sustain(uint8_t controller_value) {
    if (controller_value == 127) {
      m_sustain = 128 << 8;
    } else if (controller_value < 4) {
      m_sustain = 0 << 8;
    } else {
      m_sustain = controller_value << 8;
    }
  }

  // EXPRESSION is processed here (not in Amp) for performance reasons
  INLINE static void set_expression(uint8_t controller_value) {
    m_expression = (controller_value << 1) + 1;
    update_expression_coef();
  }

  INLINE static void set_amp_exp_amt(uint8_t controller_value) {
    m_amp_exp_amt = (controller_value << 1) + 1;
    update_expression_coef();
  }

  INLINE static void note_on() {
    m_state = STATE_ATTACK;
    m_rest = m_attack_update_coef;
  }

  INLINE static void note_off() {
    m_state = STATE_IDLE;
    m_rest = m_decay_update_coef;
  }

  INLINE static uint8_t clock(uint8_t count) {
    if ((count & (ENV_GEN_CONTROL_INTERVAL - 1)) == ((T == 0) ? 2 : 2)) {
      switch (m_state) {
      case STATE_ATTACK:
        m_rest--;
        if (m_rest == 0) {
          m_rest = m_attack_update_coef;
          m_level = ENV_GEN_LEVEL_MAX_X_1_5 - mul_q16_q8(ENV_GEN_LEVEL_MAX_X_1_5 - m_level,
                                                         188 + (m_attack_update_coef >> 1));
          if (m_level >= ENV_GEN_LEVEL_MAX) {
            m_level = ENV_GEN_LEVEL_MAX;
            m_state = STATE_SUSTAIN;
            m_rest = m_decay_update_coef;
          }
        }
        break;
      case STATE_SUSTAIN:
        m_rest--;
        if (m_rest == 0) {
          m_rest = m_decay_update_coef;
          if (m_level > m_sustain) {
            m_level = m_sustain + mul_q16_q8(m_level - m_sustain,
                                             188 + (m_decay_update_coef >> 1));
            if (m_level < m_sustain) {
              m_level = m_sustain;
            }
          }
        }
        break;
      case STATE_IDLE:
        m_rest--;
        if (m_rest == 0) {
          m_rest = m_release_update_coef;
          if (m_level > 0) {
            m_level = mul_q16_q8(m_level, 188 + (m_release_update_coef >> 1));
            if (m_level < ((T == 0) ? 0x0100 : 0x0400 /* gate for amp */)) {
              m_level = 0;
            }
          }
        }
        break;
      }

      if (T == 1) {
        m_levelOut = high_byte(high_byte(m_level) * m_expression_coef) << 1;
      } else {
        m_levelOut = high_byte(m_level);
      }
    }

    return m_levelOut;
  }

private:
  INLINE static void update_expression_coef() {
    uint8_t expression = 255 - high_byte(static_cast<uint8_t>(255 - m_expression) * m_amp_exp_amt);
    m_expression_coef = high_byte(expression * expression);
  }
};

template <uint8_t T> uint8_t  EnvGen<T>::m_state;
template <uint8_t T> uint16_t EnvGen<T>::m_level;
template <uint8_t T> uint8_t  EnvGen<T>::m_levelOut;
template <uint8_t T> uint8_t  EnvGen<T>::m_attack_update_coef;
template <uint8_t T> uint8_t  EnvGen<T>::m_decay_update_coef;
template <uint8_t T> uint8_t  EnvGen<T>::m_release_update_coef;
template <uint8_t T> uint16_t EnvGen<T>::m_sustain;
template <uint8_t T> uint8_t  EnvGen<T>::m_rest;
template <uint8_t T> uint8_t  EnvGen<T>::m_expression;
template <uint8_t T> uint8_t  EnvGen<T>::m_amp_exp_amt;
template <uint8_t T> uint8_t  EnvGen<T>::m_expression_coef;
