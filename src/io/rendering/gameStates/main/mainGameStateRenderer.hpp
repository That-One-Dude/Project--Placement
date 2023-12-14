/******************************************************************************
* File: mainGameStateRenderer.hpp
* Author: Tony Skeps
* Date: June 13, 2022
******************************************************************************/

#include "../../batchRenderer.hpp"
#include "../../../../logic/gameStates/main/mainGameStateClass.hpp"

#ifndef MAIN_GAME_STATE_RENDERER
#define MAIN_GAME_STATE_RENDERER

namespace Game::IO::Rendering::GameStates::Main {

    extern unsigned int intermediatePostFrameBuffer;

    void mainGameStateInit();

    void renderMainGameState(Game::Logic::GameStates::Main::MainGameState*, unsigned int viewportXSize, unsigned int viewportYSize);

}

#endif