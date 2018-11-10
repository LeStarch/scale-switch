/*
 * indicator.hpp:
 *
 * Indicator is the base class of all the indicators in the system. It takes
 * several inputs and allows a possible indication of status to appear. The
 * indicator handles the following types of inputs:
 *
 * 1. Button Pressed: called to when a button is pressed. Passed a button
 *    type indication to handle different types of buttons.
 * 2. Boot Status: called each time the boot status of the box is changed.
 *    Passed the current boot status.
 * 3. Write Byte: called when a byte is written to a serial port.
 *    Passed serial port type indicating interface written to.
 *
 * Implementation Note: this is a rate-driven component. All updates to
 * indicators that require timed-responses should be carried out in the
 * run function. This function will be called 1 time every millisecond.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#ifndef SRC_INDICATOR_HPP_
#define SRC_INDICATOR_HPP_
#include "types.hpp"
#include "runner.hpp"

class Indicator : public Runner
{
    public:
        /**
         * Constructs the indicator.
         */
        Indicator();
        /**
         * Called to indicate that a button was pressed.
         * Default implementation: set m_pressed for button.
         * \param ButtonType button: button pressed
         */
        virtual void button_pressed(ButtonType button);

        /**
         * Called to indicate that a serial port is being written to.
         * Default implementation: set m_writing for given port.
         * \param SerialType serial: serial port written to
         */
        virtual void serial_written(SerialType serial);

        /**
         * Called to update the current state of the host boot.
         * Default implementation: set the m_boot state variable.
         * \param BootStatus status: new boot state
         */
        virtual void boot_update(BootStatus status);

        /**
         * Indicates an error happened. Allows this indicator to display the
         * error and relevant information. Default: store the error contents
         * and set the m_error boolean to indicate an error state. It is not
         * recommended to clear this error, as errors of this kind are
         * *usually* terminal.
         * \param const char* file: file where error occured
         * \param const int line: line in file where error occured
         * \param const char* msg: user provided message
         */
        virtual void error(const char* file, const int line, const char* msg);
        /**
         * Virtual destructor needed for a virtual class
         */
        virtual ~Indicator();
    protected:
        //!< Current BootStatus
        BootStatus m_boot;
        //!< Line number of current error
        int m_error_line;
        //!< State indicating error condition encountered
        bool m_error_state;
        //!< Contains the "pressed" state of buttons. Set to false to clear.
        bool m_pressed[MAX_BUTTON];
        //!< Contains the writing state of serial ports. Set to false to clear.
        bool m_writing[MAX_SERIAL];
        //!< Holds current error file name
        char m_error_file[MAX_STRING_LEN];
        //!< Holds current error message
        char m_error_message[MAX_STRING_LEN];
};
#endif /* SRC_INDICATOR_HPP_ */
