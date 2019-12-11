#include "src/State/SplashState.h"

SplashState::SplashState(unsigned int timer, unsigned int seconds)
:   timer(timer), seconds(seconds)
{}

SplashState::SplashState()
:   timer(0), seconds(0)
{}

SplashState::~SplashState() {
}

void SplashState::enter() {
    //Allocate resources.
    //Grab resources.
}

void SplashState::exit() {
    //Release resources.
    //Destroy resources.
}

void SplashState::update(unsigned int elapsedTime) {
    //Update state
    Bus::sendMessage(Logger::LoggerMessage("SplashState is updating."));
    timer += elapsedTime;
    if(timer > 1000) {
        timer -= 1000;
        seconds++;
        Bus::sendMessage(Logger::LoggerMessage("SplashState 1 second!............................."));
        if(seconds == 5) {
            Bus::sendMessage(StateManager::TerminateStateMessage());
        }
    }
}

void SplashState::render(unsigned int elapsedTime) {
    //Pre-render udpate
    Bus::sendMessage(Logger::LoggerMessage("SplashState is preping for render"));
}

void SplashState::resume() {
    //Update stale resources
    Bus::sendMessage(Logger::LoggerMessage("SplashState is resuming"));
}

State* SplashState::clone() const {
    return new SplashState(timer, seconds);
}
