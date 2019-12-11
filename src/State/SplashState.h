#ifndef SPLASHSTATE_H_
    #define SPLASHSTATE_H_

#include "src/Bus.h"
#include "src/MessageHandler/StateManager.h"

class SplashState : public State {
    unsigned int timer;
    unsigned int seconds;
    SplashState(unsigned int timer, unsigned int seconds);
    public:
        SplashState();
        ~SplashState();
        void enter() override;
        void exit() override;
        void update(unsigned int elapsedTime) override;
        void render(unsigned int elapsedTime) override;
        void resume() override;
        State* clone() const override;
};
#endif // SPLASHSTATE_H_
