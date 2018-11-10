/*
 * led13.cpp:
 *
 * LED13 implementations.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include <Arduino.h>
#include "led13.hpp"
/**
 * Sets-up and wraps onboard LED.
 */
LED13::LED13(int pin) : Indicator(),
    m_pin(pin),
    m_counter(-1),
    m_state(HIGH)
{
    pinMode(m_pin, OUTPUT);
    //Start-up reporting error. Once running the system will blink
    digitalWrite(m_pin, m_state);
}
/**
 * Run function will change LED state every 100ms, resulting in 5 blinks
 * per second. If the system enters error state, then the LED is held on.
 */
void LED13::run() {
    m_counter++;
    //Handle error case
    if (!m_error_state) {
        m_state = LOW;
    }
    //Handle normal operation
    else if ((m_counter * RATE_GROUP_PERIOD) >= SWITCH_PERIOD_MS) {
        m_state = !m_state;
        m_counter = -1;
    }
    digitalWrite(m_pin, m_state);
}
