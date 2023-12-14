/******************************************************************************
* File: levelData.hpp
* Author: Tony Skeps
* Date: June 24, 2023
******************************************************************************/

#include <vector>
#include <fstream>
#include "symbols/symbol.hpp"
#include "boards/board.hpp"
#include "../../gl/glMath.hpp"

#ifndef LEVEL_DATA_HPP
#define LEVEL_DATA_HPP

namespace Game::Logic::Level {

    struct PreplacedSymbol {
        Symbols::Symbol* symbol;
        unsigned short cell;
    };

    struct PlaceableSymbol {
        Symbols::Symbol* symbol;
        unsigned char count;
        GL::vec2 initialOffset;
        GL::vec2 initialDirection;
        GL::mat2 recursiveTransformation;
    };

    class LevelData {
    public:
        //LevelData(std::ifstream input);
        LevelData() {}
        Boards::Board* board;
        GL::mat4 transform;
        unsigned short numPreplacedSymbols;
        unsigned short numPlaceableSymbols;
        PreplacedSymbol* preplacedSymbols;
        PlaceableSymbol* placeableSymbols;
    };

}

#endif