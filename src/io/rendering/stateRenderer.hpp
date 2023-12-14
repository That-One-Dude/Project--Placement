/******************************************************************************
* File: stateRenderer.hpp
* Author: Tony Skeps
* Date: June 11, 2022
******************************************************************************/

#include "../../logic/gameStates/gameState.hpp"
#include "batchRenderer.hpp"

#ifndef STATE_RENDERER_HPP
#define STATE_RENDERER_HPP

namespace Game::IO::Rendering {

    typedef void (*StateRenderer) (Game::Logic::GameStates::GameState*, unsigned int viewportXSize, unsigned int viewportYSize);

}

#endif