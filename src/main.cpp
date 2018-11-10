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

//Software serial is the serial device that the computer comes in on
HardwareSerial& input = Serial;
SoftwareSerial output(3, 4);

//Interval required for debouncing the interrupt pin
const int DEBOUNCE_INTERVAL_MS=3000;

//Interrupt signal pending code
bool INTERRUPT_SIGNAL_PENDING=false;

//Two buttons, one interrupt driven, the other not
Button b_podium(2, DEBOUNCE_INTERVAL_MS, BUTTON_PODIUM, true);
Button b_display(7, 500, BUTTON_DISPLAY, false);

//Indicators: LED13, and RGB
LED13 i_led(13);
RGB i_rgb(9, 10, 11);

//Setup the indicators
Indicator* indicators[] = {&i_led, &i_rgb};

//Setup all runners
Runner* runners[] = {&b_podium, &b_display, &i_led, &i_rgb};

/**
 * What to do when the podium button is pressed.
 */
void podium_press(ButtonType button) {
    REPORT_ERROR("Podium error detected");
}

/**
 * What to do when the display button is pressed.
 */
void display_press(ButtonType button) {
    REPORT_ERROR("Display error detected");
}
/**
 * Define the error handling function, which passes the arguments
 * to all the indicators.
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
    //Button press registers
    b_podium.register_handler(&podium_press);
    b_display.register_handler(&display_press);

    input.begin(9600);
    //Wait for serial for non-leonardo boards
    delay(250);
}
/**
 * Loop calling runners once every N milliseconds
 */
void loop() {
    int last = millis();
    //For every runner, call it
    for (unsigned int i = 0; i < NUM_ARRAY_ELEMENTS(runners); i++) {
        runners[i]->run();
    }
    //Wait for the next cycle
    delay((last + RATE_GROUP_PERIOD) - millis());
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
