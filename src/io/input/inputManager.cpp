/******************************************************************************
* File: inputManager.cpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <iostream>
#include "eventHandler.hpp"
#include "gameStates/main/mouseHandler.hpp"
//#include "eventHandlers/mouseHandler.hpp"
#include "../../universalData.hpp"
#include "inputManager.hpp"

namespace Game::IO::Input {

    std::unordered_map<std::string, std::vector<EventHandler>> stateHandlers;

    void eventLoop(bool* active, std::mutex* logicLock, std::mutex* windowLock, sf::Window* window) {

        sf::Event events[32];

        //EventHandler eventHandlers[] {};
        //int numEventHandlers = sizeof(eventHandlers) / sizeof(EventHandler);

        stateHandlers["mainGameState"].push_back((EventHandler)GameStates::Main::handleMouseEvents);

        InputData inputData;
        inputData.clear();
        //bool newInput = false;

        int i;
        bool shouldContinue;

        std::chrono::steady_clock::time_point start, end;

        while (*active) {

            start = std::chrono::steady_clock::now();

            windowLock->lock();
            window->setActive(true);

            //std::cout << "Event Loop!" << std::endl;

            shouldContinue = true;

            while (shouldContinue) {

                for (i = 0; i < 32 && shouldContinue; i++) {
                    shouldContinue = window->pollEvent(events[i]);
                }

                i--;

                if (i > 0) {

                    logicLock->lock();

                    for (int j = 0; j < i; j++) {

                        if (events[j].type == sf::Event::Closed) {
                            *active = false;
                            continue;
                        }

                        if (events[j].type == sf::Event::Resized) {
                            viewportXSize = events[j].size.width;
                            viewportYSize = events[j].size.height;
                            viewportRatio = (float)viewportYSize / viewportXSize;
                            continue;
                        }

                        Logic::GameStates::GameState* currentGameState = gameState.getCurrentGameState();

                        std::vector<EventHandler>& eventHandlers = stateHandlers[currentGameState->rendererName];

                        /*for (int k = 0; k < numEventHandlers; k++) {
                            if (eventHandlers[k](*window, events[j], inputData)) {
                                newInput = true;
                                break;
                            }
                        }*/

                        for (int k = 0; k < eventHandlers.size(); k++) {
                            if (eventHandlers[k](*window, events[j], currentGameState)) {
                                break;
                            }
                        }

                    }

                    logicLock->unlock();

                }
            }

            if (!shouldContinue) {
                window->setActive(false);
                windowLock->unlock();
            }

            /*if (newInput) {
                logicLock->lock();
                gameState.recieveInput(inputData);
                logicLock->unlock();
                inputData.clear();
                newInput = false;
            }*/

            end = std::chrono::steady_clock::now();
            std::chrono::nanoseconds duration = std::chrono::milliseconds(10) - (end - start);

            if (duration.count() > 0) {
                std::this_thread::sleep_for(duration);
            } else {
                std::this_thread::yield();
            }
        }

    }

}
