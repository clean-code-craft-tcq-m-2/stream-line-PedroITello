#ifndef _RECEIVER_H_
#define _RECEIVER_H_

#include "messageParser.h"
#include "sensors.h"
#include <memory>
#include <atomic>

static const int TIMEOUTTRIALS  = 3;
static const int POLLINGSECONDS = 1;

class receiver{
    private:
        std::shared_ptr<messageParser> msgParser;
        std::unique_ptr<sensorHandler> sHandler;
        std::string inputBuffer;
        std::atomic<int> timerCounter = 0;
        std::atomic<bool> pollingFlag = true;

        bool getSample();
        bool processSample();
    public:
        receiver(std::shared_ptr<messageParser> param_mp,
                 std::unique_ptr<sensorHandler> param_sh):
            msgParser(param_mp),
            sHandler(std::move(param_sh))
        {};

        void timerThread(int pollingSeconds);
        void run();
};

#endif //_RECEIVER_H_