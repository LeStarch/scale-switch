/*
 * runner.cpp:
 *
 * Runner code implementations.
 *
 *  Created on: Nov 11, 2018
 *      Author: lestarch
 */
#include <Arduino.h>
#include "runner.hpp"
//Concrete definitions, forcing rollover
uint32_t Runner::s_last = 0xFFFFFFFF;
uint32_t Runner::s_current = 0;
unsigned int Runner::s_count = 0;
SerialPass* Runner::s_sleeper = NULL;
Runner* Runner::s_runners[MAX_RUNNERS];
/**
 * Update the clock counter interval and last.
 */
void Runner::update_count() {
    s_last = s_current;
    s_current += RATE_GROUP_PERIOD;

}
/**
 * Check if given interval cbounds was crossed.
 */
bool Runner::interval_check(unsigned int interval) {
    return (s_last/interval) != (s_current/interval);
}
/**
 * Implementation of pass register
 */
void Runner::register_sleeper(SerialPass* pass) {
    s_sleeper = pass;
}
/**
 * Setup all runners based on an array.
 * Note: this will undo the array
 */
void Runner::register_runners(Runner** runners, unsigned int count) {
    //Loop through all runners and set them up
    for (unsigned int i = 0; i < count && s_count < MAX_RUNNERS; i++) {
        //On setup error, don't register; don't update count
        if (!runners[i]->setup()) {
            REPORT_ERROR("Runner setup error");
        }
        //On success, register the runner and up the count
        else {
            s_runners[s_count] = runners[i];
            s_count++;
        }
    }
}
/**
 * Run a cycle and then wait for the next cycle
 * Note: this will undo the array
 */
void Runner::cycle() {
    //Catch startup time
    uint32_t last = millis();
    update_count();
    //Loop through all runners and cycle them
    for (unsigned int i = 0; i < s_count && i < MAX_RUNNERS; i++) {
        s_runners[i]->run();
        //Sleep partially to avoid long wait
        (void) Runner::sleep((i + 1) * RATE_GROUP_PERIOD/s_count, last);
    }
    //Sleep to final end and report a rate group cycle overflow here
    int32_t slip = Runner::sleep(RATE_GROUP_PERIOD, last);
    if (slip > 0) {
        char overflow[MAX_STR_LEN]; //Could optimize by copying
        snprintf(overflow, MAX_STR_LEN, "Slip of %ldms", slip);
        REPORT_ERROR(overflow);
    }
}
/**
 * Sleep duration implementation
 */
int32_t Runner::sleep(uint32_t last, uint32_t duration) {
    //Wait for the next cycle, if needed
    int32_t wait = ((int32_t)(last + duration) - (int32_t)millis());
    //Sleep using sleeper, or delay if no sleeper defined
    if (wait > 0 && s_sleeper != NULL) {
        s_sleeper->run(wait);
        return 0;
    } else if (wait > 0) {
        delay(wait);
        return 0;
    }
    return wait;
}
