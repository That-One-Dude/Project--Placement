/******************************************************************************
* File: truncatedSquareGridBoard.hpp
* Author: Tony Skeps
* Date: August 28, 2023
******************************************************************************/

#include "board.hpp"

#ifndef TRUNCATED_SQUARE_GRID_BOARD_HPP
#define TRUNCATED_SQUARE_GRID_BOARD_HPP

namespace Game::Logic::Level::Boards {

    class TruncatedSquareGridBoard: public Board {
    public:
        TruncatedSquareGridBoard(unsigned int xSize, unsigned int ySize, CellColor* cells);
        ~TruncatedSquareGridBoard() {delete[] cells;}
        virtual void fillCells(std::vector<const Cell*>&) const;
        const unsigned int xSize, ySize;
        const unsigned int cellGridSize, truncationCellsStart;
        const CellOrNot* getCellGrid() const {return cells;}
    private:
        CellOrNot* cells;
        
    };

    extern const BoardFactory* truncatedSquareGridBoardFactory;
}

#endif