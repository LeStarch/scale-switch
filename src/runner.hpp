/*
 * runner.hpp:
 *
 * Runner class which defines one function "run". This run function will be
 * called at a fixed rate: 1/Nms. Thus the implementor can perform updates
 * at 1/ms. Nothing done in the "run" function should ever take a long time to
 * execute, but rather should just update state (quickly). N is set to:
 * RATE_GROUP_PERIOD milliseconds.
 *
 *  Created on: Nov 9, 2018
 *      Author: lestarch
 */

#ifndef SRC_RUNNER_HPP_
#define SRC_RUNNER_HPP_
#include "types.hpp"
#include "serial.hpp"
//!< Rate-group period. Run functions called every N-ms.
//!< Currently set to one, as serial sends roughly 1char/ms
#define RATE_GROUP_PERIOD 100
class Runner {
    public:
        /**
         * Runs a single update. Will be called on a 1ms interval. This means
         * that no long-running task should be done, but rather work should be
         * broken up into 1/Nms steps. Default implementation: do no work.
         */
        virtual void run() {};
        /**
         * Setup this runner. Return false if setup fails. Otherwise retun true.
         */
        virtual bool setup() {return true;}
        /**
         * Register a sleeper to burn time instead of waiting.
         * \param SerialPass* pass: passthrough used to burn time
         */
        static void register_sleeper(SerialPass* pass);
        /**
         * Setup all runners. Loops through the runner array less than
         * count and calls the runner's setup method. On setup failure,
         * the runner is removed from the list in order prevent it from
         * running later.
         * \param Runner* runners[]: runner array to run against
         * \param unsigned int count: runner array size
         */
        static void register_runners(Runner* runners[], unsigned int count);
        /**
         * Setup all runners. Loops through the runner array less than
         * count and calls the runner's setup method. On setup failure,
         * the runner is removed from the list in order prevent it from
         * running later.
         * \param Runner* runners[]: runner array to run against
         * \param unsigned int count: runner array size
         */
        static void setup_all(Runner* runners[], unsigned int count);
        /**
         * Run a cycle of the system.
         */
        static void cycle();
        /**
         * Sleep for the given duration past the given last timestamp
         * \param uint32_t last: last time stamp from millis
         * \param uint32_t duration: duration in millis past last to sleep
         * \return: slip if duration was missed at time of call
         */
        static int32_t sleep(uint32_t last, uint32_t duration);
        /**
         * Updates the static counters s_count and s_last, in order to power
         * the interval check call.
         */
        static void update_count();
        /**
         * Interval check. Checks to see if a clock running at the given interval
         * has ticked in the last cycle.
         * \param unsigned int interval: interval to check
         * \return true if a clock cycle ticked in last interval
         */
        static bool interval_check(unsigned int interval);
        //Virtual destructor required, but no work needed.
        virtual ~Runner() {};
    private:
        //!< Current millisecond clock count
        static uint32_t s_current;
        //!< Current last clock count
        static uint32_t s_last;
        //!< Current runner count
        static unsigned int s_count;
        //!< Sleeper used to burn time
        static SerialPass* s_sleeper;
        //!< Current set of runners
        static Runner* s_runners[MAX_RUNNERS];
};
#endif /* SRC_RUNNER_HPP_ */
