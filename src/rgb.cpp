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

//!< Array of arrays of color values. 0xFF is full-on, 0 is off.
int POINTS[][COLOR_COUNT] = {
        {0xFF, 0x00, 0x00}, //!< RED
        {0xFF, 0xFF, 0x00}, //!< YELLOW
        {0x00, 0xFF, 0x00}, //!< GREEN
        {0x00, 0xFF, 0xFF}, //!< BLUE-GREEN
        {0x00, 0x00, 0xFF}, //!< BLUE
        {0xFF, 0x00, 0xFF}  //!< PURPLE
};
//!< Color to display on error
int ERROR_COLOR[] = {0xFF, 0x54, 0x90};
/**
 * Attach to given pins, and set their types.
 */
RGB::RGB(int rpin, int gpin, int bpin) :
    m_index(0),
    m_count(0)
{
    m_pin[RED] = rpin;
    m_pin[GREEN] = gpin;
    m_pin[BLUE] = bpin;
    //Setup output pins for all colors
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(m_pin); i++) {
        pinMode(m_pin[i], OUTPUT);
    }
    memcpy(m_color, POINTS[0], sizeof(m_color));
}

/**
 * Run function slowly walks through way-points. Each waypoint represents an equi-
 * distant point to walk through.
 */
void RGB::run() {
    //ASSERT(COLOR_COUNT == NUM_ARRAY_ELEMENTS(m_pin), "Color count failure");
    m_count += RATE_GROUP_PERIOD;
    //Find the next destination
    int next = (m_index + 1) % NUM_ARRAY_ELEMENTS(POINTS);
    //Handle errors, by copying in error color
    if (m_error_state) {
        memcpy(m_color, ERROR_COLOR, sizeof(m_color));
    }
    //Handle waypoints when not erring
    else {
        //Update each color by doing the following:
        // 1. Calculate the distance between this and next point as scalar
        // 2. Multiply by current step size
        // 3. Add value to color (at least +1 or -1)
        for (int i = 0; i < COLOR_COUNT; i++) {
            int new_color = 0;
            int dist = POINTS[next][i] - POINTS[m_index][i];
            float add = ((float)(dist))/255.0f * PWM_STEP;
            //Less than distance bottom out at next waypoint
            if (add < 0) {
                add = MIN(add, -1.1f); //Step at least 1
                new_color = MAX(m_color[i] + (int)add, POINTS[next][i]);
            }
            //Otherwise max out at next waypoint
            else {
                add = MAX(add, 1.1f); //Step at least 1
                new_color = MIN(m_color[i] + (int)add, POINTS[next][i]);
            }
            //Assign the new color
            m_color[i] = new_color;
        }
    }
    //Loop over colors and set their output
    //Also note: shift output to keep from overloading LEDs
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(m_pin); i++) {
        //ASSERT(m_color[i] >= 0 && m_color[i] <= 0xFF, "Bad color supplied");
        analogWrite(m_pin[i], m_color[i] >> PWM_SHIFT);
    }
    //Now update the pointer
    if (m_count >= 1000) {
        //Jump to the next waypoint
        memcpy(m_color, POINTS[next], sizeof(m_color));
        m_index = next;
        m_count = 0;
    }
}
