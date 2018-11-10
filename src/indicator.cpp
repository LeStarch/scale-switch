/*
 * indicator.cpp:
 *
 * Used to hold the basic implementations of functions of the indicator.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include <string.h>
#include "indicator.hpp"

/**
 * Constuctor initializes the member variables of the class.
 */
Indicator::Indicator() : Runner(),
    m_boot(BOOT_OFF),
    m_error_line(-1),
    m_error_state(false)
{
    //Initialize all member arrays
    for (int i = 0; i < MAX_BUTTON; i++) {
        m_pressed[i] = false;
    }
    for (int i = 0; i < MAX_SERIAL; i++) {
        m_writing[i] = 0;
    }
    memset(m_error_file, 0, MAX_STRING_LEN);
    memset(m_error_message, 0, MAX_STRING_LEN);
}

/**
 * The default indicator action on the button pressed event is to set that
 * the button was pressed. A subclass can then respond to that set-point and
 * clear the indicator.
 */
void Indicator::button_pressed(ButtonType button) {
    //Bounds checking, or return
    ASSERT(button < MAX_BUTTON, "Button out of range");
    m_pressed[button] = true;
}
/**
 * The default indicator action on the serial port write event is to set that
 * the serial port wrote. A subclass can then respond to that set-point and
 * clear the indicator.
 */
void Indicator::serial_written(SerialType serial) {
    //Bounds checking, or return
    ASSERT(serial < MAX_SERIAL, "Serial out of range");
    m_writing[serial] = true;
}
/**
 * The default indicator action on the boot status update is to set the
 * current boot state.
 */
void Indicator::boot_update(BootStatus status) {
    //Bounds checking, or return
    ASSERT(status < MAX_BOOT, "Boot status out of range");
    m_writing[status] = true;
}
/**
 * The default indicator action for errors is to set the error state and set
 * the error variables. It is not recommended that the m_error_state variable
 * be cleared, as it ensures the first error is captured *and* an error is
 * usually fatal so an attempt to run past indication is discouraged.
 */
void Indicator::error(const char* file, const int line, const char* msg) {
    //Only set an error if not in error state. Otherwise we risk clearning
    //the root-cause of any error chains
    if (!m_error_state) {
        m_error_state = true;
        //Do not error in error, so check for null, don't ASSERT not-null
        if (file != NULL) {
            //Use strncpy to avoid buffer overflows
            strncpy(m_error_file, file, MAX_STRING_LEN);
        }
        if (msg != NULL) {
            strncpy(m_error_message, msg, MAX_STRING_LEN);
        }
        m_error_line = line;
    }
}
//Static memory used, no destructor specialization needed
Indicator::~Indicator() {}

