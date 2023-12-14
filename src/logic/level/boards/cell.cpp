/******************************************************************************
* File: cell.cpp
* Author: Tony Skeps
* Date: July 1, 2023
******************************************************************************/

#include "cell.hpp"

namespace Game::Logic::Level::Boards {

    void connectCells(Cell* left, unsigned char leftConnectionPoint, unsigned char leftContinuedDirection, float leftRotation, Cell* right, unsigned int rightConnectionPoint, unsigned char rightContinuedDirection, float rightRotation) {
        if (left != nullptr) {
            left->neighbors[leftConnectionPoint].cell = right;
            left->neighbors[leftConnectionPoint].continuedDirection = rightContinuedDirection;
            left->neighbors[leftConnectionPoint].rotation = leftRotation;
        }
        if (right != nullptr) {
            right->neighbors[rightConnectionPoint].cell = left;
            right->neighbors[rightConnectionPoint].continuedDirection = leftContinuedDirection;
            right->neighbors[rightConnectionPoint].rotation = rightRotation;
        }
    }

    void connectAdjacentCells(Cell* left, unsigned char leftConnectionPoint, unsigned char leftContinuedDirection, unsigned char leftAdjacentConnectionPoint, float leftRotation, Cell* right, unsigned int rightConnectionPoint, unsigned char rightContinuedDirection, unsigned char rightAdjacentConnectionPoint, float rightRotation) {
        connectCells(left, leftConnectionPoint, leftContinuedDirection, leftRotation, right, rightConnectionPoint, rightContinuedDirection, rightRotation);
        if (left != nullptr) {
            left->adjacentNeighbors[leftAdjacentConnectionPoint].cell = right;
            left->adjacentNeighbors[leftAdjacentConnectionPoint].continuedDirection = rightContinuedDirection;
            left->adjacentNeighbors[leftAdjacentConnectionPoint].rotation = leftRotation;
        }
        if (right != nullptr) {
            right->adjacentNeighbors[rightAdjacentConnectionPoint].cell = left;
            right->adjacentNeighbors[rightAdjacentConnectionPoint].continuedDirection = leftContinuedDirection;
            right->adjacentNeighbors[rightAdjacentConnectionPoint].rotation = rightRotation;
        }
    }

}