/*
 * oled.hpp:
 *
 * This file powers the ScaleAV OLED screen. It allows the ScaleAV team to
 * control the OLED screen. This screen will take the following inputs above
 * and beyond the basic indicator properties.
 *
 * 1. IP Address: will set the IP address of the machine, once known.
 * 2. Room name: will set the IP address of the box, once known.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#ifndef SRC_OLED_HPP_
#define SRC_OLED_HPP_
//Wire and SPI libraries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
//Includes for Adafruit library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "types.hpp"
#include "indicator.hpp"
class OLED : public Indicator
{
    public:
        /**
         * Constructor, must set initial data for IP and name.
         */
        OLED();
        /**
         * Setup this OLED screen.
         */
        bool setup();
        /**
	 * A button has been pressed.  We only care about the front button.
	 */
        void button_pressed(ButtonType button);
        /**
         * Overrides run to provide OLED specific actions
         */
        void run();
    protected:
        //!< OLED screen to display to
        Adafruit_SSD1306 m_display;
        //!< Index of current display
        uint8_t m_index;
        //!< Current count 
        uint8_t m_update_count;
        //!< Updated message
        bool m_updated;
        //!< First error
        bool m_first_error;
};
#endif /* SRC_OLED_HPP_ */
