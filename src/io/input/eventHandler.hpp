/******************************************************************************
* File: eventHandler.hpp
* Author: Tony Skeps
* Date: June 15, 2022
******************************************************************************/

#include <SFML/Window.hpp>
#include "inputData.hpp"
#include "../../logic/gameStates/gameState.hpp"

#ifndef EVENT_HANDLER_HPP
#define EVENT_HANDLER_HPP

namespace Game::IO::Input {

    typedef bool (*EventHandler) (const sf::Window&, const sf::Event&, Logic::GameStates::GameState*);

    /*class EventHandler {
    public:
        EventHandler() {}
        virtual bool handleEvent(const sf::Window&, const sf::Event&, InputData&) = 0;
        virtual void cleanUp(InputData&) = 0;
        virtual void doWindowStuff(const sf::Window&) = 0;
    };*/

}

#endif