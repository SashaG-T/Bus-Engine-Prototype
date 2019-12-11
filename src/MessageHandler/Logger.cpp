#include "src/MessageHandler/Logger.h"

Logger::LoggerData::LoggerData(char const * s)
:   Bus::Data(LOGGING_MESSAGE), s(s)
{}

Bus::Data* Logger::LoggerData::clone() const {
    return new LoggerData(s);
}

char const * Logger::LoggerData::c_str() const {
    return s;
}

Logger::LoggerMessage::LoggerMessage(char const * s)
:   Bus::Message(Bus::destination<Logger>(), Bus::destination<LoggerMessage>(), new Logger::LoggerData(s))
{}

Logger::Logger()
:   outputToFile(false),
    filename(0)
{}

Logger::Logger(char const * filename)
:   outputToFile(true),
    filename(filename)
{
    fs.open(filename, std::fstream::trunc | std::fstream::out);
}

bool Logger::handle(Bus::Message& message) {
    bool handled(false);
    if(message.id == Bus::destination<Logger>()) {
        LoggerData const & loggerData(message.retrieve<LoggerData>());
        log(loggerData.c_str());
        handled = true;
    } else if(message.id == Bus::destination<Bus::All>()) {
        std::stringstream ss;
        handled = true;
        switch(message.getType()) {
            case Bus::UPDATE_MESSAGE: {
                Bus::UpdateData const & updateData(message.retrieve<Bus::UpdateData>());
                ss << "Elapsed Update Time: " << updateData.getElapsedTime();
                log(ss.str().c_str());
                break;
            }
            case Bus::RENDER_MESSAGE: {
                Bus::RenderData const & renderData(message.retrieve<Bus::RenderData>());
                ss << "Elapsed Render Time: " << renderData.getElapsedTime();
                log(ss.str().c_str());
                break;
            }
            default: {
                handled = false;
            }
        }
    }
    return handled;
}

void Logger::log(char const * s) {
    if(outputToFile) {
        fs << s << std::endl;
    } else {
        printf("Logger: %s\n", s);
    }
}
