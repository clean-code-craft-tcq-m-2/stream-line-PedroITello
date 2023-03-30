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
    inputBuffer = "";
    getline(cin >> ws, inputBuffer);
    msgParser->mapMessage(inputBuffer);
    return true;
}

bool receiver::processSample(){
    if(inputBuffer.length() != 0){
        string outputBuffer = sHandler->handleSample();
        cout<<outputBuffer<<endl;
    }
    return true;
}

void receiver::timerThread(int pollingSeconds){
    while(pollingFlag){
        this_thread::sleep_for(chrono::seconds(pollingSeconds));
        timerCounter++;
        if(TIMEOUTTRIALS<=timerCounter){
            exit(0);
        }
    }
}

void receiver::run(){
    thread th(&timerThread, this, ref(POLLINGSECONDS));
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