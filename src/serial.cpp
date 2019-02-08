/*
 * serial.cpp:
 *
 * Serial pass through implementations.
 *
 *  Created on: Nov 11, 2018
 *      Author: lestarch
 */
#include "serial.hpp"
#include <string.h>
/**
 * Construction done via references, to ensure saftey and memory.
 */
SerialPass::SerialPass(HardwareSerial& in, SoftwareSerial& out) :
    m_in(in),
    m_out(out),
    m_reset_time(0),
    m_mode(SerialPass::MODE_IDLE),
    m_index(0),
	m_current(0),
    m_switch(false)
{
    memcpy(m_matrix, MATRIX_TEMPLATE_STR, sizeof(m_matrix));
    memset(m_buffer, '0', sizeof(m_buffer));
}
/**
 * Begin the serial device
 */
void SerialPass::begin(int baud) {
    m_in.begin(baud);
    m_out.begin(baud);
}
/**
 * Run the serial pass-through and deframer
 */
void SerialPass::run(uint32_t wait) {
    uint32_t ending = wait + millis();
    //Write a queued message out, if pending
    if (m_mode == SerialPass::MODE_IDLE && m_switch) {
        this->send_manual();
    }
    uint32_t now = 0;
    //Loop for the time reading and writing
    while ((now = millis()) < ending) {
        //Pass-through the returned UART message
        int character = m_out.read();
        if (character != -1) {
            m_in.write(static_cast<uint8_t>(character));
        }
        //Handle outgoing (to matrix) character
        character = m_in.read();
        //Check mode timeout, and force-reset
        if (m_mode != SerialPass::MODE_IDLE &&
            now >= m_reset_time) {
            m_mode = SerialPass::MODE_IDLE;
            m_index = 0;
        }
        //Check if the character read was invalid
        if (character == -1) {
            continue;
        }
        //Check for command-mode start character
        else if (static_cast<char>(character) == START_CMD) {
            m_mode = SerialPass::MODE_COMMAND;
            m_index = 0;
            m_reset_time = now + IDLE_TIMEOUT_MS;
        }
        //Break-out of command mode
        else if (m_mode == SerialPass::MODE_COMMAND &&
                 static_cast<char>(character) == END_CMD) {
            m_mode = SerialPass::MODE_IDLE;
            m_index = 0;
            //TODO: send completed command
            //m_recorder.set(/*key*/m_buffer,/*msg*/m_buffer + 4);
        }
        //Check for the second character in pass-mode
        else if (m_mode == SerialPass::MODE_IDLE &&
                 m_buffer[m_index] == START_MAT &&
                 static_cast<char>(character) == COMM_MAT) {
            m_buffer[0] = 'M';
            m_index = 1;
            m_mode = SerialPass::MODE_PASS;
            m_reset_time = now + IDLE_TIMEOUT_MS;
        }
        //Check for end of pass-through
        else if (m_mode == SerialPass::MODE_PASS &&
                 m_buffer[m_index] == END_MAT &&
                 static_cast<char>(character) == COMM_MAT) {
            m_mode = SerialPass::MODE_IDLE;
            m_index = 0;
            //Update current from last message, shifting from ascii to 0 based integer index
            m_current = m_buffer[MATRIX_TEMPLATE_FILL_INDEX] - '1';
        }
        //Steady-state command and pass through seeks through buffer
        else if (m_mode == SerialPass::MODE_COMMAND ||
                 m_mode == SerialPass::MODE_PASS) {
            m_index = m_index + 1;
            m_index = (m_index < sizeof(m_buffer)) ? m_index : sizeof(m_buffer) - 1;
        }
        //Misc-idle characters, write to index 0
        else if (m_mode == SerialPass::MODE_IDLE) {
            m_index = 0;
        }
        ASSERT(m_index < sizeof(m_buffer), "Buffer index out of range.");
        m_buffer[m_index] = static_cast<char>(character);
        //Pass through active character
        if (m_mode != SerialPass::MODE_COMMAND &&
            character != START_CMD) {
            m_out.write(character);
        }
    }
}
/**
 * Sends the toggle message. This should be called at the beginning of the
 * "waste time" loop.
 */
void SerialPass :: send_manual() {
    m_current = (m_current + 1) % MAX_MATRIX;
    //Shifting from 0 based integer index to ascii
    m_matrix[MATRIX_TEMPLATE_FILL_INDEX] = m_current + '1';
    m_out.write(m_matrix, sizeof(m_matrix));
    m_switch = false;
}
/**
 * Toggle the devices.
 */
void SerialPass :: manual() {
    m_switch = true;
}
