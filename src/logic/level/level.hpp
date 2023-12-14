/******************************************************************************
* File: level.hpp
* Author: Tony Skeps
* Date: April 20, 2023
******************************************************************************/

#include <vector>

#include "boards/cell.hpp"
#include "symbols/symbol.hpp"
#include "boardState.hpp"
#include "levelData.hpp"

#ifndef LEVEL_HPP
#define LEVEL_HPP

namespace Game::Logic::Level {

    struct PlacementData {
        float xPos, yPos;
        Symbols::Symbol* symbol;
    };

    enum PlaceResult {
        FAILED = 0b001,
        SUCCESS_UNSATISFIED_BOARD = 0b010,
        SUCCESS = 0b100
    };

    class Level {
    public:
        Level(const LevelData* levelData);
        ~Level();
        PlaceResult placeSymbol(const Boards::Cell* cell);
        PlacementData* pickUpSymbol(const Symbols::Symbol* symbol);
        void hoverSymbol(const Boards::Cell* cell);
        void adjustSymbol(const Boards::Cell* cell);
        PlacementData* getCurrentlyPickedUpSymbol() const {return currentlyPickedUpSymbol;}
        bool recalculate();
        void update();
        bool isSatisfied() const;
        const BoardState& getBoardState() const {return boardState;}
        const LevelData* getLevelData() const {return levelData;}
        const std::vector<const Boards::CellColor*>& getCellColorOrder() const {return cellColorOrder;}
        const std::vector<PlacementData>& getPlacemenetData() const {return symbolsToPlace;}
    private:
        std::vector<PlacementData> symbolsToPlace;
        PlacementData* currentlyPickedUpSymbol = nullptr;
        std::vector<PlacementData>::iterator location;
        BoardState boardState;
        const LevelData* levelData;
        std::vector<const Boards::CellColor*> cellColorOrder;
    };

}

#endif