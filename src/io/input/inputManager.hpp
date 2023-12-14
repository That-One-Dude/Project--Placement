/******************************************************************************
* File: inputManager.hpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <thread>
#include <mutex>
#include <SFML/Window.hpp>

#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

namespace Game::IO::Input {

    void eventLoop(bool* active, std::mutex* logicLock, std::mutex* windowLock, sf::Window* window);

}

#endif
