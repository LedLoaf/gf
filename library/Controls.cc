/*
 * Gamedev Framework (gf)
 * Copyright (C) 2016-2017 Julien Bernard
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#include <gf/Controls.h>

#include <cassert>

#include <gf/Event.h>

namespace gf {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
inline namespace v1 {
#endif

  // keycode key control

  KeycodeKeyControl::KeycodeKeyControl(Keycode code)
  : m_code(code)
  {

  }

  void KeycodeKeyControl::processEvent(const Event& event) {
    if (event.type == EventType::KeyPressed) {
      if (event.key.keycode == m_code) {
        setActive();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.keycode == m_code) {
        setActive(false);
      }
    }
  }

  // scancode key control

  ScancodeKeyControl::ScancodeKeyControl(Scancode code)
  : m_code(code)
  {

  }

  void ScancodeKeyControl::processEvent(const Event& event) {
    if (event.type == EventType::KeyPressed) {
      if (event.key.scancode == m_code) {
        setActive();
      }
    }

    if (event.type == EventType::KeyReleased) {
      if (event.key.scancode == m_code) {
        setActive(false);
      }
    }
  }

  // mouse button control

  MouseButtonControl::MouseButtonControl(MouseButton button)
  : m_button(button)
  {

  }

  void MouseButtonControl::processEvent(const Event& event) {
    if (event.type == EventType::MouseButtonPressed) {
      if (event.mouseButton.button == m_button) {
        setActive();
      }
    }

    if (event.type == EventType::MouseButtonReleased) {
      if (event.mouseButton.button == m_button) {
        setActive(false);
      }
    }
  }

  // gamepad button control

  GamepadButtonControl::GamepadButtonControl(GamepadId id, GamepadButton button)
  : m_id(id)
  , m_button(button)
  {

  }

  void GamepadButtonControl::processEvent(const Event& event) {
    if (event.type == EventType::GamepadButtonPressed) {
      if (event.gamepadButton.id == m_id && event.gamepadButton.button == m_button) {
        setActive();
      }
    }

    if (event.type == EventType::GamepadButtonReleased) {
      if (event.gamepadButton.id == m_id && event.gamepadButton.button == m_button) {
        setActive(false);
      }
    }
  }

  // gamepad axis control

  GamepadAxisControl::GamepadAxisControl(GamepadId id, GamepadAxis axis, GamepadAxisDirection dir)
  : m_id(id)
  , m_axis(axis)
  , m_dir(dir)
  {

  }

  static constexpr int16_t GamepadAxisThreshold = 8000;

  void GamepadAxisControl::processEvent(const Event& event) {
    if (event.type == EventType::GamepadAxisMoved) {
      if (event.gamepadAxis.id == m_id && event.gamepadAxis.axis == m_axis) {
        switch (m_dir) {
          case GamepadAxisDirection::Positive:
            setActive(event.gamepadAxis.value > GamepadAxisThreshold);
            break;
          case GamepadAxisDirection::Negative:
            setActive(event.gamepadAxis.value < -GamepadAxisThreshold);
            break;
        }
      }
    }
  }


  // close control

  CloseControl::CloseControl()
  {

  }

  void CloseControl::processEvent(const Event& event) {
    if (event.type == EventType::Closed) {
      setActive();
    }
  }


  // konami control

  namespace {

    constexpr int KonamiKeysCount = 10;

    Keycode nthKeyForKonami(int index) {
      assert(index < KonamiKeysCount);

      switch (index) {
        case 0:
        case 1:
          return Keycode::Up;
        case 2:
        case 3:
          return Keycode::Down;
        case 4:
        case 6:
          return Keycode::Left;
        case 5:
        case 7:
          return Keycode::Right;
        case 8:
          return Keycode::B;
        case 9:
          return Keycode::A;
      }

      assert(false);
      return Keycode::Unknown;
    }

  }

  KonamiKeyboardControl::KonamiKeyboardControl()
  : m_index(0)
  , m_state(Released)
  {

  }

  void KonamiKeyboardControl::processEvent(const Event& event) {
    if (m_state == Released && event.type == EventType::KeyPressed) {
      if (event.key.keycode == nthKeyForKonami(m_index)) {
        m_state = Pressed;
      } else {
        m_index = 0;
      }
    }

    if (m_state == Pressed && event.type == EventType::KeyReleased) {
      m_state = Released;

      if (event.key.keycode == nthKeyForKonami(m_index)) {
        ++m_index;
      } else {
        m_index = 0;
      }
    }

    if (m_index == KonamiKeysCount) {
      setActive();
      m_index = false;
    } else {
      setActive(false);
    }
  }

#ifndef DOXYGEN_SHOULD_SKIP_THIS
}
#endif
}
