#include <stdio.h>
#include <memory>

#include "src/Bus.h"
#include "src/MessageHandler/Logger.h"
#include "src/MessageHandler/Graphics.h"
#include "src/MessageHandler/StateManager.h"

#include "src/Interface/Implementation/GraphicsImplementor.h"

#include "src/State/SplashState.h"

int main(int argc, char* argv[]) {

    //Setup engine components
    //Screen size, app name, org name, etc.
    #ifdef DEBUG
    Logger logger;
    #else
    Logger logger("Log.txt"); //must be difrerent log files...unless we share the filestream... or close them after each print...
    Bus::setOutput("ErrorLog.txt");
    #endif // DEBUG
    GraphicsImplementor graphicsImplementation;
    graphicsImplementation.setWindowSize(320, 200);
    graphicsImplementation.setWindowTitle("Engine Prototype");
    graphicsImplementation.createWindow();
    graphicsImplementation.setWindowIcon("resources/icon.bmp");
    Graphics graphics(graphicsImplementation);
    StateManager stateManager;

    Bus::addMessageHandler(logger);
    Bus::addMessageHandler(graphics);
    Bus::addMessageHandler(stateManager);

    //initialize additional variables
    ///...
    ///SplashState splashState;

    //Queue initial messages
    //These message will be initial setup messages, one would instantiate the splashstate.
    //The splashstate will send it's own messages to setup it's resources.
    Bus::sendMessage(Logger::LoggerMessage("This line will be logged."));

    //Bus::sendMessage(Bus::Message(Bus::destination<Bus::All>(), Bus::NULL_DATA));
    //Bus::sendMessage(Graphics::GraphicsMessage());
    Bus::sendMessage(StateManager::InitializeStateMessage<SplashState>()); ///Initializes SplashState
    //Bus::sendMessage(StateManager::InitializeStateMessage<SplashState>(splashState)); //should instantiate a completely different splashState...

    Bus::sendMessage(Logger::LoggerMessage("The Last Message!!!"));

    printf("%d\n", Bus::destination<Logger>());
    printf("%d\n", Bus::destination<StateManager>());
    printf("%d\n", Bus::destination<Bus::All>());
    printf("%d\n", Bus::destination<Graphics>());
    printf("%d\n", Bus::destination<Bus::Null>());

    printf("%d\n", Bus::destination<StateManager::TerminateStateMessage>());
    printf("%d\n", Bus::destination<StateManager::InitializeStateMessage<SplashState>>());
    printf("%d\n", Bus::destination<Bus::TerminateMessage>());
    printf("%d\n", Bus::destination<Bus::UpdateMessage>());

    printf("%d\n", Bus::destination<Bus::RenderMessage>());
    printf("%d\n", Bus::destination<Graphics::GraphicsMessage>());
    printf("%d\n", Bus::destination<Logger::LoggerMessage>());
    printf("%d\n", Bus::destination<Bus::RunError>());

    //Start the engine
    //Initial messages will queue more messages.
    Bus::run();

    return 0;
}
