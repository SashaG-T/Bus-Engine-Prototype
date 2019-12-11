#ifndef LOGGER_H_
    #define LOGGER_H_

#include <stdio.h>
#include <fstream>
#include <sstream>

#include "src/Bus.h"

struct Logger : public Bus::MessageHandler {
    enum LoggerMessageTypes : int {
        LOGGING_MESSAGE
    };
    class LoggerData : public Bus::Data {
        char const * s;
        public:
            LoggerData(char const * s);
            Data* clone() const override;
            char const * c_str() const;
    };
    struct LoggerMessage : public Bus::Message {
        LoggerMessage(char const * c_str);
    };
    Logger();
    Logger(char const * filename);
    bool handle(Bus::Message& message);
    private:
        bool outputToFile;
        char const * filename;
        std::fstream fs;
        void log(char const * s);
};

#endif // LOGGER_H_
