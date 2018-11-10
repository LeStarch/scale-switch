/*
 * oled.hpp:
 *
 * This file powers the ScaleAV OLED screen. It allows the ScaleAV team to
 * control the OLED screen. This screen will take the following inputs above
 * and beyond the basic indicator properties.
 *
 * 1. IP Address: will set the IP address of the machine, once known.
 * 2. Room name: will set the IP address of the box, once known.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */
#ifndef SRC_OLED_HPP_
#define SRC_OLED_HPP_
#include "types.hpp"
#include "indicator.hpp"
class OLED : public Indicator
{
    public:
        /**
         * Constructor, must set initial data for IP and name.
         */
        OLED();
        /**
         * Sets the IP address once it arrives
         * \param const char* ip: ip address to set
         */
        void set_ip(const char* ip);

        /**
         * Sets the name of the device
         * \param const char* name: name to set
         */
        void set_name(const char* name);

        /**
         * Overrides run to provide OLED specific actions
         */
        void run();
    protected:
        //!< IP address string
        char m_ip[MAX_STRING_LEN];
        //!< Name string
        char m_name[MAX_STRING_LEN];
};
#endif /* SRC_OLED_HPP_ */
