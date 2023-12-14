/******************************************************************************
* File: universalData.hpp
* Author: Tony Skeps
* Date: June 13, 2022
******************************************************************************/

#include <chrono>
#include <unordered_map>
#include "namespacedId.hpp"
#include "logic/gameStates/gameState.hpp"
#include "logic/level/boards/boardFactory.hpp"
#include "logic/level/symbols/symbolFactory.hpp"

#ifndef UNIVERSAL_DATA_HPP
#define UNIVERSAL_DATA_HPP

namespace Game {

    // Requires the Logic Lock
    extern Logic::GameStates::BaseGameState gameState;

    // Not sure which lock it requires
    extern Logic::Level::Boards::CompositeBoardFactory boardFactory;
    extern Logic::Level::Symbols::CompositeSymbolFactory symbolFactory;

    // Requires the Logic Lock
    extern std::chrono::steady_clock::time_point last_tick;

    // Requires the Window Lock
    extern int viewportXSize, viewportYSize;
    extern float viewportRatio;

}

#endif