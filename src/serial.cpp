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
    m_cmd_index(0),
    m_cmd_state(false)
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
 * Run the serial pass-through and deframer
 */
void SerialPass::run(uint32_t wait) {
    uint32_t ending = wait + millis();
    //Loop for the time reading and writing
    while (millis() < ending) {
        int character = m_in.read();
        //Handle operations in normal mode (sending matrix data)
        if (!m_cmd_state) {
            //Read a start character, switch to command mode
            if (static_cast<char>(character) == START_CMD) {
                m_cmd_state = true;
                m_cmd_index = 0;
            }
            //Otherwise pass the data through
            else if (character != -1) {
                m_out.write(static_cast<uint8_t>(character));
            }
        }
        //Command mode, read data and store for parsing
        else {
            //Termination of command mode, parse stored data
            if (static_cast<char>(character) == END_CMD) {
                m_cmd_state = false;
                //TODO: parse command here
            }
            //Store valid data
            else if (character != -1) {
                m_cmd[m_cmd_index] = static_cast<uint8_t>(character);
                m_cmd_index++;
            }
        }
        //Pass-through the returned UART message
        character = m_out.read();
        if (character != -1) {
            m_in.write(static_cast<uint8_t>(character));
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
