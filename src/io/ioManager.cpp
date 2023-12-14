/******************************************************************************
* File: ioManager.cpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <thread>
#include <string>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "rendering/renderingManager.hpp"
#include "input/inputManager.hpp"
#include "ioManager.hpp"

namespace Game::IO {

    /******************************************************************************
    * Global Constants
    ******************************************************************************/

    const uint ANIMATION_FRAME_RATE ( 0 );
    const uint SURFACE_WIDTH ( 1600 );
    const uint SURFACE_HEIGHT ( 1200 );
    const float RATIO = ((float) SURFACE_HEIGHT) / SURFACE_WIDTH;

    int maxFPS = 0;

    using namespace sf;
    using namespace std;

    void activate(bool* active, std::mutex* lock) {

        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.antialiasingLevel = 2;
        settings.stencilBits = 8;
        //settings.majorVersion = 3;
        //settings.minorVersion = 0;

        RenderWindow window( VideoMode( SURFACE_WIDTH, SURFACE_HEIGHT ),
                    "Project: OffGrid", sf::Style::Default, settings );

        window.setFramerateLimit ( ANIMATION_FRAME_RATE );
        window.setVerticalSyncEnabled (true);
        window.setActive(false);

        std::mutex windowLock;

        std::thread renderingThread(Game::IO::Rendering::renderLoop, active, lock, &windowLock, &window);
        Input::eventLoop(active, lock, &windowLock, &window);
        renderingThread.join();

    }

}
