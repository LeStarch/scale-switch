/*
 * oled.cpp:
 *
 * Implementation of the OLED class functions.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#include <string.h>
#include "oled.hpp"
/**
 * Constructor sets up the default values in m_ip and m_name
 */
OLED::OLED() : Indicator(),
    m_display(SSD1306_LCDWIDTH, SSD1306_LCDHEIGHT),
    m_index(0),
    m_update_count(0),
    m_updated(true),
    m_first_error(true)
{}
/**
 * Sets up the OLED screen by calling being for the OLED driver.
 */
bool OLED::setup() {
    bool disp;
    if ((disp = m_display.begin(SSD1306_SWITCHCAPVCC, 0x3C))) {
        //Splash screen. OH YEAH!
        m_display.clearDisplay();
        m_display.display();
        //Global setups for later
        m_display.setTextSize(2);
        m_display.setTextColor(WHITE);
    }
    return true;
}
/**
 * Respond to a button press
 */
void OLED::button_pressed(ButtonType button) {
    if (button == BUTTON_DISPLAY) {
        m_index = (m_index + 1) % MAX_MSG_COUNT;
    }
    m_updated = true;
}
/**
 * Implementation of the run function. Remember: all work must be done in
 * snapshots that occur 1/Nms. This means *no* long-running work.
 */
void OLED::run() {
    m_updated = m_updated || m_update_count == 0;
    m_update_count = (m_update_count + 1) % 20;
    //No updates, don't waste time
    if (!m_updated && !(m_first_error && s_error_state)) {
        return;
    }
    //Prepare display for future work
    m_display.clearDisplay();
    m_display.setCursor(0,0);
    //Handle errors
    if (s_error_state) {
        m_first_error = false;
        m_display.println(s_error_message);
        //m_display.setCursor(0,16);
        m_display.print(s_error_file);
        m_display.print(":");
        m_display.print(s_error_line);
    }
    else {
        m_updated = false;
        m_display.print(Indicator::s_key_store[m_index]);
        m_display.println(":");
        m_display.setTextSize(1);
        m_display.println(Indicator::s_msg_store[m_index]);
        m_display.setTextSize(2);
    }
    //Display the data in the display buffer
    m_display.display();
}
