/*
 * types.hpp:
 *
 * Defines the types used by the scale-switch. This means various enumerations
 * and other types needed as part of the system. As extra help this class
 * includes standard int types for use in the rest of the system.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */

#ifndef SRC_TYPES_HPP_
#define SRC_TYPES_HPP_
#include <stdint.h>

//!< Maximum length of string
#define MAX_STRING_LEN 80
//!< Rate-group period. Run functions called every N-ms.
//!< Currently set to one, as serial sends roughly 1char/ms
#define RATE_GROUP_PERIOD 10
//!< Seconds per millisecond
#define MS_PER_SECOND 1000
//!< Maximum gpio pin count
#define GPIO_PIN_COUNT 32
//!< Get array elements
#define NUM_ARRAY_ELEMENTS(array) sizeof(array)/sizeof(array[0])

//!< Sized 32bit floating point
typedef float float32;
//!< Sized 64bit floatin point
typedef double float64;

/**
 * ButtonType:
 *
 * Defines the button types defined by the system. These are the external
 * hardware buttons used to control this system. Used to communicate which
 * button for button events.
 */
enum ButtonType {
    BUTTON_PODIUM, //!< Button supplied to speaker taped to podium
    BUTTON_DISPLAY, //!< Button on the display circuit
    MAX_BUTTON //!< Helper for bounds checking
};
/**
 * SerialType:
 *
 * Defines the serial interfaces used in the system. Used to communicate which
 * serial port when handling serial port events.
 */
enum SerialType {
    SERIAL_USB, //!< Serial USB connected to the host box
    SERIAL_MATRIX, //!< Serial connected to the matrix switch
    MAX_SERIAL //!< Helper for bounds checking
};
/**
 * BootStatus:
 *
 * Current state of the host computers boot. Used to communicate where in the
 * boot process the host computer is at.
 */
enum BootStatus {
    BOOT_ON, //!< Host system is on
    BOOT_OFF, //!< Host system is off
    BOOT_STARTUP, //!< Host system is starting up
    MAX_BOOT //!< Helper for bounds checking
};
/**
 * Handles assertions for the system by asserting, reporting an error and
 * explicitly returning from the "current" function to prevent downstream
 * problems.
 */
#define ASSERT(cond,message) if(cond){error(__FILE__, __LINE__, message);return;}
/**
 * Reports an error to the system. Will be handled by below error function.
 */
#define REPORT_ERROR(message) error(__FILE__, __LINE__, message)

/**
 * Function for handling errors. Will be called from the above error handling
 * function.
 */
void error(const char* file, const int line, const char* message);

#endif /* SRC_TYPES_HPP_ */
