/*
 * led13.hpp:
 *
 * A use of the onboard LED as an indicator. This will simply blink the onboard
 * LED in case of error and will hold the LED solid-low in case of power on.
 * This acts as a fail-safe indicator, if the other indicators fail or are not
 * installed.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#ifndef SRC_LED13_HPP_
#define SRC_LED13_HPP_
#include "indicator.hpp"
#define SWITCH_PERIOD_MS 100
class LED13 : public Indicator {
    public:
        /**
         * Construct the LED13. Pass in the on-board pin. This is always
         * pin 13, but for transparency, this should be passed in.
         */
        LED13(int pin);

        /**
         * Run function called every N milliseconds.
         */
        void run();
    protected:
        //!< Pin used for onboard LED, stored locally
        int m_pin;
        //!< State of the LED
        int m_state;
};
#endif /* SRC_LED13_HPP_ */
