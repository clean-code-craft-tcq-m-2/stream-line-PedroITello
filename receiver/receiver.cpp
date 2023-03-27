#include <iostream>
#include <exception>
#include <thread>
#include "receiver.h"

#ifdef UTEST
#define CATCH_CONFIG_RUNNER
#include "TestTools/catch.hpp"
#endif // UTEST

using namespace std;

bool receiver::getSample(){
    getline(std::cin, inputBuffer);
    msgParser->mapMessage(inputBuffer);
    return true;
}

bool receiver::processSample(){
    string outputBuffer = sHandler->handleSample();
    cout<<outputBuffer<<endl;
    return true;
}

void receiver::timerThread(int pollingSeconds){
    while(pollingFlag){
        std::this_thread::sleep_for(std::chrono::seconds(pollingSeconds));
        timerCounter++;
        if(TIMEOUTTRIALS<=timerCounter){
            std::exit(0);
        }
    }
}

void receiver::run(){
    std::thread th(&timerThread, this, std::ref(POLLINGSECONDS));
    th.detach();

    while(pollingFlag){
        getSample();
        processSample();
    }

    th.join();
}

int main(){
    #ifdef UTEST
    Catch::Session().run();
    #endif // UTEST

    shared_ptr<messageParser> mp = make_shared<messageParser>();
    unique_ptr<sensorHandler> sHandler = make_unique<sensorHandler>(mp);
    receiver rec(mp,std::move(sHandler));
    rec.run();
}