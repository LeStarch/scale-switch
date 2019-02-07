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
//Command message delimiters
#define START_CMD '<'
#define END_CMD '>'
//Matrix message delimiters
#define START_MAT 'M'
#define END_MAT 'N'
#define COMM_MAT 'T'
//Other definitions
#define IDLE_TIMEOUT_MS 200
#define MAX_MATRIX 2
#define MATRIX_TEMPLATE_SIZE 12
//Template to fill with characters
#define MATRIX_TEMPLATE_STR "MT00SW0x01NT"
#define MATRIX_TEMPLATE_FILL_INDEX 7
class SerialPass {
    public:
        /**
         * SerialMode:
         *
         * Mode for the scale-switch serial handler. This sets if it is idle,
         * passing-through, or commanding locally.
         */
        enum SerialMode {
            MODE_IDLE,   //!< Idle mode, nothing going on
            MODE_PASS,   //!< Pass-through mode, sending command to Matrix
            MODE_COMMAND,//!< Command mode, processing command locally
            MAX_SERIAL_MODE //!< For Enum bounds checking
        };
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
         * Sends the toggle message. This should be called at the beginning of the
         * "waste time" loop.
         */
        void send_manual();
        /**
         * Queues the next toggle message
         */
        void manual();
    private:
        //!< Hardware serial input (from host)
        HardwareSerial& m_in;
        //!< Hardware serial output to Matrix
        SoftwareSerial& m_out;
        //!< Time of forced idle reset
        uint32_t m_reset_time;
        //!< Current mode of the serial port
        SerialMode m_mode;
        //!< Index into m_cmd
        unsigned int m_index;
        //!< Command and pass-through data
        uint8_t m_buffer[MAX_STR_LEN];
        //!< Non-constant storage
        char m_matrix[MATRIX_TEMPLATE_SIZE];
        //!< Current matrix output mapping
        char m_current;
        //!< Flag for sending out matrix command
        bool m_switch;
};
#endif /* SRC_SERIAL_HPP_ */
