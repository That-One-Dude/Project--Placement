/******************************************************************************
* File: mouseHandler.hpp
* Author: Tony Skeps
* Date: August 20, 2023
******************************************************************************/

#include "../../eventHandler.hpp"
#include "../../../../logic/gameStates/main/mainGameStateClass.hpp"

#ifndef MAIN_MOUSE_HANDLER_HPP
#define MAIN_MOUSE_HANDLER_HPP

namespace Game::IO::Input::GameStates::Main {

    bool handleMouseEvents(const sf::Window&, const sf::Event&, Logic::GameStates::Main::MainGameState*);

}

#endif