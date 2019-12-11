#ifndef EVENTINTERFACE_H_
    #define EVENTINTERFACE_H_

#include <vector>
#include "src/engine/EventListener.h"

struct EventInterface {

        virtual ~EventInterface() {};

        //virtual void addEventListener(EventListener& eventListener) =0;
        //virtual void removeEventListener(EventListener& eventListener) =0;

        virtual void handleEvents(std::vector<EventListener*>& eventListenerVector) =0;
        //virtual void dispatchEvents() =0;

        virtual uint32_t getTicks() =0;

        //virtual void registerAction(enum Action,  )

};

struct DummyEventInterface : public EventInterface {

    void handleEvents(std::vector<EventListener*>& eventListenerVector) {};
    uint32_t getTicks() {return 0;};

};

#endif // EVENTINTERFACE_H_
