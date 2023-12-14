/******************************************************************************
* File: renderingManager.hpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>

#ifndef RENDERING_MANAGER_HPP
#define RENDERING_MANAGER_HPP

namespace Game::IO::Rendering {

    void setFPS(int fps);

    void releaseMouseControl();

    void renderLoop(bool* active, std::mutex* logicLock, std::mutex* windowLock, sf::RenderWindow* window);

}

#endif