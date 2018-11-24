/*
 * button.cpp:
 *
 * Button function implementations
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include "button.hpp"
//Initialize static pointer
Button* Button::s_interrupt = NULL;
/**
 * ISR handler for use in interrupt registry
 */
void button_isr() {
    //Don't call button if NULL
    if (Button::s_interrupt != NULL) {
        Button::s_interrupt->handle();
    }
}

/**
 * Button constructor implementation. This handles interrupt registry and
 * setting up the pin type.
 */
Button::Button(int pin, int debounce, ButtonType type, bool interrupt) :
    m_pin(pin),
    m_debounce(debounce),
    m_last(0),
    m_handler(NULL),
    m_type(type)
{
    pinMode(pin, INPUT_PULLUP);
    //Free to handle interrupt, and want to handle interrupt
    if (Button::s_interrupt != NULL && interrupt) {
        attachInterrupt(digitalPinToInterrupt(m_pin), button_isr, EDGE);
        m_interrupt = true;
    } else {
        m_interrupt = false;
    }
}
/**
 * Register the button handler.
 */
void Button::register_handler(ButtonHandle handler) {
    m_handler = handler;
}
/**
 * Handle the button press. This includes debouncing and calling registered
 * callback function.
 */
void Button::handle() {
    unsigned int current = millis();
    //Brake out early when debouncing
    //Note: will fail safely on overflow (shorter debounce interval)
    if ((current > m_last) && (current < (m_last + m_debounce))) {
        return;
    }
    //Call registered callback
    else if (m_handler != NULL) {
        m_handler(m_type);
    }
    m_last = current;
}
/**
 * Run handler called every N milliseconds. Here the button state is polled
 * every N milliseconds, if not in interrupt mode.
 */
void Button::run() {
    //If not interrupt driven and the pin is "active" trigger press
    if (!m_interrupt && digitalRead(m_pin) == ACTIVE) {
        this->handle();
    }
}

