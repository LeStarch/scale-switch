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
    m_updated(true),
    m_first_error(true)
{
    memset(m_ip, 0, IP_ADDRESS_LEN);
    strncpy(m_name, "<unknown name>", MAX_STR_LEN);
}
/**
 * Sets the IP address in the member variable for use later.
 */
void OLED::set_ip(const uint8_t* ip) {
    memcpy(m_ip, ip, IP_ADDRESS_LEN);
    m_updated = true;
}
/**
 * Sets the name in the member variable for use later.
 */
void OLED::set_name(const char* name) {
    strncpy(m_name, name, MAX_STR_LEN);
    m_updated = true;
}
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
        m_display.setTextSize(1);
        m_display.setTextColor(WHITE);
    }
    return true;
}
/**
 * Implementation of the run function. Remember: all work must be done in
 * snapshots that occur 1/Nms. This means *no* long-running work.
 */
void OLED::run() {
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
    //TODO: better than just IP and name
    else {
        m_updated = false;
        for (int i = 0; i < IP_ADDRESS_LEN; i++) {
            //Integer for ith octet
            m_display.print(((int)m_ip[i]));
            //Add a "." after first N-1 octets
            if (i < (IP_ADDRESS_LEN -1)) {
                m_display.print(".");
            }
        }
        //Switch to another line
        m_display.println();
        m_display.println(m_name);
    }
    //Display the data in the display buffer
    m_display.display();
}
