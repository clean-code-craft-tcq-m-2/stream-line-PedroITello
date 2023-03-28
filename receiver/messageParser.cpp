#include "messageParser.h"
#include <algorithm>
#include <list>
#include <cstring>
#include <cctype>

using namespace std;

bool messageParser::cleanMessage(string& payload){
    payload.erase(remove(payload.begin(),payload.end(),' '),payload.end());
    payload.erase(remove(payload.begin(),payload.end(),'\''),payload.end());
    payload.erase(remove(payload.begin(),payload.end(),'{'),payload.end());
    payload.erase(remove(payload.begin(),payload.end(),'}'),payload.end());
    return true;
}

bool messageParser::separateTokens(const string& payload){
    // Ensure sample clearance
    sample.clear();

    // Split into tokens from separators
    strcpy(cBuffer, payload.c_str());
    char* tokenptr;
    list<string>parts;
    tokenptr = strtok(cBuffer, ":,");
    while (tokenptr!= NULL){
        parts.push_back(static_cast<string>(tokenptr));
        tokenptr = strtok(NULL, ":,");
    }

    // Map tokens
    for(list<string>::iterator li = parts.begin(); li != parts.end(); li++){
        string key=*li;
        li++;
        if(li==parts.end()){
            return false;
        }
        string value=*li;
        sample.insert(pair<string,string>(key,value));
    }
    return true;
}

bool messageParser::mapMessage(string& payload){
    return (cleanMessage(payload) & separateTokens(payload));
}

bool messageParser::getValueFromKey(const string& key, int& payload){
    string value = (*(sample.find(key))).second;
    for(int i = 0; i<static_cast<int>(value.length()); ++i){
        if(!isdigit(value.c_str()[i])){
            return false;
        }
    }
    payload = stoi(value);
    return true;
}

bool messageParser::getValueFromKey(const string& key, string& payload){
    payload = (*(sample.find(key))).second;
    return true;
}