/******************************************************************************
* File: levelSelectGameState.cpp
* Author: Tony Skeps
* Date: June 10, 2023
******************************************************************************/

#include <cmath>
#include "levelSelectGameState.hpp"

namespace Game::Logic::GameStates::LevelSelect {

    const float highestAngle = M_PI_2 - 0.1f;

    void init() {
        
    }

    LevelSelectGameState::LevelSelectGameState(GameState* state): GameState(state, "levelSelectGameState") {

    }

    void LevelSelectGameState::tick() {

        
        
    }

    void LevelSelectGameState::recieveInput(IO::Input::InputData data) {

        horizontalFacing += data.xDeltaLook;
        verticalFacing += data.yDeltaLook;
        while (horizontalFacing >= M_PIf) {
            horizontalFacing -= M_PIf * 2;
        }
        while (horizontalFacing < -M_PIf) {
            horizontalFacing += M_PIf * 2;
        }
        if (verticalFacing > highestAngle) {
            verticalFacing = highestAngle;
        }
        if (verticalFacing < -highestAngle) {
            verticalFacing = -highestAngle;
        }

    }

}
