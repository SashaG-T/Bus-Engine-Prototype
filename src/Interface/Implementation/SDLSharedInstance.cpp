#include "src/Interface/Implementation/SDLSharedInstance.h"
#include <sstream>

SDLSharedInstance* SDLSharedInstance::instance = 0;

SDLSharedInstance::SDLSharedInstance() {

    if(SDL_Init(SDL_INIT_EVERYTHING)) {

            fprintf(stderr, "Error: SDL could not be initialized: %s\n", SDL_GetError());
            std::stringstream ss;
            ss << "Error: SDL could not be initialized: \n" << SDL_GetError();
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                             "Generic Jumper | Sauteur G\u00e9n\u00e9rique",
                             ss.str().c_str(),
                             NULL);
            exit(1);
    }

    atexit(SDL_Quit);
/*
    if(SDLNet_Init()==-1) {

        fprintf(stderr, "Error: SDL_Net could not be initialized: %s\n", SDLNet_GetError());
        exit(1);
    }

    atexit(SDLNet_Quit);
*/
}

SDLSharedInstance::~SDLSharedInstance() {
    //SDLNet_Quit(); //only necessary if the user stops using SDL but then re-initializes it in the same program execution...which would probably never happen.
    SDL_Quit();
}
