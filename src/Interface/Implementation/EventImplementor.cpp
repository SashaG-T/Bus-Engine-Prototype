#include "src/SDLBackend/EventImplementor.h"

EventImplementor::EventImplementor() {

    SDLSharedInstance::init();

}

EventImplementor::~EventImplementor() {

    SDLSharedInstance::quit();

}
/*
void EventImplementor::addEventListener(EventListener& eventListener) {
    eventListenerVector.push_back(&eventListener);
}

void EventImplementor::removeEventListener(EventListener& eventListener) {
    auto iter(std::find(eventListenerVector.begin(), eventListenerVector.end(), &eventListener));
    if(iter != eventListenerVector.end()) {
        eventListenerVector.erase(iter);
    }
}
*/
void EventImplementor::handleEvents(std::vector<EventListener*>& eventListenerVector) {

    SDL_Event event;
    while( SDL_PollEvent( &event ) ) {
        for(auto iter = eventListenerVector.begin(); iter != eventListenerVector.end(); iter++) {
            onEvent(&event, *iter);
        }
    }

/**
        Dr.Memory spit this out.


        Error #1: UNINITIALIZED READ: reading 0x007cf4cc-0x007cf4d0 4 byte(s) within 0x007cf4c8-0x007cf4d0
        # 0 system call NtUserTrackMouseEvent TRACKMOUSEEVENT post-dwFlags
        # 1 SDL2.dll!?                                                                +0x0      (0x6c8067ff <SDL2.dll+0xc67ff>)
        # 2 SDL2.dll!?                                                                +0x0      (0x6c7f7e3d <SDL2.dll+0xb7e3d>)
        # 3 SDL2.dll!?                                                                +0x0      (0x6c77b776 <SDL2.dll+0x3b776>)
        # 4 SDL2.dll!?                                                                +0x0      (0x6c7fbdf6 <SDL2.dll+0xbbdf6>)
        # 5 USER32.dll!SetManipulationInputTarget                                     +0x52     (0x742a84f3 <USER32.dll+0x384f3>)
        # 6 USER32.dll!CallWindowProcW                                                +0x2ff    (0x74286c40 <USER32.dll+0x16c40>)
        # 7 USER32.dll!DispatchMessageW                                               +0x250    (0x74286541 <USER32.dll+0x16541>)
        # 8 USER32.dll!DispatchMessageW                                               +0xf      (0x74286300 <USER32.dll+0x16300>)
        # 9 SDL2.dll!?                                                                +0x0      (0x6c7fcd55 <SDL2.dll+0xbcd55>)
        #10 SDL2.dll!?                                                                +0x0      (0x6c777df1 <SDL2.dll+0x37df1>)
        #11 EventInterface::handleEvents                                               [C:/Users/Sasha/AppData/Roaming/CodeBlocks/UserTemplates/Core Engine/src/core/EventInterface.cpp:15]
        #12 ControllerManager::update                                                  [C:/Users/Sasha/AppData/Roaming/CodeBlocks/UserTemplates/Core Engine/src/engine/ControllerManager.cpp:66]
        #13 Engine::run                                                                [C:/Users/Sasha/AppData/Roaming/CodeBlocks/UserTemplates/Core Engine/src/engine/Engine.cpp:45]
        #14 App::execute                                                               [C:/Users/Sasha/AppData/Roaming/CodeBlocks/UserTemplates/Core Engine/src/engine/App.cpp:15]
        #15 SDL_main                                                                   [C:/Users/Sasha/AppData/Roaming/CodeBlocks/UserTemplates/Core Engine/src/engine/App.cpp:29]
        #16 console_main                                                               [../src/main/windows/SDL_windows_main.c:140]
        #17 WinMain@16                                                                 [../src/main/windows/SDL_windows_main.c:177]
        #18 main                                                                       [../../../../src/gcc-4.7.1/libgcc/libgcc2.c:1059]
        Note: @0:00:27.835 in thread 10188

**/

}

