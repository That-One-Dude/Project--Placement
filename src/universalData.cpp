/******************************************************************************
* File: universalData.cpp
* Author: Tony Skeps
* Date: June 15, 2022
******************************************************************************/

#include "universalData.hpp"

namespace Game {

    Logic::GameStates::BaseGameState gameState;

    Logic::Level::Boards::CompositeBoardFactory boardFactory;
    Logic::Level::Symbols::CompositeSymbolFactory symbolFactory;

    std::chrono::steady_clock::time_point last_tick;

    int viewportXSize = 1600, viewportYSize = 1200;
    float viewportRatio = ((float)viewportXSize) / viewportYSize;

}
