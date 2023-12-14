/******************************************************************************
* File: squareGridBoard.cpp
* Author: Tony Skeps
* Date: July 1, 2023
******************************************************************************/

#include <cmath>
#include "squareGridBoard.hpp"

namespace Game::Logic::Level::Boards {

    SquareGridBoard::SquareGridBoard(unsigned int xSize, unsigned int ySize, CellColor* colors): Board(), xSize(xSize), ySize(ySize), cells(new CellOrNot[xSize * ySize]) {

        // Step 1: make Cells
        for (unsigned int i = 0; i < xSize * ySize; i++) {
            if (colors[i] != nocell) {
                cells[i].is = true;
                cells[i].cell.cell = Cell(colors[i], 8, 4);
            } else {
                cells[i].is = false;
            }
        }

        // Step 2: connect cells
        //     1
        //   3 2 1
        // 2 4 c 0 0
        //   5 6 7
        //     3        
        for (unsigned int i = 0; i <= ySize; i++) {
            unsigned int row = i * xSize;
            for (unsigned int j = 0; j <= xSize; j++) {
                Cell* left = nullptr;
                Cell* down = nullptr;
                Cell* downleft = nullptr;
                Cell* center = nullptr;
                if (i < ySize && j > 0 && cells[row + j - 1].is) {
                    left = &(cells[row + j - 1].cell.cell);
                }
                if (i > 0 && j < xSize && cells[row + j - xSize].is) {
                    down = &(cells[row + j - xSize].cell.cell);
                }
                if (i > 0 && j > 0 && cells[row + j - xSize - 1].is) {
                    downleft = &(cells[row + j - xSize - 1].cell.cell);
                }
                if (i < ySize && j < xSize && cells[row + j].is) {
                    center = &(cells[row + j].cell.cell);
                }
                connectAdjacentCells(left, 0, 4, 0, 0.0f, center, 4, 0, 2, M_PI);
                connectAdjacentCells(down, 2, 6, 1, M_PI_2, center, 6, 2, 3, 3 * M_PI_2);
                connectCells(downleft, 1, 5, M_PI_4, center, 5, 1, 5 * M_PI_4);
                connectCells(left, 7, 3, 7 * M_PI_4, down, 3, 7, 3 * M_PI_4);
            }
        }
    }

    void SquareGridBoard::fillCells(std::vector<const Cell*>& cellContainer) const {
        for (unsigned int i = 0; i < xSize * ySize; i++) {
            if (cells[i].is) {
                cellContainer.push_back(&(cells[i].cell.cell));
            }
        }
    }

    class: public BoardFactory {
        Board* produce(void* data) const {
            unsigned char* trueData = (unsigned char*)data;
            unsigned int xSize = *(unsigned int*)(trueData + 0);
            unsigned int ySize = *(unsigned int*)(trueData + 4);
            return new SquareGridBoard(xSize, ySize, (CellColor*)(trueData + 8));
        }
        unsigned int getDataSize(Board* board) const {
            SquareGridBoard* actualBoard = (SquareGridBoard*)board;
            return sizeof(unsigned int) * 2 + sizeof(CellColor) * actualBoard->xSize * actualBoard->ySize;
        }
        void writeData(void* data, Board* board) const {
            SquareGridBoard* actualBoard = (SquareGridBoard*)board;
            unsigned int offset = 0;
            *(unsigned int*)(data + offset) = actualBoard->xSize;
            offset += sizeof(unsigned int);
            *(unsigned int*)(data + offset) = actualBoard->ySize;
            offset += sizeof(unsigned int);
            const CellOrNot* grid = actualBoard->getCellGrid();
            for (unsigned int i = 0; i < actualBoard->xSize * actualBoard->ySize; i++) {
                CellColor color = nocell;
                if (grid[i].is) {
                    color = grid[i].cell.cell.getBaseColor();
                }
                *(CellColor*)(data + offset) = color;
                offset += sizeof(CellColor);
            }
        }
    } value;

    const BoardFactory* squareGridBoardFactory = &value;

}
