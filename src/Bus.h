#ifndef BUS_H_
    #define BUS_H_

#include <stdio.h>
#include <vector>
#include <deque>
#include <time.h>
#include <fstream>
#include <sstream>

namespace Bus {

    enum MessageTypes : int {
        NULL_MESSAGE,
        TERMINATE_MESSAGE,
        ERROR_MESSAGE,
        UPDATE_MESSAGE,
        RENDER_MESSAGE,
    };

    typedef int dest_t;

    struct All {};  //Used to send to all BusHandlers
    struct Null{};  //Used to send to bus
    template<typename T>    //Used to generate unique id's for destinations.
    dest_t destination() {
        return (dest_t)&destination<T>;
    }
    template <>
    constexpr dest_t destination<Null>() {
        return 0;
    }

    ///All Data should be STL container friendly.
    struct Data {
        int type;
        Data(int type);
        virtual Data* clone() const;
    };
    //static Data* const NULL_DATA(0);
    class ErrorData : public Data {
        char const * s;
        public:
        ErrorData(char const * s);
        Data* clone() const override;
        char const * c_str() const;
    };
    class UpdateData : public Data {
        unsigned int & elapsedTime;
        public:
            UpdateData(unsigned int & elapsedTime);
            Data* clone() const override;
            unsigned int getElapsedTime() const;
    };
    class RenderData : public Data {
        unsigned int & elapsedTime;
        public:
            RenderData(unsigned int & elapsedTime);
            Data* clone() const override;
            unsigned int getElapsedTime() const;
    };

    struct Message {
        dest_t id;
        dest_t source;
        unsigned int timestamp;
        Data* data; //complete ownership of pointerà
        Message();
        Message(dest_t id, dest_t source, Data* data);
        //Message(dest_t id, Data* data, void(*) deleter);
        ~Message(); //call deleter on data
        Message(Message const & message);
        Message(Message&& message);
        Message& operator=(Message const & message);
        Message& operator=(Message&& message);
        int getType() const;
        template<typename T>
        T const & retrieve() {
            return *((T*)data);
        }
    };

    struct TerminateMessage : public Message {
        TerminateMessage() : Message(destination<Null>(), destination<TerminateMessage>(), new Data(TERMINATE_MESSAGE)) {};
    };

    struct UpdateMessage : public Message {
        UpdateMessage(unsigned int & elapsedTime) : Message(destination<All>(), destination<UpdateMessage>(), new UpdateData(elapsedTime)) {};
    };

    struct RenderMessage : public Message {
        RenderMessage(unsigned int & elapsedTime) : Message(destination<All>(), destination<RenderMessage>(), new RenderData(elapsedTime)) {};
    };

    struct RunError{};

    struct MessageHandler {
        virtual bool handle(Message& message) =0; // return true if message was handled.
    };

    void setOutput(char const * filename); // set the output file to dump errors and such during runtime
    void log(char const * s);
    void addMessageHandler(MessageHandler& messageHandler);
    void sendMessage(Message const & message);
    void sendQuickMessage(Message const & message);
    void sendSuperQuickMessage(Message const & message);
    void run();
}

#endif // BUS_H_
