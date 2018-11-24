/*
 * main.cpp:
 *
 * Main function that establishes both "loop" and "setup" functions.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "types.hpp"
#include "button.hpp"
#include "runner.hpp"
#include "indicator.hpp"
#include "led13.hpp"
#include "rgb.hpp"
#include "oled.hpp"
#include "serial.hpp"

//!< Debounce Interval for HDMI
#define HDMI_DEBOUNCE_INTERVAL_MS 3000
//!< Debounce interval for screen button
#define DISPLAY_DEBOUNCE_INTERVAL_MS 500
//!< Recv pin for soft serial (2 or 3 have interrupts)
#define SOFT_SERIAL_RECV_PIN 3
//!< Send pin for soft serial
#define SOFT_SERIAL_SEND_PIN 6
//!< HDMI button number
#define BUTTON_HDMI_PIN 2
//!< OLED display button
#define BUTTON_DISPLAY_PIN 7
//!< Start-up time for the system
#define STARUP_TIME_MS 5000
//!< Serial baud rate for in and out
#define SERIAL_BAUD_RATE 9600

SoftwareSerial soft(SOFT_SERIAL_RECV_PIN, SOFT_SERIAL_SEND_PIN);
SerialPass pass(Serial, soft);

//Two buttons, one interrupt driven, the other not
Button b_podium(BUTTON_HDMI_PIN, HDMI_DEBOUNCE_INTERVAL_MS,
        BUTTON_PODIUM, true);
Button b_display(BUTTON_DISPLAY_PIN, DISPLAY_DEBOUNCE_INTERVAL_MS,
        BUTTON_DISPLAY, false);

//Indicators: LED13, RGB, and OLED screen
LED13 i_led(13);
OLED i_oled;
RGB i_rgb(9, 10, 11);

//Setup the indicator array to run
Indicator* indicators[] = {&i_oled, &i_rgb, &i_led};

//Setup non-indicator runners
Button* buttons[] = {&b_podium, &b_display};
/**
 * What to do when the podium button is pressed.
 */
void podium_press(ButtonType button) {
    //Error all the indicators
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(indicators); i++) {
        indicators[i]->button_pressed(button);
    }
    REPORT_ERROR("Podium error");
}
/**
 * What to do when the display button is pressed.
 */
void display_press(ButtonType button) {
    //Error all the indicators
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(indicators); i++) {
        indicators[i]->button_pressed(button);
    }
    REPORT_ERROR("ERROR");
}
/**
 * Define the error handling function, which passes the arguments
 * to all the indicators.
 * Note: this is declared in "types.hpp" for use system wide
 */
void error(const char* file, const int line, const char* message) {
    //Error all the indicators
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(indicators); i++) {
        indicators[i]->error(file, line, message);
    }
}
/**
 * Setup:
 *
 * Run one time function used to setup the serial device and 
 * interrupts based on the button push.
 */
void setup() {
    //Setup button handle registrars
    b_podium.register_handler(&podium_press);
    b_display.register_handler(&display_press);
    //Launch the serial port code
    pass.begin(SERIAL_BAUD_RATE);
    //Register all runners
    Runner::register_sleeper(&pass);
    Runner::register_runners(reinterpret_cast<Runner**>(buttons), NUM_ARRAY_ELEMENTS(buttons));
    Runner::register_runners(reinterpret_cast<Runner**>(indicators), NUM_ARRAY_ELEMENTS(indicators));
    //Allow serial port to start-up, and system to become quiescent
    //before starting up standard rate group drivers
    delay(STARUP_TIME_MS);
}
/**
 * Loop calling runners once every N milliseconds
 */
void loop() {
    Runner::cycle();
}

/**
 * Main function:
 *
 * Some potential boards do not use the arduino compiler so this
 * mimics what the arduino compiler does.
 */
/*int main(int argc, char** argv) {
    setup();
    while(1) {
        loop();
    }
}*/
