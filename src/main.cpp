#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>

//Software serial is the serial device that the computer comes in on
HardwareSerial& input = Serial;
SoftwareSerial output(3, 4);
//Pins used for leds, status, and interrupts. Interrupts must be 2 or 3
const byte BOARD_LED = 13;
const byte INTERRUPT_PIN = 2;

//Interval required for debouncing the interrupt pin
const int DEBOUNCE_INTERVAL_MS=100;

//Interrupt signal pending code
bool INTERRUPT_SIGNAL_PENDING=false;

//Function prototype for isr
void isr();

/**
 * Setup:
 *
 * Run one time function used to setup the serial device and 
 * interrupts based on the button push.
 */
void setup() {
    pinMode(BOARD_LED, OUTPUT);
    input.begin(9600);
    //output.begin(9600);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), isr, RISING); 
    //Wait for serial for non-leonardo boards
    delay(250);
}

void loop() {
	static unsigned int length = 0;
    static int led_state = LOW;
	static char message[1024];
    //Check if the interrupt is pending
    if (INTERRUPT_SIGNAL_PENDING && input.available() == 0) {
        led_state = !led_state;
        INTERRUPT_SIGNAL_PENDING = 0;
        strncpy(message, "SOME", 5);
        length = 4;
    } else if (input.available() > 0) {
    	message[0] = input.read();
    	length = 1;
    }
    digitalWrite(BOARD_LED, led_state);
    output.write(message, length);
}

/**
 * isr:
 *
 * Handles (and debounces) the incoming interrupt. 
 */
void isr() {
    static unsigned int last = 0;
    unsigned int current = millis();
    //Brake out early when debouncing
    //Note: will fail safely on overflow
    if ((current > last) && (current < (last + DEBOUNCE_INTERVAL_MS))) {
        return;
    }
    last = current;
    INTERRUPT_SIGNAL_PENDING = true;
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
