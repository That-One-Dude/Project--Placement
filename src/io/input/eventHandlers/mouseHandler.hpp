/******************************************************************************
* File: mouseHandler.hpp
* Author: Tony Skeps
* Date: June 15, 2022
******************************************************************************/

#include "../eventHandler.hpp"

#ifndef MOUSE_HANDLER_HPP
#define MOUSE_HANDLER_HPP

namespace Game::IO::Input::EventHandlers {

    bool handleMouseEvents(const sf::Window&, const sf::Event&, InputData&);

}

#endif