#ifndef GRAPHICS_H
    #define GRAPHICS_H_

#include "src/Interface/GraphicsInterface.h"

#include "src/Bus.h"
#include "src/MessageHandler/Logger.h"

struct Graphics : public Bus::MessageHandler {
private:
    GraphicsInterface& graphicsInterface;
public:
    enum GraphicsMessageTypes : int {
        ///We want as little message types... so since we can instantiate the window before passing it to the Graphics MessageHandler then we might as well not have messages that manage windows...except for fullscreen and such...
        INSTANTIATE_WINDOW_MESSAGE, // We want to be able to have more than one window
        ADJUST_WINDOW_MESSAGE,      // We want to be able to adjust the window size... resolution, which display, etc.
        KILL_WINDOW_MESSAGE,        // We want to be able to free up memory that a window was using.
        INSTANTIATE_SPRITE_MESSAGE, // We want to be able to load image data and set blend data.
        KILL_SPRITE_MESSAGE,        // We want to be able to free up sprite memory. (Using an internal reference counter)
        INSTANTIATE_ANIMATOR_MESSAGE,   // We want to be able to animate sprites using the Animator class.
        KILL_ANIMATOR_MESSAGE,          // We want to be able to free up memory taken up by Animators. (Reference counter? .. maybe)
    };

    //Animator class used to animate sprites (example: [struct WalkingAnimator : public Animator {..}] would be used to animate walking for a specific sprite)
    //Animators would be defined by the Object that uses it and instantiated using Bus::sendMessage(..)
    //Animators are triggered by Actions/Events?? (idk yet...)
    /*struct Animator {
        Animator() {};
        virtual Animator() {};
        int getFrame(int a, int b, int t) =0; //or something
    };*/
    struct GraphicsData : public Bus::Data {
        GraphicsData() : Bus::Data(INSTANTIATE_SPRITE_MESSAGE) {};
        Data* clone() const override {
            return new GraphicsData();
        }
    };
    struct GraphicsMessage : public Bus::Message {
        GraphicsMessage() : Bus::Message(Bus::destination<Graphics>(), Bus::destination<GraphicsMessage>(), new GraphicsData()) {};
    };

    Graphics(GraphicsInterface& graphicsInterface) : graphicsInterface(graphicsInterface) {};

    bool handle(Bus::Message& message) {
        bool handled(message.id == Bus::destination<Graphics>());
        if(handled) {
            switch(message.getType()) {
                case INSTANTIATE_SPRITE_MESSAGE: {
                    //
                    break;
                }
                default: {
                    handled = false;
                }
            }
        }
        return handled;
    }
    private:
       // GraphicsInterface& graphcisInterface;

};

#endif // GRAPHICS_H
