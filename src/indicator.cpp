/*
 * indicator.cpp:
 *
 * Used to hold the basic implementations of functions of the indicator.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include <string.h>
#include <Arduino.h>
#include "indicator.hpp"
#include "version.hpp"

//Concrete definitions for shared static members
char Indicator::s_error_file[MAX_STR_LEN];
char Indicator::s_error_message[MAX_STR_LEN];
int Indicator::s_error_line = -1;
bool Indicator::s_error_state = false;
unsigned int Indicator::s_msg_pointer = 0;
//!< Static, shared key storage
char Indicator::s_key_store[MAX_MSG_COUNT][MAX_KEY_LEN + 1];
//!< Static, shared message storage
char Indicator::s_msg_store[MAX_MSG_COUNT][MAX_STR_LEN + 1];

/**
 * Constuctor initializes the member variables of the class.
 */
Indicator::Indicator() : Runner()
{
    //Initialize all member arrays
    for (int i = 0; i < MAX_BUTTON; i++) {
        m_pressed[i] = false;
    }
    for (int i = 0; i < MAX_SERIAL; i++) {
        m_writing[i] = 0;
    }
    memset(s_error_file, 0, MAX_STR_LEN);
    memset(s_error_message, 0, MAX_STR_LEN);
    memset(Indicator::s_key_store, 0, sizeof(Indicator::s_key_store));
    memset(Indicator::s_msg_store, 0, sizeof(Indicator::s_msg_store));
    strncpy(Indicator::s_key_store[MAX_MSG_COUNT - 1], "FIRM", MAX_KEY_LEN);
    strncpy(Indicator::s_msg_store[MAX_MSG_COUNT - 1],  VERSION, MAX_STR_LEN);
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
 * The default handler for incoming messages. This will store the last N
 * messages received, and discard the rest.
 */
void Indicator::message(const char* key, const char* msg) {
    strncpy(Indicator::s_key_store[Indicator::s_msg_pointer], key, MAX_KEY_LEN);
    strncpy(Indicator::s_msg_store[Indicator::s_msg_pointer], msg, MAX_STR_LEN);
    Indicator::s_key_store[Indicator::s_msg_pointer][MAX_KEY_LEN] = '\0';
    Indicator::s_msg_store[Indicator::s_msg_pointer][MAX_STR_LEN] = '\0';

    s_msg_pointer = (s_msg_pointer + 1) % (MAX_MSG_COUNT - 1); // Last slot is for firmware id
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
    if (!s_error_state) {
        s_error_state = true;
        //Do not error in error, so check for null, don't ASSERT not-null
       if ((file != NULL) && (strncmp(s_error_file, "", MAX_STR_LEN) == 0)) {
            //Use strncpy to avoid buffer overflows
            strncpy(s_error_file, file, MAX_STR_LEN);
        }
        if ((msg != NULL) &&
          (strncmp(s_error_message, "", MAX_STR_LEN) == 0)) {
            strncpy(s_error_message, msg, MAX_STR_LEN);
        }
        s_error_line = line;
    }
}
//Static memory used, no destructor specialization needed
Indicator::~Indicator() {}

