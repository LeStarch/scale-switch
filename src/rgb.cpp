/*
 * rgb.cpp:
 *
 * RGB implementation functions, and definition of the waypoints (for the
 * pretty rainbow).
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include <Arduino.h>
#include <string.h>
#include "rgb.hpp"

//!< Maximum macro
#define MAX(a,b) ((a >= b)?a:b)
//!< Minimum macro
#define MIN(a,b) ((a <= b)?a:b)

//!< Note: store this memory in program (FLASH) not in stack. Save memory.
//!< Array of arrays of color values. 0xFF is full-on, 0 is off.
const unsigned int RGB::POINTS[] PROGMEM = {
        0xFF, 0x00, 0x00, //!< RED
        0xFF, 0xFF, 0x00, //!< YELLOW
        0x00, 0xFF, 0x00, //!< GREEN
        0x00, 0xFF, 0xFF, //!< BLUE-GREEN
        0x00, 0x00, 0xFF, //!< BLUE
        0xFF, 0x00, 0xFF, //!< PURPLE
};
//!< Color to display on error
const unsigned int RGB::ERROR_POINTS[] PROGMEM = {
        0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00
};
/**
 * Attach to given pins, and set their types.
 */
RGB::RGB(int rpin, int gpin, int bpin) :
    m_countdown(0),
    m_index(0),
    m_started(false)
{
    m_pin[RED] = rpin;
    m_pin[GREEN] = gpin;
    m_pin[BLUE] = bpin;
    //Setup output pins for all colors
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(m_pin); i++) {
        pinMode(m_pin[i], OUTPUT);
    }
    //Initialize color to be the first three words
    m_color[0] = pgm_read_word_near(POINTS + 0);
    m_color[1] = pgm_read_word_near(POINTS + 1);
    m_color[2] = pgm_read_word_near(POINTS + 2);
}
void RGB::message(const char* key, const char* msg) {
     m_started = true;
}
/**
 * Run function slowly walks through way-points. Each waypoint represents an equi-
 * distant point to walk through.
 */
void RGB::run() {
    //ASSERT(COLOR_COUNT == NUM_ARRAY_ELEMENTS(m_pin), "Color count failure");
    unsigned int next_index = 0;
    unsigned int current[COLOR_COUNT]; //Current waypoint
    unsigned int next[COLOR_COUNT]; //Next waypoint
    const unsigned int *waypoints;
    // A podium button was pressed
    if (m_pressed[BUTTON_PODIUM]) {
        m_pressed[BUTTON_PODIUM] = false;
        m_countdown = 30;
        analogWrite(m_pin[BLUE], 0xFF);
        analogWrite(m_pin[RED], 0);
        analogWrite(m_pin[GREEN], 0);
        return;
    }
        // Presse expiration interval
    else if (m_countdown > 0) {
        m_countdown--;
        analogWrite(m_pin[BLUE], 0xFF - (m_countdown << 3));
        return;
    }
        //Assign the waypoint pointer, and next index based on the error state
    else if (s_error_state) {
        next_index = (m_index + COLOR_COUNT) % NUM_ARRAY_ELEMENTS(ERROR_POINTS);
        waypoints = ERROR_POINTS;
    } else if (!m_started) {
        return;
    } else {
        next_index = (m_index + COLOR_COUNT) % NUM_ARRAY_ELEMENTS(POINTS);
        waypoints = POINTS;
    }
    //Assign current and next pointers
    for (unsigned int i = 0; i < COLOR_COUNT; i++) {
        current[i] = pgm_read_word_near(waypoints + m_index + i);
        next[i] = pgm_read_word_near(waypoints + next_index + i);
    }
    //Update each color by doing the following:
    // 1. Calculate the distance between this and next point as scalar
    // 2. Multiply by current step size
    // 3. Add value to color (at least +1 or -1)
    for (unsigned int i = 0; i < COLOR_COUNT; i++) {
        int dist = next[i] - current[i];
        float add = ((float)(dist))/255.0f * PWM_STEP;
        //Less than distance bottom out at next waypoint
        if (add < -0.001) {
            add = MIN(add, -1.1f); //Step at least 1
            m_color[i] = MAX(m_color[i] + (int)add, next[i]);
        }
        //Otherwise max out at next waypoint
        else if (add > 0.001) {
            add = MAX(add, 1.1f); //Step at least 1
            m_color[i] = MIN(m_color[i] + (int)add, next[i]);
        }
    }
    //Loop over colors and set their output
    //Also note: shift output to keep from overloading LEDs
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(m_pin); i++) {
        //Cap color to prevent out-of-bounds color
        m_color[i] = MIN(m_color[i], 0xFF);
        m_color[i] = MAX(m_color[i], 0x00);
        analogWrite(m_pin[i], m_color[i] >> PWM_SHIFT);
    }
    //Now update the pointer
    if (Runner::interval_check(1000)) {
        //Jump to the next waypoint
        memcpy(m_color, next, sizeof(m_color));
        m_index = next_index;
    }
}
