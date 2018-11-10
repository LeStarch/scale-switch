/*
 * oled.cpp:
 *
 * Implementation of the OLED class functions.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include <string.h>
#include "oled.hpp"

/**
 * Constructor sets up the default values in m_ip and m_name
 */
OLED::OLED() {
    strncpy(m_ip, "<unknown ip>", MAX_STRING_LEN);
    strncpy(m_name, "<unknown name>", MAX_STRING_LEN);
}
/**
 * Sets the IP address in the member variable for use later.
 */
void OLED::set_ip(const char* ip) {
    strncpy(m_ip, ip, MAX_STRING_LEN);
}
/**
 * Sets the name in the member variable for use later.
 */
void OLED::set_name(const char* name) {
    strncpy(m_name, name, MAX_STRING_LEN);
}
/**
 * Implementation of the run function. Remember: all work must be done in
 * snapshots that occur 1/Nms. This means *no* long-running work.
 */
void OLED::run() {
    //Error case: print the message, line and file
    if (m_error_state) {

    }
    //TODO: implement non-error case here
    else {

    }
}


