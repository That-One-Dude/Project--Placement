/******************************************************************************
* File: logicManager.hpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <mutex>

#ifndef LOGIC_MANAGER_HPP
#define LOGIC_MANAGER_HPP

namespace Game::Logic {

    void mainLoop(bool* active, std::mutex* lock);

}

#endif
