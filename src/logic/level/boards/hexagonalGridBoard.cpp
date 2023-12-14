/******************************************************************************
* File: hexagonalGridBoard.cpp
* Author: Tony Skeps
* Date: August 28, 2023
******************************************************************************/

#include <cmath>
#include "hexagonalGridBoard.hpp"

namespace Game::Logic::Level::Boards {

    unsigned int getNumCells(unsigned int aSize, unsigned int bSize, unsigned int cSize) {
        unsigned int base = (aSize + bSize - 1) * (bSize + cSize - 1);
        base -= bSize * (bSize - 1);
        return base; 
    }

    HexagonalGridBoard::HexagonalGridBoard(unsigned int aSize, unsigned int bSize, unsigned int cSize, CellColor* colors): Board(), aSize(aSize), bSize(bSize), cSize(cSize), cellGridSize(getNumCells(aSize, bSize, cSize)), cells(new CellOrNot[cellGridSize]) {

        // Step 1: make Cells
        for (unsigned int i = 0; i < cellGridSize; i++) {
            if (colors[i] != nocell) {
                cells[i].is = true;
                cells[i].cell.cell = Cell(colors[i], 6, 6);
            } else {
                cells[i].is = false;
            }
        }

        // Step 2: connect cells
        //     2
        // 3       1
        //     c
        // 4       0
        //     5
        unsigned int indices[(aSize + bSize - 1) * (bSize + cSize - 1)];
        for (unsigned int i = 0; i < (aSize + bSize - 1) * (bSize + cSize - 1); i++) {
            indices[i] = -1;
        }
        unsigned int index = 0;
        unsigned int maxI = bSize + cSize - 1;
        for (unsigned int i = 0; i <= maxI; i++) {
            unsigned int start = 0, end = aSize + bSize - 1;
            if (i < bSize) end -= (bSize - 1 - i);
            if (i >= cSize) start += (i - cSize + 1);
            unsigned int row = i * maxI;
            for (unsigned int j = start; j <= end; j++) {
                if (i < maxI && j < end) {
                    indices[j + i * maxI] = index;
                    index++;
                }
                Cell* upLeft = nullptr;
                Cell* down = nullptr;
                Cell* downLeft = nullptr;
                Cell* center = nullptr;
                if (i < maxI && j > 0 && indices[row + j - 1] != (unsigned int)-1 && cells[indices[row + j - 1]].is) {
                    upLeft = &(cells[indices[row + j - 1]].cell.cell);
                }
                if (i > 0 && j < end && indices[row + j - maxI] != (unsigned int)-1 && cells[indices[row + j - maxI]].is) {
                    down = &(cells[indices[row + j - maxI]].cell.cell);
                }
                if (i > 0 && j > 0 && indices[row + j - maxI - 1] != (unsigned int)-1 && cells[indices[row + j - maxI - 1]].is) {
                    downLeft = &(cells[indices[row + j - maxI - 1]].cell.cell);
                }
                if (i < maxI && j < end && cells[indices[row + j]].is) {
                    center = &(cells[indices[row + j]].cell.cell);
                }
                connectAdjacentCells(upLeft, 0, 3, 0, 11 * M_PI / 6, center, 3, 0, 3, 5 * M_PI / 6);
                connectAdjacentCells(down, 2, 5, 2, M_PI_2, center, 5, 2, 5, 3 * M_PI_2);
                connectAdjacentCells(downLeft, 1, 4, 1, M_PI / 6, center, 4, 1, 4, 7 * M_PI / 6);
            }
            if (start > 0) {
                Cell* down = nullptr;
                if (i > 0 && indices[row + start - 1 - maxI] != (unsigned int)-1 && cells[indices[row + start - 1 - maxI]].is) {
                    down = &(cells[indices[row + start - 1 - maxI]].cell.cell);
                }
                connectAdjacentCells(down, 2, 5, 2, M_PI_2, nullptr, 5, 2, 5, 3 * M_PI_2);
            }
        }
        /*for (unsigned int i = 0; i < ySize; i++) {
            unsigned int row = i * ySize;
            for (unsigned int j = 0; j < xSize; j++) {
                Cell* left = nullptr;
                Cell* down = nullptr;
                Cell* downleft = nullptr;
                Cell* center = nullptr;
                if (j > 0 && cells[row + j - 1].is) {
                    left = &(cells[row + j - 1].cell.cell);
                }
                if (i > 0 && cells[row + j - ySize].is) {
                    down = &(cells[row + j - ySize].cell.cell);
                }
                if (i > 0 && j > 0 && cells[row + j - ySize - 1].is) {
                    downleft = &(cells[row + j - ySize - 1].cell.cell);
                }
                if (cells[row + j].is) {
                    center = &(cells[row + j].cell.cell);
                }
                connectAdjacentCells(left, 0, 4, 0, 0.0f, center, 4, 0, 2, M_PI);
                connectAdjacentCells(down, 2, 6, 1, M_PI_2, center, 6, 2, 3, 3 * M_PI_2);
                connectCells(downleft, 1, 5, M_PI_4, center, 5, 1, 5 * M_PI_4);
                connectCells(left, 7, 3, 7 * M_PI_4, down, 3, 7, 3 * M_PI_4);
            }
        }*/
    }

    void HexagonalGridBoard::fillCells(std::vector<const Cell*>& cellContainer) const {
        for (unsigned int i = 0; i < cellGridSize; i++) {
            if (cells[i].is) {
                cellContainer.push_back(&(cells[i].cell.cell));
            }
        }
    }

    class: public BoardFactory {
        Board* produce(void* data) const {
            unsigned char* trueData = (unsigned char*)data;
            unsigned int aSize = *(unsigned int*)(trueData + 0);
            unsigned int bSize = *(unsigned int*)(trueData + 4);
            unsigned int cSize = *(unsigned int*)(trueData + 4);
            return new HexagonalGridBoard(aSize, bSize, cSize, (CellColor*)(trueData + 12));
        }
        unsigned int getDataSize(Board* board) const {
            HexagonalGridBoard* actualBoard = (HexagonalGridBoard*)board;
            return sizeof(unsigned int) * 3 + sizeof(CellColor) * actualBoard->cellGridSize;
        }
        void writeData(void* data, Board* board) const {
            HexagonalGridBoard* actualBoard = (HexagonalGridBoard*)board;
            unsigned int offset = 0;
            *(unsigned int*)(data + offset) = actualBoard->aSize;
            offset += sizeof(unsigned int);
            *(unsigned int*)(data + offset) = actualBoard->bSize;
            offset += sizeof(unsigned int);
            *(unsigned int*)(data + offset) = actualBoard->cSize;
            offset += sizeof(unsigned int);
            const CellOrNot* grid = actualBoard->getCellGrid();
            for (unsigned int i = 0; i < actualBoard->cellGridSize; i++) {
                CellColor color = nocell;
                if (grid[i].is) {
                    color = grid[i].cell.cell.getBaseColor();
                }
                *(CellColor*)(data + offset) = color;
                offset += sizeof(CellColor);
            }
        }
    } trueHexagonalGridBoardFactory;

    const BoardFactory* hexagonalGridBoardFactory = &trueHexagonalGridBoardFactory;

}
