#include "src/Bus.h"

#include "src/MessageHandler/Logger.h"

namespace Bus {

    Data::Data(int type) : type(type) {};
    Data* Data::clone() const {
        return new Data(type);
    };

    ErrorData::ErrorData(char const * s) : Data(ERROR_MESSAGE), s(s) {};
    Data* ErrorData::clone() const {
        return new ErrorData(s);
    };
    char const * ErrorData::c_str() const {
        return s;
    }

    UpdateData::UpdateData(unsigned int & elapsedTime) : Data(UPDATE_MESSAGE), elapsedTime(elapsedTime) {};
    Data* UpdateData::clone() const {
        return new UpdateData(elapsedTime);
    }
    unsigned int UpdateData::getElapsedTime() const {
        return elapsedTime;
    }

    RenderData::RenderData(unsigned int & elapsedTime) : Data(RENDER_MESSAGE), elapsedTime(elapsedTime) {};
    Data* RenderData::clone() const {
        return new RenderData(elapsedTime);
    };
    unsigned int RenderData::getElapsedTime() const {
        return elapsedTime;
    }
    static unsigned int counter = 0;
    Message::Message() : id(0), source(0), data(new Data(NULL_MESSAGE)), timestamp(0) {};
    Message::Message(dest_t id, dest_t source, Data* data) : id(id), source(source), data(data), timestamp(counter++) {}
    Message::~Message() {
        delete data;
    }

    /// WE WANT MESSAGE TO HAVE COMPLETE OWNERSHIP OF DATA*...
    Message::Message(Message const & message) :
        id(message.id),
        source(message.source),
        data(message.data ? message.data->clone() : 0),
        timestamp(message.timestamp) {}
    Message::Message(Message&& message) : id(message.id), source(message.source), data(message.data), timestamp(message.timestamp) {
        message.data = 0;
    }
    Message& Message::operator=(Message const & message) {
        if(&message != this) {
            this->id = message.id;
            this->source = message.source;
            this->data = message.data ? message.data->clone() : 0;
            this->timestamp = message.timestamp;
        }
        return *this;
    }
    Message& Message::operator=(Message&& message) {
        if(&message != this) {
            this->id = message.id;
            this->source = message.source;
            this->data = message.data;
            message.data = 0;
            this->timestamp = message.timestamp;
        }
        return *this;
    }
    int Message::getType() const {
        return data->type;
    }

    char const * filename(0);
    std::fstream fs;
    std::deque<Message> messageDeque1;
    std::deque<Message> messageDeque2;
    std::deque<Message>* messageDequePtr(&messageDeque1);
    std::deque<Message>* pendingMessageDequePtr(&messageDeque2);
    std::vector<MessageHandler*> messageHandlerVec;

    void setOutput(char const * filename) {
        if(filename) {
            fs.close();
        }
        Bus::filename = filename;
        if(filename) {
            fs.open(filename, std::fstream::trunc | std::fstream::out);
        } else {
            fs.close();
        }
    }

    void log(char const * s) {
        if(filename) {
            fs << s << std::endl;
        } else {
            printf("Bus: %s\n", s);
        }
    }

    void addMessageHandler(MessageHandler& messageHandler) {
        messageHandlerVec.push_back(&messageHandler);
    }

    void sendMessage(Message const & message) {
        pendingMessageDequePtr->push_back(message);
    }

    void sendQuickMessage(Message const & message) {
        messageDequePtr->push_back(message);
    }

    void sendSuperQuickMessage(Message const & message) {
        messageDequePtr->push_front(message);
    }

    void run() {
        clock_t updateTime(clock());
        unsigned int updateElapsedTime;
        unsigned int renderElapsedTime;
        unsigned int renderTimer(0);
        UpdateMessage updateMessage(updateElapsedTime);
        RenderMessage renderMessage(renderElapsedTime);
        bool running(true);
        while(running) {
            //swap messagequeues
            std::deque<Message>* tmpMessageDeque(messageDequePtr);
            messageDequePtr = pendingMessageDequePtr;
            pendingMessageDequePtr = tmpMessageDeque;
            //update elapsedTime
            clock_t checkTime(clock());
            updateElapsedTime = ((checkTime - updateTime)*1000) / CLOCKS_PER_SEC;
            updateElapsedTime = updateElapsedTime > 500 ? 500 : updateElapsedTime;
            updateTime = checkTime;
            //Send update and render messages
            sendMessage(updateMessage);
            renderTimer += updateElapsedTime;
            if(renderTimer >= 500) { // Don't render when we don't need too.
                renderElapsedTime = renderTimer;
                renderTimer -= 500;
                sendMessage(renderMessage);
            }
            while(!messageDequePtr->empty()) {
                Message& message(messageDequePtr->front());
                if(message.id == destination<Null>()) {
                    switch(message.getType()) {
                        case TERMINATE_MESSAGE: {
                            running = false;
                            break;
                        }
                        case ERROR_MESSAGE: {
                            ErrorData const & errorData(message.retrieve<ErrorData>());
                            std::stringstream ss;
                            ss << "Error: " << errorData.c_str();
                            log(ss.str().c_str());
                            break;
                        }
                        default: {};
                    }
                } else {
                    bool handled(false);
                    for(MessageHandler* messageHandler : messageHandlerVec) {
                        handled = messageHandler->handle(message) || handled;
                    }
                    if(!handled) {
                        Message errorMessage(destination<Null>(), destination<RunError>(), new ErrorData("Invalid Message or no message received; Ignoring."));
                        sendMessage(errorMessage);
                    }
                }
                messageDequePtr->pop_front();
            }
        }
    }
}
