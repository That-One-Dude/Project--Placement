/******************************************************************************
* File: logicManager.cpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <thread>
#include <iostream>
#include "gameStates/gameState.hpp"
#include "gameStates/main/mainGameState.hpp"
#include "../universalData.hpp"
#include "logicManager.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define OFFSET_POINTER(toOffset, offset) ((void*)(toOffset) + (offset))

namespace Game::Logic {

    std::chrono::steady_clock::time_point last_tick;

    void mainLoop(bool* active, std::mutex* lock) {

        GameStates::Main::init();

        GameStates::Main::MainGameState a(&Game::gameState);

        std::chrono::steady_clock::time_point start;

        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

        int tickCount = 0;

        while (*active) {

            start = std::chrono::steady_clock::now();

            lock->lock();

            Game::gameState.tick();

            lock->unlock();

            { // FPS debug

                tickCount++;

                std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

                if (time_span.count() > 1.0) {
                    std::cout << "Ticks Per Second: " << (tickCount / time_span.count()) << std::endl;
                    tickCount = 0;
                    t1 = t2;
                }

            }

            last_tick = std::chrono::steady_clock::now();
            std::chrono::nanoseconds duration = std::chrono::milliseconds(20) - (last_tick - start);

            if (duration.count() > 0) {
                std::this_thread::sleep_for(duration);
            } else {
                std::this_thread::yield();
            }

        }

    }

}
