/******************************************************************************
* File: hexagonalGridBoard.hpp
* Author: Tony Skeps
* Date: August 28, 2023
******************************************************************************/

#include "board.hpp"

#ifndef HEXAGONAL_GRID_BOARD_HPP
#define HEXAGONAL_GRID_BOARD_HPP

namespace Game::Logic::Level::Boards {

    class HexagonalGridBoard: public Board {
    public:
        HexagonalGridBoard(unsigned int aSize, unsigned int bSize, unsigned int cSize, CellColor* cells);
        ~HexagonalGridBoard() {delete[] cells;}
        virtual void fillCells(std::vector<const Cell*>&) const;
        const unsigned int aSize, bSize, cSize;
        const unsigned int cellGridSize;
        const CellOrNot* getCellGrid() const {return cells;}
    private:
        CellOrNot* cells;
        
    };

    extern const BoardFactory* hexagonalGridBoardFactory;
}

#endif