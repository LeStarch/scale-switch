/*
 * serial.cpp:
 *
 * Serial pass through implementations.
 *
 *  Created on: Nov 11, 2018
 *      Author: lestarch
 */
#include "serial.hpp"
#include "indicator.hpp"
#include <string.h>
/**
 * Construction done via references, to ensure saftey and memory.
 */
SerialPass::SerialPass(HardwareSerial& in, SoftwareSerial& out) :
    m_in(in),
    m_out(out),
    m_cmd_index(0),
    m_response_count(0),
    m_state(IDLE),
    m_interrupt(false)
{
    memcpy(m_matrix, MATRIX_TEMPLATE_STR, sizeof(m_matrix));
}
/**
 * Begin the serial device
 */
void SerialPass::begin(int baud) {
    m_in.begin(baud);
    m_out.begin(baud);
}
/**
 * Interrupted, toggle
 */
void SerialPass::interrupt() {
    m_interrupt = true;
}
/**
 * Run the serial pass-through and deframer
 */
void SerialPass::run(uint32_t wait) {
    uint32_t ending = wait + millis();
    //Loop for the time reading and writing
    while (millis() < ending) {
        // Handle podium presses before passthrough
        if (m_interrupt && m_state == IDLE) {
            toggle();
            m_interrupt = false;
        }
        //While waiting for a response, don't read character
        int character = -1;
        if (m_state != RESP) {
            character = m_in.read();
        }
        //Handle operations in normal mode (sending matrix data)
        if (m_state == IDLE) {
            //Read a start character, switch to command mode
            if (static_cast<char>(character) == START_CMD) {
                m_state = COMMAND;
                m_cmd_index = 0;
            }
            //Handle 'M' characters the other possible token
            else if (static_cast<char>(character) == 'M') {
                m_state = MSG1;
                m_out.write(static_cast<uint8_t>(character));
            }
        }
        // Messaging states
        else if (m_state == MSG1 || m_state == MSG2) {
            //Handle 'T' character states, second one is done
            if (static_cast<char>(character) == 'T' && m_state == MSG1) {
                m_state = MSG2;
            } else if (static_cast<char>(character) == 'R' && m_state == MSG2) {
                //Expected a response if this is a read
                m_response_count = RESPONSE_SIZE;
            } else if (static_cast<char>(character) == 'T' && m_state == MSG2) {
                m_state = RESP;
            }
            m_out.write(static_cast<uint8_t>(character));             
        }
        //Command mode, read data and store for parsing
        else if (m_state == COMMAND) {
            //Termination of command mode, parse stored data
            if (static_cast<char>(character) == END_CMD) {
                m_state = IDLE;
                Indicator::message(m_cmd, m_cmd + MAX_KEY_LEN);
            }
            //Store valid data
            else if (character != -1 && m_cmd_index < (MAX_STR_LEN + MAX_KEY_LEN)) {
                m_cmd[m_cmd_index] = static_cast<uint8_t>(character);
                m_cmd_index++;
            }
        }
        // Handle response counting back
        else if (m_state == RESP) {
            if (m_response_count == 0) {
                m_state = IDLE;
            }
        }
        else {
            ASSERT(false, "Invalid serial state");
        }
        //Pass-through the returned UART message
        character = m_out.read();
        if (character != -1) {
            m_in.write(static_cast<uint8_t>(character));
            if (m_response_count > 0) {
                m_response_count = m_response_count - 1;
            }
        }
    }
}
/**
 * Toggle the devices.
 */
void SerialPass :: toggle() {
        //Singleton pointer to the active device
        static char active = 0;
        m_matrix[7] = active + '1';
        m_out.write(m_matrix, sizeof(m_matrix));
        active = (active + 1) % MAX_MATRIX;
}
