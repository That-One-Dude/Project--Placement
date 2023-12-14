/******************************************************************************
* File: cell.hpp
* Author: Tony Skeps
* Date: June 27, 2023
******************************************************************************/

#include <vector>

#ifndef CELL_HPP
#define CELL_HPP

namespace Game::Logic::Level::Boards {

    class Cell;

    struct CellNeighbor {
        CellNeighbor(): cell(nullptr), continuedDirection(-1), rotation(0.0f) {}
        CellNeighbor(Cell* cell, unsigned char continuedDirection, float rotation): continuedDirection(continuedDirection), cell(cell), rotation(rotation) {}
        Cell* cell; // nullptr for end of the road
        unsigned char continuedDirection;
        float rotation;
    };

    using CellColor = unsigned char;
    const CellColor nocell = -1;
    
    void connectCells(Cell* left, unsigned char leftConnectionPoint, unsigned char leftContinuedDirection, float leftRotation, Cell* right, unsigned int rightConnectionPoint, unsigned char rightContinuedDirection, float rightRotation);
    void connectAdjacentCells(Cell* left, unsigned char leftConnectionPoint, unsigned char leftContinuedDirection, unsigned char leftAdjacentConnectionPoint, float leftRotation, Cell* right, unsigned int rightConnectionPoint, unsigned char rightContinuedDirection, unsigned char rightAdjacentConnectionPoint, float rightRotation);

    class Cell {
        friend void connectCells(Cell* left, unsigned char leftConnectionPoint, unsigned char leftContinuedDirection, float leftRotation, Cell* right, unsigned int rightConnectionPoint, unsigned char rightContinuedDirection, float rightRotation);
        friend void connectAdjacentCells(Cell* left, unsigned char leftConnectionPoint, unsigned char leftContinuedDirection, unsigned char leftAdjacentConnectionPoint, float leftRotation, Cell* right, unsigned int rightConnectionPoint, unsigned char rightContinuedDirection, unsigned char rightAdjacentConnectionPoint, float rightRotation);
    public:
        Cell(CellColor baseColor, unsigned char numNeighbors, unsigned char numAdjacentNeighbors): baseColor(baseColor), neighbors(numNeighbors, CellNeighbor()), adjacentNeighbors(numAdjacentNeighbors, CellNeighbor()) {}
        unsigned char getBaseColor() const {return baseColor;}
        const std::vector<CellNeighbor>& getNeighbors() const {return neighbors;}
        const std::vector<CellNeighbor>& getAdjacentNeighbors() const {return adjacentNeighbors;}
    private:
        unsigned char baseColor;
        std::vector<CellNeighbor> neighbors; // the order is counterclockwise
        std::vector<CellNeighbor> adjacentNeighbors; // the continuedDirection for this is in reference to neighbors. Not adjacentNeighbors
    };

    /*struct CellInstance {
        CellColor color;
        const Cell* cell;
    };*/

}

#endif