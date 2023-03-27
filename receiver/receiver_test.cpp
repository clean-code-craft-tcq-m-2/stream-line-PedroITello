#include "TestTools/catch.hpp"
#include "receiver.h"
#include "messageParser.h"
#include <memory>

using namespace std;

TEST_CASE("Message Parser validation"){
    SECTION("Ensure propper pair separation"){
        string testMessage = "{'coolingType': 'HI_ACTIVE_COOLING',"\
                             "'temperatureInc': 36, 'lowerLimit': 0,"\
                             "'upperLimit': 45}";
        const int totalPairs=4;
        messageParser mp;
        mp.mapMessage(testMessage);
        REQUIRE(totalPairs==mp.sample.size());
    }
}

TEST_CASE("Tracked Parameter validation"){
    trackedParameter tp("testParam");
    SECTION("Default values check"){
        REQUIRE(0==tp.parameterLabel.compare("testParam"));
        REQUIRE(0==tp.getMaxValue().compare(notInitPrompt));
        REQUIRE(0==tp.getMinValue().compare(notInitPrompt));
        REQUIRE(0==tp.getAvgValue().compare(notInitPrompt));
    }
    SECTION("First value"){
        tp.updateParameter(30);
        REQUIRE(0==tp.getMaxValue().compare("30"));
        REQUIRE(0==tp.getMinValue().compare("30"));
        REQUIRE(0==tp.getAvgValue().compare(notInitPrompt));
    }
    SECTION("Average Buffer fullfilled"){
        tp.updateParameter(10);
        tp.updateParameter(20);
        tp.updateParameter(30);
        tp.updateParameter(40);
        tp.updateParameter(50);
        REQUIRE(0==tp.getMaxValue().compare("50"));
        REQUIRE(0==tp.getMinValue().compare("10"));
        REQUIRE(0==tp.getAvgValue().compare("30"));  // (10 + 20 + ... + 50)/5
    
        // --- Overflowing populated avg buffer ---
        for(char i = 0; i<MOVINGBASE; ++i){
            tp.updateParameter(15);
        }
        REQUIRE(0==tp.getAvgValue().compare("15"));
    }
}

TEST_CASE("Sensor Handler + Sensor abstraction validation"){
    shared_ptr<messageParser> mp = make_shared<messageParser>();
    sensorHandler sensorHandlerTest(mp);
    
    SECTION("First message (instance of blank sensor)"){
        string testMessage1 = "{'coolingType': 'testSensor1',"\
                                "'temperatureInc': 36, 'lowerLimit': 0,"\
                                "'upperLimit': 45}";
        mp->mapMessage(testMessage1);
        string outputBufferTest = sensorHandlerTest.handleSample();
        REQUIRE(outputBufferTest.find("testSensor1")!=string::npos);
        REQUIRE(outputBufferTest.find("temperatureInc")!=string::npos);
        REQUIRE(outputBufferTest.find(" - MinValue: 36")!=string::npos);
        REQUIRE(outputBufferTest.find(" - MaxValue: 36")!=string::npos);
        REQUIRE(outputBufferTest.find(" - AvgValue: " + \
                                    notInitPrompt)!=string::npos);
    }
}