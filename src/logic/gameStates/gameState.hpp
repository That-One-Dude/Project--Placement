/******************************************************************************
* File: gameState.hpp
* Author: Tony Skeps
* Date: June 11, 2022
******************************************************************************/

#include <string>
#include "../../io/input/inputData.hpp"

#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

namespace Game::Logic::GameStates {

    class GameState {
    public:
        GameState(GameState*, const std::string& rendererName);
        GameState(GameState**, const std::string& rendererName);
        ~GameState();
        virtual void tick() = 0;
        virtual void recieveInput(IO::Input::InputData) = 0;
        GameState* getPrevious();
        const std::string rendererName;
    protected:
        void exit();
    private:
        GameState* previous;
        GameState** holder;
    };

    class BaseGameState: public GameState {
    public:
        BaseGameState();
        ~BaseGameState();
        virtual void tick();
        virtual void recieveInput(IO::Input::InputData);
        GameState* getCurrentGameState();
    private:
        GameState* holder;
    };

}

#endif