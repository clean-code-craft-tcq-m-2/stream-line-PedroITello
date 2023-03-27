#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <string>
#include <list>
#include <memory>
#include "messageParser.h"
#include "sensorConstants.h"

class trackedParameter{
    private:
        int maximumValue = NOTINITIALIZED;
        int minimumValue = NOTINITIALIZED;
        int movingAverage = NOTINITIALIZED;
        unsigned char avgSampleCount = 0;
        unsigned char avgWriteIndex = 0;
        int averageBuffer[MOVINGBASE];

        bool isMaxValue(int value);
        bool isMinValue(int value);
        bool computeMovingAvg(int value);
        void updateAvgBuffer(int value);
        void calculateAverage();

    public:
        std::string parameterLabel;

        trackedParameter(std::string paramLabel):
            parameterLabel(paramLabel)
        {};
        bool updateParameter(int value);
        std::string getMinValue();
        std::string getMaxValue();
        std::string getAvgValue();
};

class sensor{
    private:
        std::list<std::string> parameterIds{
            temperatureKey};
        std::shared_ptr<messageParser> sampleRef;
        std::list<trackedParameter*> parameters;
        
        bool initialize();
    public:
        std::string sensorId;

        sensor(std::string Id, std::shared_ptr<messageParser> p_mp):
            sampleRef(p_mp),
            sensorId(Id)
        {
            initialize();
        };

        bool updateParameters();
        bool formatReport(std::string&);
};

class sensorHandler{
    private:
        std::shared_ptr<messageParser> sampleRef;
        std::map<std::string, sensor*> sensorMap;

        bool updateSensor(std::string sensorId);
        bool addSensor(std::string newSensorId,
                std::map<std::string, sensor*>::iterator);
    public:
        sensorHandler(std::shared_ptr<messageParser> p_mp):
            sampleRef(p_mp)
        {};

        std::string handleSample();
};

#endif //_SENSORS_H_