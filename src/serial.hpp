/*
 * serial.hpp:
 *
 * This sets up the Serial pass-through and deframes any system-based messages
 * to interpret them locally.
 *
 *  Created on: Nov 11, 2018
 *      Author: lestarch
 */
#ifndef SRC_SERIAL_HPP_
#define SRC_SERIAL_HPP_
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "types.hpp"
#define START_CMD '<'
#define END_CMD '>'
#define MAX_MATRIX 2
#define MATRIX_TEMPLATE_SIZE 12
//Template to fill with characters
#define MATRIX_TEMPLATE_STR "MT00SW0x02NT"

enum SerialState {
    IDLE,    // Nothing going on
    COMMAND, // Processing a command
    MSG1,    // First part of message (before first T)
    MSG2     // Second part of message (before closing T)
};

class SerialPass {
    public:
        /**
         * Serial constructor taking in and out types.
         */
        SerialPass(HardwareSerial& in, SoftwareSerial& out);
        /**
         * Begin the serial port
         * \param int baud: baud rate
         */
        void begin(int baud);
        /**
         * Runs the serial passthough and deframer for the specified number of
         * milli-seconds. Once the time expires, control will return to the
         * caller.
         * \param uint32_t wait: number of milliseconds to run for
         */
        void run(uint32_t wait);
        /** 
	 * Interrupt line
	 */
	void interrupt();
        /**
         * Iterate through the available device.
         */
        void toggle();
    private:
        //!< Hardware serial input (from host)
        HardwareSerial& m_in;
        //!< Hardware serial output to Matrix
        SoftwareSerial& m_out;
        //!< Index into m_cmd
        unsigned int m_cmd_index;
        //!< Serial state to process commands, or others
        SerialState m_state;
        //!< Command data
        uint8_t m_cmd[MAX_STR_LEN];
        //!< Non-constant storage
        char m_matrix[MATRIX_TEMPLATE_SIZE];
        //!< Interrupted
        bool m_interrupt;
};
#endif /* SRC_SERIAL_HPP_ */
