#ifndef SDLBACKEND_H_
    #define SDLBACKEND_H_

#include "src/engine/Backend.h"

#include "src/SDLBackend/GraphicsImplementor.h"
#include "src/SDLBackend/AudioImplementor.h"
#include "src/SDLBackend/EventImplementor.h"
#include "src/SDLBackend/FileImplementor.h"

struct SDLBackend : public Backend {

    GraphicsImplementor graphicsInterface;
    AudioImplementor audioInterface;
    EventImplementor eventInterface;
    FileImplementor fileInterface;

    SDLBackend() {
        setGraphicsInterface(graphicsInterface);
        setAudioInterface(audioInterface);
        setEventInterface(eventInterface);
        setFileInterface(fileInterface);
    }

};

#endif // SDLBACKEND_H_
