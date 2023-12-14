/******************************************************************************
* File: main.cpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <thread>
#include <string>
#include <mutex>
#include <X11/Xlib.h>
#include "logic/logicManager.hpp"
#include "io/ioManager.hpp"

/******************************************************************************
* Function: main
******************************************************************************/
int main(int argc, char ** argv) {

    // Setup threads
    XInitThreads();

    // Begin Loops
    bool active = true;
    std::mutex logicLock;

    std::thread logicThread(Game::Logic::mainLoop, &active, &logicLock);
    Game::IO::activate(&active, &logicLock);
    logicThread.join();

    return 0;

}