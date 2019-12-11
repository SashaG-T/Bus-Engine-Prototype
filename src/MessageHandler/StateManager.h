#ifndef STATEMANAGER_H_
    #define STATEMANAGER_H_

#include <vector>

#include "src/Bus.h"
#include "src/MessageHandler/Logger.h"

struct State {
    State() {};
    virtual ~State() {};
    virtual void enter() {};
    virtual void exit() {};
    virtual void update(unsigned int elapsedTime) {};
    virtual void render(unsigned int elapsedTime) {};
    virtual void resume() {};
    virtual State* clone() const =0;
};

struct StateManager : public Bus::MessageHandler {
    enum StateManagerMessageTypes : int {
        INITIALIZE_MESSAGE,
        TERMINATE_MESSAGE
    };
    private:
    struct StateManagerData : public Bus::Data {
        State * state; /// Has ownership
        StateManagerData(enum StateManagerMessageTypes type, State * state);
        StateManagerData(enum StateManagerMessageTypes type);
        ~StateManagerData();
        StateManagerData(StateManagerData const & stateManagerData);
        StateManagerData(StateManagerData&& stateManagerData);
        StateManagerData& operator=(StateManagerData const & stateManagerData);
        StateManagerData& operator=(StateManagerData&& stateManagerData);
        Data* clone() const override;
    };
    std::vector<StateManagerData> stateStack;
    public:
    template<typename T>
    struct InitializeStateMessage : public Bus::Message {
        /// Maybe replace [T const & state] with [StateManager::Initializer<T> const & stateInitializer] where [StateManager::Initializer<T>] is a struct that holds init values.
        InitializeStateMessage(T const & state)
        :   Bus::Message(Bus::destination<StateManager>(),
                         Bus::destination<InitializeStateMessage>(),
                         new StateManagerData(INITIALIZE_MESSAGE, new T(state)))
        {};
        InitializeStateMessage()
        :   Bus::Message(Bus::destination<StateManager>(),
                         Bus::destination<InitializeStateMessage>(),
                         new StateManagerData(INITIALIZE_MESSAGE, new T()))
        {};
    };
    struct TerminateStateMessage : public Bus::Message {
        TerminateStateMessage();
    };

    bool handle(Bus::Message& message);
};

#endif // STATEMANAGER_H_
