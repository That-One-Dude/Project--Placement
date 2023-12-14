/******************************************************************************
* File: ioManager.hpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <mutex>

#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP

namespace Game::IO {

    void activate(bool* active, std::mutex* lock);

}

#endif
