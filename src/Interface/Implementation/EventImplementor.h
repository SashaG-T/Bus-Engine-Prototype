#ifndef EVENTIMPLEMENTOR_H_
    #define EVENTIMPLEMENTOR_H_

#include <vector>
#include <algorithm>

#include "src/engine/Key.h"
#include "src/engine/EventInterface.h"
#include "src/SDLBackend/SDLSharedInstance.h"

class EventImplementor : public EventInterface {

    private:
        std::vector<EventListener*> eventListenerVector;

    //handle event, redirect it to the currentEventListner's related function.
        void onEvent(SDL_Event* event, EventListener* eventListener);

    public:
        EventImplementor();
        ~EventImplementor();

        //void addEventListener(EventListener& eventListener);
        //void removeEventListener(EventListener& eventListener);

        void handleEvents(std::vector<EventListener*>& eventListenerVector);
        //void dispatchEvents();

        uint32_t getTicks();

};

#endif // EVENTIMPLEMENTOR_H_
