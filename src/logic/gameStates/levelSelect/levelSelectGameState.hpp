/******************************************************************************
* File: levelSelectGameState.hpp
* Author: Tony Skeps
* Date: June 10, 2023
******************************************************************************/

#include <string>
#include "../../level/level.hpp"
#include "../gameState.hpp"

#ifndef LEVEL_SELECT_GAME_STATE_HPP
#define LEVEL_SELECT_GAME_STATE_HPP

namespace Game::Logic::GameStates::LevelSelect {

    void init();

    class LevelSelectGameState: public GameState {
    public:
        LevelSelectGameState(GameState*);
        virtual void tick();
        virtual void recieveInput(IO::Input::InputData);

        void selectLevel(int level);
    private:
        float horizontalFacing, verticalFacing;
        float pushX, pushY;
    };

}

#endif

