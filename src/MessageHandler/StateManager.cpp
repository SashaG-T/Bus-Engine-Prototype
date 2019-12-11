#include "src/MessageHandler/StateManager.h"

StateManager::StateManagerData::StateManagerData(enum StateManagerMessageTypes type, State * state)
:   Bus::Data(type), state(state)
{};

StateManager::StateManagerData::StateManagerData(enum StateManagerMessageTypes type)
:   Bus::Data(type), state(0)
{};

StateManager::StateManagerData::~StateManagerData() {
    delete state;
}

StateManager::StateManagerData::StateManagerData(StateManagerData const & stateManagerData)
:   Bus::Data(stateManagerData.type),
    state(stateManagerData.state ? stateManagerData.state->clone() : 0)
{}

StateManager::StateManagerData::StateManagerData(StateManagerData&& stateManagerData)
:   Bus::Data(stateManagerData.type),
    state(stateManagerData.state)
{
    stateManagerData.state = 0;
}

StateManager::StateManagerData& StateManager::StateManagerData::operator=(StateManagerData const & stateManagerData) {
    if(&stateManagerData != this) {
        this->type = stateManagerData.type;
        this->state = stateManagerData.state ? stateManagerData.state->clone() : 0;
    }
    return *this;
}
StateManager::StateManagerData& StateManager::StateManagerData::operator=(StateManagerData&& stateManagerData) {
    if(&stateManagerData != this) {
        this->type = stateManagerData.type;
        this->state = stateManagerData.state;
        stateManagerData.state = 0;
    }
    return *this;
}

Bus::Data* StateManager::StateManagerData::clone() const {
    return new StateManagerData((StateManagerMessageTypes)type, state ? state->clone() : 0);
}

StateManager::TerminateStateMessage::TerminateStateMessage()
:   Bus::Message(Bus::destination<StateManager>(), Bus::destination<TerminateStateMessage>(), new StateManagerData(TERMINATE_MESSAGE))
{};

bool StateManager::handle(Bus::Message& message) {
    bool handled(true);
    if(message.id == Bus::destination<Bus::All>()) {
        switch(message.getType()) {
            case Bus::UPDATE_MESSAGE: {
                //called between frames, all da tim!
                //Bus::sendMessage(Logger::LoggerMessage("garbage message"));
                if(!stateStack.empty()) {
                    Bus::UpdateData const & updateData(message.retrieve<Bus::UpdateData>());
                    stateStack.back().state->update(updateData.getElapsedTime());
                }
                break;
            }
            case Bus::RENDER_MESSAGE: {
                //called before frame is rendered
                if(!stateStack.empty()) {
                    Bus::RenderData const & renderData(message.retrieve<Bus::RenderData>());
                    stateStack.back().state->render(renderData.getElapsedTime());
                }
                break;
            }
            default: {
                handled = false;
                break;
            }
        }
    } else if(message.id == Bus::destination<StateManager>()) {
        switch(message.getType()) {
            case INITIALIZE_MESSAGE: {
                //copy State to state stack...
                Bus::sendMessage(Logger::LoggerMessage("StateManager init state"));
                stateStack.push_back(message.retrieve<StateManagerData>());
                stateStack.back().state->enter();
                break;
            }
            case TERMINATE_MESSAGE: {
                stateStack.back().state->exit();
                stateStack.pop_back(); //destroy state on top of stack
                if(stateStack.empty()) {
                    Bus::sendQuickMessage(Logger::LoggerMessage("Terminate message sent."));
                    Bus::sendMessage(Bus::TerminateMessage());
                } else {
                    stateStack.back().state->resume();
                }
                break;
            }
            default: {
                handled = false;
                break;
            }
        }
    } else {
        handled = false;
    }
    return handled;
}
