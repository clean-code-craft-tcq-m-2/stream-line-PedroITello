#include "sensors.h"
#include "sensorConstants.h"

using namespace std;

/* --- Tracked parameter definitions --- */
bool trackedParameter::isMaxValue(int value){
    if((maximumValue<value)||(NOTINITIALIZED == maximumValue)){
        maximumValue=value;
        return true;
    }
    return false;
}

bool trackedParameter::isMinValue(int value){
    if((minimumValue>value)||(NOTINITIALIZED == minimumValue)){
        minimumValue=value;
        return true;
    }
    return false;
}

bool trackedParameter::computeMovingAvg(int value){
    updateAvgBuffer(value);
    if(MOVINGBASE!=avgSampleCount){
        avgSampleCount++;
    }
    if(MOVINGBASE==avgSampleCount){
        calculateAverage();
        return true;
    }
    return false;
}

void trackedParameter::updateAvgBuffer(const int value){
    averageBuffer[avgWriteIndex]=value;
    avgWriteIndex++;
    if(MOVINGBASE==avgWriteIndex){
        avgWriteIndex=0;
    }
}

void trackedParameter::calculateAverage(){
    movingAverage=0;
    for(unsigned char i = 0; i<MOVINGBASE; i++){
        movingAverage+=averageBuffer[i];
    }
    movingAverage=movingAverage/MOVINGBASE;
}

bool trackedParameter::updateParameter(int value){
    isMaxValue(value);
    isMinValue(value);
    computeMovingAvg(value);
    return true;
}

string trackedParameter::getMinValue(){
    return minimumValue!=NOTINITIALIZED?to_string(minimumValue):notInitPrompt;
}

string trackedParameter::getMaxValue(){
    return maximumValue!=NOTINITIALIZED?to_string(maximumValue):notInitPrompt;
}

string trackedParameter::getAvgValue(){
    return movingAverage!=NOTINITIALIZED?to_string(movingAverage):notInitPrompt;
}


/* --- Sensor definitions --- */
bool sensor::initialize(){
    for(auto parameterId:parameterIds){
        trackedParameter* p_param = new trackedParameter(parameterId);
        parameters.push_back(p_param);
    }
    return true;
}

bool sensor::updateParameters(){
    int value=0;
    for(auto parameter:parameters){
        sampleRef->getValueFromKey(parameter->parameterLabel, value);
        parameter->updateParameter(value);
    }
    return true;
}

bool sensor::formatReport(string& outputBuffer){
    outputBuffer = "";
    for(auto parameter:parameters){
        outputBuffer += parameter->parameterLabel;
        outputBuffer += " - MinValue: " + parameter->getMinValue();
        outputBuffer += " - MaxValue: " + parameter->getMaxValue();
        outputBuffer += " - AvgValue: " + parameter->getAvgValue();
        outputBuffer += "\n";
    }
    return true;
}


/* --- Sensor Handler definitions --- */
bool sensorHandler::addSensor(string newSensorId,
        map<string, sensor*>::iterator sensorIt){
    sensor* p_s = new sensor(newSensorId, sampleRef);
    std::pair<string, sensor*> tp(newSensorId, p_s);
    sensorIt = (sensorMap.insert(tp)).first;
    return true;
}

string sensorHandler::handleSample(){
    string output;
    string sAux;
    sampleRef->getValueFromKey(sensorIdKey,sAux);
    map<string, sensor*>::iterator sensorIt = \
        sensorMap.find(sAux);
    if(sensorIt == sensorMap.end()){
        addSensor(sAux, sensorIt);
    }
    sensorIt = sensorMap.find(sAux);
    sensorIt->second->updateParameters();
    sensorIt->second->formatReport(sAux);
    output="[" + sensorIt->second->sensorId + "]" + sAux;
    return output;
}