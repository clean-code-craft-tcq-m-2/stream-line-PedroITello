#ifndef _MESSAGE_PARSER_H_
#define _MESSAGE_PARSER_H_

#include <string>
#include <map>

const static int CBUFFSIZE = 150;

class messageParser{
    public:
        std::map<std::string, std::string>sample;
        
        messageParser(){};
        bool mapMessage(std::string& payload);
        bool getValueFromKey(const std::string& key, int& payload);
        bool getValueFromKey(const std::string& key, std::string& payload);

    private:
        char cBuffer[CBUFFSIZE];
        bool cleanMessage(std::string&);
        bool separateTokens(const std::string&);
};

#endif // _MESSAGE_PARSER_H_