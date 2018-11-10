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

class Runner {
    public:
        /**
         * Runs a single update. Will be called on a 1ms interval. This means
         * that no long-running task should be done, but rather work should be
         * broken up into 1/Nms steps. Default implementation: do no work.
         */
        virtual void run() {};
        //Virtual destructor required, but no work needed.
        virtual ~Runner() {};
};




#endif /* SRC_RUNNER_HPP_ */
