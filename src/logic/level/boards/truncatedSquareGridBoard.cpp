/******************************************************************************
* File: truncatedSquareGridBoard.cpp
* Author: Tony Skeps
* Date: August 28, 2023
******************************************************************************/

#include <cmath>
#include "truncatedSquareGridBoard.hpp"

namespace Game::Logic::Level::Boards {

    TruncatedSquareGridBoard::TruncatedSquareGridBoard(unsigned int xSize, unsigned int ySize, CellColor* colors): Board(), xSize(xSize), ySize(ySize), truncationCellsStart(xSize * ySize), cellGridSize(xSize * ySize + (xSize + 1) * (ySize + 1)), cells(new CellOrNot[cellGridSize]) {

        // Step 1: make Cells
        for (unsigned int i = 0; i < truncationCellsStart; i++) {
            if (colors[i] != nocell) {
                cells[i].is = true;
                cells[i].cell.cell = Cell(colors[i], 8, 8);
            } else {
                cells[i].is = false;
            }
        }
        for (unsigned int i = truncationCellsStart; i < cellGridSize; i++) {
            if (colors[i] != nocell) {
                cells[i].is = true;
                cells[i].cell.cell = Cell(colors[i], 4, 4);
            } else {
                cells[i].is = false;
            }
        }

        // Step 2: connect cells
        // 3   2   1    1     0
        //   
        // 4   o   0       s
        //   
        // 5   6   7    2     3
        for (unsigned int i = 0; i < ySize; i++) {
            unsigned int row = i * xSize;
            unsigned int row2 = i * (xSize + 1);
            for (unsigned int j = 0; j < xSize; j++) {
                Cell* center = nullptr;
                Cell* upRight = nullptr;
                Cell* upLeft = nullptr;
                Cell* downRight = nullptr;
                Cell* downLeft = nullptr;
                Cell* down = nullptr;
                Cell* left = nullptr;

                if (cells[row + j].is) {
                    center = &(cells[row + j].cell.cell);
                }
                if (cells[truncationCellsStart + row2 + j + (xSize + 1) + 1].is) {
                    upRight = &(cells[truncationCellsStart + row2 + j + (xSize + 1) + 1].cell.cell);
                }
                if (cells[truncationCellsStart + row2 + j + (xSize + 1)].is) {
                    upLeft = &(cells[truncationCellsStart + row2 + j + (xSize + 1)].cell.cell);
                }
                if (cells[truncationCellsStart + row2 + j].is) {
                    downLeft = &(cells[truncationCellsStart + row2 + j].cell.cell);
                }
                if (cells[truncationCellsStart + row2 + j + 1].is) {
                    downRight = &(cells[truncationCellsStart + row2 + j + 1].cell.cell);
                }
                if (i > 0 && cells[row + j - xSize].is) {
                    down = &(cells[row + j - xSize].cell.cell);
                }
                if (j > 0 && cells[row + j - 1].is) {
                    left = &(cells[row + j - 1].cell.cell);
                }

                connectAdjacentCells(upRight, 2, 0, 2, 5 * M_PI_4, center, 1, 5, 1, M_PI_4);
                connectAdjacentCells(upLeft, 3, 1, 3, 7 * M_PI_4, center, 3, 7, 3, 3 * M_PI_4);
                connectAdjacentCells(downLeft, 0, 2, 0, M_PI_4, center, 5, 1, 5, 5 * M_PI_4);
                connectAdjacentCells(downRight, 1, 3, 1, 3 * M_PI_4, center, 7, 3, 7, 7 * M_PI_4);
                connectAdjacentCells(down, 2, 6, 2, M_PI_2, center, 6, 2, 6, 3 * M_PI_2);
                connectAdjacentCells(left, 0, 4, 0, 0.0f, center, 4, 0, 4, M_PI);
            }
        }

        // Handle Edges
        if (cells[truncationCellsStart].is) {
            Cell* cell = &(cells[truncationCellsStart].cell.cell);
            connectAdjacentCells(cell, 2, 0, 2, 5 * M_PI_4, nullptr, 1, 5, 1, M_PI_4);
        }
        if (cells[truncationCellsStart + xSize].is) {
            Cell* cell = &(cells[truncationCellsStart + xSize].cell.cell);
            connectAdjacentCells(cell, 3, 1, 3, 7 * M_PI_4, nullptr, 3, 7, 3, 3 * M_PI_4);
        }
        if (cells[truncationCellsStart + (xSize + 1) * ySize].is) {
            Cell* cell = &(cells[truncationCellsStart + (xSize + 1) * ySize].cell.cell);
            connectAdjacentCells(cell, 1, 3, 1, 3 * M_PI_4, nullptr, 7, 3, 7, 7 * M_PI_4);
        }
        if (cells[truncationCellsStart + (xSize + 1) * ySize + xSize].is) {
            Cell* cell = &(cells[truncationCellsStart + (xSize + 1) * ySize + xSize].cell.cell);
            connectAdjacentCells(cell, 0, 2, 0, M_PI_4, nullptr, 5, 1, 5, 5 * M_PI_4);
        }

        for (unsigned int i = 0; i < ySize; i++) {
            unsigned int row = i * xSize;
            unsigned int row2 = i * (xSize + 1);

            Cell* downLeft = nullptr;
            Cell* upLeft = nullptr;
            
            if (cells[truncationCellsStart + row2].is) {
                downLeft = &(cells[truncationCellsStart + row2].cell.cell);
            }
            if (cells[truncationCellsStart + row2 + (xSize + 1)].is) {
                upLeft = &(cells[truncationCellsStart + row2 + (xSize + 1)].cell.cell);
            }

            connectAdjacentCells(downLeft, 1, 3, 1, 3 * M_PI_4, nullptr, 7, 3, 7, 7 * M_PI_4);
            connectAdjacentCells(upLeft, 2, 0, 2, 5 * M_PI_4, nullptr, 1, 5, 1, M_PI_4);

            Cell* right = nullptr;
            Cell* downRight = nullptr;
            Cell* upRight = nullptr;

            if (cells[row + xSize - 1].is) {
                right = &(cells[row + xSize - 1].cell.cell);
            }
            if (cells[truncationCellsStart + row2 + xSize].is) {
                downRight = &(cells[truncationCellsStart + row2 + xSize].cell.cell);
            }
            if (cells[truncationCellsStart + row2 + xSize + (xSize + 1)].is) {
                upRight = &(cells[truncationCellsStart + row2 + xSize + (xSize + 1)].cell.cell);
            }

            connectAdjacentCells(right, 0, 4, 0, 0.0f, nullptr, 4, 0, 4, M_PI);
            connectAdjacentCells(upRight, 3, 1, 3, 7 * M_PI_4, nullptr, 3, 7, 3, 3 * M_PI_4);
            connectAdjacentCells(downRight, 0, 2, 0, M_PI_4, nullptr, 5, 1, 5, 5 * M_PI_4);
        }

        for (unsigned int j = 0; j < xSize; j++) {

            Cell* downLeft = nullptr;
            Cell* downRight = nullptr;
            
            if (cells[truncationCellsStart + j].is) {
                downLeft = &(cells[truncationCellsStart + j].cell.cell);
            }
            if (cells[truncationCellsStart + j + 1].is) {
                downRight = &(cells[truncationCellsStart + j + 1].cell.cell);
            }

            connectAdjacentCells(downLeft, 3, 1, 3, 7 * M_PI_4, nullptr, 3, 7, 3, 3 * M_PI_4);
            connectAdjacentCells(downRight, 2, 0, 2, 5 * M_PI_4, nullptr, 1, 5, 1, M_PI_4);

            unsigned int row = xSize * (ySize - 1);
            unsigned int row2 = (xSize + 1) * ySize;

            Cell* up = nullptr;
            Cell* upLeft = nullptr;
            Cell* upRight = nullptr;

            if (cells[row + j].is) {
                up = &(cells[row + j].cell.cell);
            }
            if (cells[truncationCellsStart + row2 + j].is) {
                upLeft = &(cells[truncationCellsStart + row2 + j].cell.cell);
            }
            if (cells[truncationCellsStart + row2 + j + 1].is) {
                upRight = &(cells[truncationCellsStart + row2 + j + 1].cell.cell);
            }

            connectAdjacentCells(up, 2, 6, 2, M_PI_2, nullptr, 6, 2, 6, 3 * M_PI_2);
            connectAdjacentCells(upLeft, 0, 2, 0, M_PI_4, nullptr, 5, 1, 5, 5 * M_PI_4);
            connectAdjacentCells(upRight, 1, 3, 1, 3 * M_PI_4, nullptr, 7, 3, 7, 7 * M_PI_4);
        }
    }

    void TruncatedSquareGridBoard::fillCells(std::vector<const Cell*>& cellContainer) const {
        for (unsigned int i = 0; i < cellGridSize; i++) {
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
            return new TruncatedSquareGridBoard(xSize, ySize, (CellColor*)(trueData + 8));
        }
        unsigned int getDataSize(Board* board) const {
            TruncatedSquareGridBoard* actualBoard = (TruncatedSquareGridBoard*)board;
            return sizeof(unsigned int) * 2 + sizeof(CellColor) * actualBoard->cellGridSize;
        }
        void writeData(void* data, Board* board) const {
            TruncatedSquareGridBoard* actualBoard = (TruncatedSquareGridBoard*)board;
            unsigned int offset = 0;
            *(unsigned int*)(data + offset) = actualBoard->xSize;
            offset += sizeof(unsigned int);
            *(unsigned int*)(data + offset) = actualBoard->ySize;
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
    } trueTruncatedSquareGridBoardFactory;

    const BoardFactory* truncatedSquareGridBoardFactory = &trueTruncatedSquareGridBoardFactory;

}
