/*
 * rgb.hpp:
 *
 * Red, Green, and Blue LED. This indicator uses color to communicate status to
 * the user. The RGB led operates by sending PWM signals to set the ratio of the
 * color (think RGB values for color).
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#ifndef SRC_RGB_HPP_
#define SRC_RGB_HPP_
#include "types.hpp"
#include "indicator.hpp"
//!< Number of colors in RGB led
#define COLOR_COUNT 3
//!< Red's index in arrays
#define RED 0
//!< Green's index in arrays
#define GREEN 1
//!< Blue's index in arrays
#define BLUE 2

class RGB : public Indicator {
    public:
        //Note: when resistors are installed, set to 0
        //!< Force PWM down by this power of 2 to prevent overload
        const int PWM_SHIFT = 2;
        //!< Step size per interval roughly 1 waypoint per second
        const int PWM_STEP = (255 * RATE_GROUP_PERIOD)/MS_PER_SECOND;
        /**
         * Constructor to set the pins for the RGB leds.
         */
        RGB(int rpin, int gpin, int bpin);
        /**
         * Run every 1/N ms. Should display RED on error, or make one
         * step to change color between this and the next step.
         */
        void run();
    private:
        //!< Waypoints for normal mode, will be subdivided into sets of 3
        static const unsigned int POINTS[];
        //!< Waypoints for error mode, will be subdivided into sets of 3
        static const unsigned int ERROR_POINTS[];
        //!< Count for pressed
        unsigned int m_countdown;
        //!< Pins to write to, in order to output color
        int m_pin[COLOR_COUNT];
        //!< Index of last waypoint reached
        unsigned int m_index;
        //!< Current R,G,B colors of the LED
        unsigned int m_color[COLOR_COUNT];
};
#endif /* SRC_RGB_HPP_ */
