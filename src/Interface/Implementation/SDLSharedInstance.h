#ifndef SDLSHAREDINSTANCE_H_
    #define SDLSHAREDINSTANCE_H_

#include "SDL2/SDL.h"
/*
//#include "SDL2/SDL_net.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
*/
#include <stdio.h>
#include <stdlib.h>

///Used so that SDL is not instantiated more than once. Also so that SDL_Quit is called only when all classes using it are done.

class SDLSharedInstance {

    private:
        SDLSharedInstance();
        int referenceCount;

        static SDLSharedInstance* instance;

    public:
        ~SDLSharedInstance();

        static void init() {
            if(!instance) {
                instance = new SDLSharedInstance();
            }
            instance->referenceCount++;
        }

        static void quit() {
            if(instance) {
                instance->referenceCount--;
                if(instance->referenceCount < 1) {
                    delete instance;
                    instance = 0;
                }
            }
        }

};

#endif // SDLSHAREDINSTANCE_H_
