/******************************************************************************
* File: boardState.cpp
* Author: Tony Skeps
* Date: July 9, 2023
******************************************************************************/

#include "boards/cellInstance.hpp"
#include "boardState.hpp"

namespace Game::Logic::Level {

    bool BoardState::setCellColor(const Boards::Cell* cell, Boards::CellColor color) {
        if (cellColors.at(cell).setColor(color)) {
            updatedCells.insert(cell);
            return true;
        }
        return false;
    }

    bool BoardState::lockCellColor(const Boards::Cell* cell) {
        return cellColors.at(cell).lock();
    }
}
