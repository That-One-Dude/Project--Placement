/******************************************************************************
* File: squareGridBoard.hpp
* Author: Tony Skeps
* Date: July 1, 2023
******************************************************************************/

#include "board.hpp"

#ifndef SQUARE_GRID_BOARD_HPP
#define SQUARE_GRID_BOARD_HPP

namespace Game::Logic::Level::Boards {

    class SquareGridBoard: public Board {
    public:
        SquareGridBoard(/*std::vector<Cell>* cells, */unsigned int xSize, unsigned int ySize, CellColor* cells/*, unsigned int* indexGrid*/);
        ~SquareGridBoard() {delete[] cells;}
        virtual void fillCells(std::vector<const Cell*>&) const;
        const unsigned int xSize, ySize;
        //unsigned int* indexGrid;
        const CellOrNot* getCellGrid() const {return cells;}
    private:
        CellOrNot* cells;
        
    };

    extern const BoardFactory* squareGridBoardFactory;
}

#endif