/*
 * button.hpp:
 *
 * Represents a button, polled or interrupt triggered. It provides software
 * debouncing. This cleans up the input. A registered callback function
 * is called when the button is pressed (polled or triggered) Note: since the
 * Arduino only offers pull-up resistors, this button is active low or
 * falling edge driven.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#ifndef SRC_BUTTON_HPP_
#define SRC_BUTTON_HPP_
#include <Arduino.h>
#include "types.hpp"
#include "runner.hpp"
//!< External handler for button
typedef void (*ButtonHandle)(ButtonType button);
class Button : public Runner
{
    //!< Buttons are active low when pressed
    const int ACTIVE = LOW;
    //!< Buttons trigger on a falling edge
    const int EDGE = FALLING;

    public:
        /**
         * Constructor. Wraps pin. If set to interrupt, will trigger on
         * interrupt, unless another pin is already using an interrupt.
         * \param int pin: pin to wrap
         * \param int debounce: debounce interval in ms
         * \param ButtonType type: type of this button
         * \param bool interrupt: should this attempt to be interrupt driven?
         */
        Button(int pin, int debounce, ButtonType type, bool interrupt);

        /**
         * Register a handler for this buttons press event. Note: if handling
         * interrupt based clicks, this must be fast as it will run in
         * interrupt context.
         * \param ButtonHandle handler: handler function to call on press
         */
        void register_handler(ButtonHandle handler);

        /**
         * Helper function called on press of the button.
         */
        void handle();

        /**
         * Run handler called every 1/Nms
         */
        void run();

        //!< Only one button interrupt allowed, the rest will be polled.
        static Button* s_interrupt;
    private:
        //!< Pin to wrap
        int m_pin;
        //!< Debounce interval in milliseconds
        int m_debounce;
        //!< Last pressed time (fill will millis() call
        unsigned int m_last;
        //!< Button press handler function
        ButtonHandle m_handler;
        //!< Button type of this button
        ButtonType m_type;
        //!< Interrupt driven or polling
        bool m_interrupt;
};

#endif /* SRC_BUTTON_HPP_ */