void EventImplementor::onEvent(SDL_Event* Event, EventListener* currentEventListener) {
    switch(Event->type) {
//Application events
        case SDL_QUIT: {
        //user-requested quit
            currentEventListener->onQuit(Event->quit.timestamp);
            break;
        }
//Android and iOS events; see Remarks for details
        case SDL_APP_TERMINATING: {
        //OS is terminating the application
            currentEventListener->onTerminating();
            break;
        }
        case SDL_APP_LOWMEMORY: {
        //OS is low on memory; free some
            currentEventListener->onLowMemory();
            break;
        }
        case SDL_APP_WILLENTERBACKGROUND: {
        //application is entering background
            currentEventListener->onWillEnterBackground();
            break;
        }
        case SDL_APP_DIDENTERBACKGROUND: {
        //application entered background
            currentEventListener->onDidEnterBackground();
            break;
        }
        case SDL_APP_WILLENTERFOREGROUND: {
        //application is entering foreground
            currentEventListener->onWillEnterForeground();
            break;
        }
        case SDL_APP_DIDENTERFOREGROUND: {
        //application entered foreground
            currentEventListener->onDidEnterForeground();
            break;
        }
//Window events
        case SDL_WINDOWEVENT: {
        //window state change
            switch(Event->window.event) {
                case SDL_WINDOWEVENT_SHOWN: {
                    currentEventListener->onWindowShown(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_HIDDEN: {
                    currentEventListener->onWindowHidden(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_EXPOSED: {
                    currentEventListener->onWindowExposed(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_MOVED: {
                    currentEventListener->onWindowMoved(Event->window.windowID, Event->window.data1, Event->window.data2);
                    break;
                }
                case SDL_WINDOWEVENT_RESIZED: {
                    currentEventListener->onWindowResized(Event->window.windowID, Event->window.data1, Event->window.data2);
                    break;
                }
                case SDL_WINDOWEVENT_SIZE_CHANGED: {
                    currentEventListener->onWindowSizeChanged(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_MINIMIZED: {
                    currentEventListener->onWindowMinimized(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_MAXIMIZED: {
                    currentEventListener->onWindowMaximized(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_RESTORED: {
                    currentEventListener->onWindowRestored(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_ENTER: {
                    currentEventListener->onWindowGainedMouseFocus(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_LEAVE: {
                    currentEventListener->onWindowLostMouseFocus(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_FOCUS_GAINED: {
                    currentEventListener->onWindowGainedKeyboardFocus(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_FOCUS_LOST: {
                    currentEventListener->onWindowLostKeyboardFocus(Event->window.windowID);
                    break;
                }
                case SDL_WINDOWEVENT_CLOSE: {
                    currentEventListener->onWindowClose(Event->window.windowID);
                    break;
                }
            }
            break;
        }
        case SDL_SYSWMEVENT: {
        //system specific event
            ///currentEventListener->onSysWMEvent(Event->syswm.type, Event->syswm.timestamp, Event->syswm.msg);
            break;
        }
//Keyboard events
        case SDL_KEYDOWN: {
        //key pressed
            Key key;
            key.keycode = Event->key.keysym.sym;
            key.scancode = (Scancode)Event->key.keysym.scancode;
            key.modifier = Event->key.keysym.mod;
            currentEventListener->onKeyDown(key, Event->key.windowID, Event->key.timestamp);
            break;
        }
        case SDL_KEYUP: {
        //key released
            Key key;
            key.keycode = Event->key.keysym.sym;
            key.scancode = (Scancode)Event->key.keysym.scancode;
            key.modifier = Event->key.keysym.mod;
            currentEventListener->onKeyUp(key, Event->key.windowID, Event->key.timestamp);
            break;
        }
        case SDL_TEXTEDITING: {
        //keyboard text editing (composition)
            currentEventListener->onTextEditing(Event->edit.text, Event->edit.start, Event->edit.length, Event->edit.windowID, Event->edit.timestamp);
            break;
        }
        case SDL_TEXTINPUT: {
        //keyboard text input
            currentEventListener->onTextInput(Event->text.text, Event->text.windowID, Event->text.timestamp);
            break;
        }
//Mouse events
        case SDL_MOUSEMOTION: {
        //mouse moved
            bool left = Event->motion.state & SDL_BUTTON_LMASK;
            bool right = Event->motion.state & SDL_BUTTON_RMASK;
            bool middle = Event->motion.state & SDL_BUTTON_MMASK;
            bool x1 = Event->motion.state & SDL_BUTTON_X1MASK;
            bool x2 = Event->motion.state & SDL_BUTTON_X2MASK;
            currentEventListener->onMouseMotion(Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel, left, right, middle, x1, x2, Event->motion.which, Event->motion.windowID, Event->motion.timestamp);
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
        //mouse button pressed
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    currentEventListener->onLeftMouseButtonDown(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    currentEventListener->onRightMouseButtonDown(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    currentEventListener->onMiddleMouseButtonDown(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_X1: {
                    currentEventListener->onX1MouseButtonDown(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_X2: {
                    currentEventListener->onX2MouseButtonDown(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
            }
            break;
        }
        case SDL_MOUSEBUTTONUP: {
        //mouse button released
            switch(Event->button.button) {
                case SDL_BUTTON_LEFT: {
                    currentEventListener->onLeftMouseButtonUp(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    currentEventListener->onRightMouseButtonUp(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    currentEventListener->onMiddleMouseButtonUp(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_X1: {
                    currentEventListener->onX1MouseButtonUp(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
                case SDL_BUTTON_X2: {
                    currentEventListener->onX2MouseButtonUp(Event->button.x, Event->button.y, Event->button.which, Event->button.windowID, Event->button.timestamp);
                    break;
                }
            }
            break;
        }
        case SDL_MOUSEWHEEL: {
        //mouse wheel motion
            currentEventListener->onMouseWheel(Event->wheel.x, Event->wheel.y, Event->wheel.which, Event->wheel.windowID, Event->wheel.timestamp);
            break;
        }
//Joystick events
        case SDL_JOYAXISMOTION: {
        //joystick axis motion
            currentEventListener->onJoyAxisMotion(Event->jaxis.axis, Event->jaxis.value, Event->jaxis.which, Event->jaxis.timestamp);
            break;
        }
        case SDL_JOYBALLMOTION: {
        //joystick trackball motion
            currentEventListener->onJoyBallMotion(Event->jball.ball, Event->jball.xrel, Event->jball.yrel, Event->jball.which, Event->jball.timestamp);
            break;
        }
        case SDL_JOYHATMOTION: {
        //joystick hat position change
            currentEventListener->onJoyHatMotion(Event->jhat.hat, Event->jhat.value, Event->jhat.which, Event->jhat.timestamp);
            break;
        }
        case SDL_JOYBUTTONDOWN: {
        //joystick button pressed
            currentEventListener->onJoyButtonDown(Event->jbutton.button, Event->jbutton.which, Event->jbutton.timestamp);
            break;
        }
        case SDL_JOYBUTTONUP: {
        //joystick button released
            currentEventListener->onJoyButtonUp(Event->jbutton.button, Event->jbutton.which, Event->jbutton.timestamp);
            break;
        }
        case SDL_JOYDEVICEADDED: {
        //joystick connected
            currentEventListener->onJoyDeviceAdded(Event->jdevice.which, Event->jdevice.timestamp);
            break;
        }
        case SDL_JOYDEVICEREMOVED: {
        //joystick disconnected
            currentEventListener->onJoyDeviceRemoved(Event->jdevice.which, Event->jdevice.timestamp);
            break;
        }
//Controller events
        case SDL_CONTROLLERAXISMOTION: {
        //controller axis motion
            currentEventListener->onControllerAxisMotion(Event->caxis.axis, Event->caxis.value, Event->caxis.which, Event->caxis.timestamp);
            break;
        }
        case SDL_CONTROLLERBUTTONDOWN: {
        //controller button pressed
            currentEventListener->onControllerButtonDown(Event->cbutton.button, Event->cbutton.which, Event->cbutton.timestamp);
            break;
        }
        case SDL_CONTROLLERBUTTONUP: {
        //controller button released
            currentEventListener->onControllerButtonUp(Event->cbutton.button, Event->cbutton.which, Event->cbutton.timestamp);
            break;
        }
        case SDL_CONTROLLERDEVICEADDED: {
        //controller connected
            currentEventListener->onControllerDeviceAdded(Event->cdevice.which, Event->cdevice.timestamp);
            break;
        }
        case SDL_CONTROLLERDEVICEREMOVED: {
        //controller disconnected
            currentEventListener->onControllerDeviceRemoved(Event->cdevice.which, Event->cdevice.timestamp);
            break;
        }
        case SDL_CONTROLLERDEVICEREMAPPED: {
        //controller mapping updated
            currentEventListener->onControllerDeviceRemapped(Event->cdevice.which, Event->cdevice.timestamp);
            break;
        }
//Touch events
        case SDL_FINGERDOWN: {
        //user has touched input device
            currentEventListener->onFingerDown(Event->tfinger.x, Event->tfinger.y, Event->tfinger.dx, Event->tfinger.dy, Event->tfinger.pressure, Event->tfinger.fingerId, Event->tfinger.touchId, Event->tfinger.timestamp);
            break;
        }
        case SDL_FINGERUP: {
        //user stopped touching input device
            currentEventListener->onFingerUp(Event->tfinger.x, Event->tfinger.y, Event->tfinger.dx, Event->tfinger.dy, Event->tfinger.pressure, Event->tfinger.fingerId, Event->tfinger.touchId, Event->tfinger.timestamp);
            break;
        }
        case SDL_FINGERMOTION: {
        //user is dragging finger on input device
            currentEventListener->onFingerMotion(Event->tfinger.x, Event->tfinger.y, Event->tfinger.dx, Event->tfinger.dy, Event->tfinger.pressure, Event->tfinger.fingerId, Event->tfinger.touchId, Event->tfinger.timestamp);
            break;
        }
        //Gesture events
        case SDL_DOLLARGESTURE: {
            currentEventListener->onDollarGesture(Event->dgesture.x, Event->dgesture.y, Event->dgesture.error, Event->dgesture.numFingers, Event->dgesture.gestureId, Event->dgesture.touchId, Event->dgesture.timestamp);
            break;
        }
        case SDL_DOLLARRECORD: {
            currentEventListener->onDollarRecord(/* pamareters!?! */);
            break;
        }
        case SDL_MULTIGESTURE: {
            currentEventListener->onMultiGesture(Event->mgesture.x, Event->mgesture.y, Event->mgesture.numFingers, Event->mgesture.dDist, Event->mgesture.dTheta, Event->mgesture.touchId, Event->mgesture.timestamp);
            break;
        }
//Clipboard events
        case SDL_CLIPBOARDUPDATE: {
        //the clipboard changed
            currentEventListener->onClipboardUpdate(/* parameters!?! */);
            break;
        }
        //Drag and drop events
        case SDL_DROPFILE: {
        //the system requests a file open
            currentEventListener->onDropFile(Event->drop.file, Event->drop.timestamp);
            break;
        }
//These are for your use, and should be allocated with SDL_RegisterEvents()
        case SDL_USEREVENT: {
        //a user-specified event
            currentEventListener->onUserEvent(Event->user.type, Event->user.code, Event->user.data1, Event->user.data2, Event->user.windowID, Event->user.timestamp);
            break;
        }
        case SDL_LASTEVENT: {
        //only for bounding internal arrays
            currentEventListener->onLastEvent();
            break;
        }
    }
}

uint32_t EventImplementor::getTicks() {
    return SDL_GetTicks();
}
