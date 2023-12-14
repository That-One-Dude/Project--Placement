/******************************************************************************
* File: lineDrawerSymbol.cpp
* Author: Tony Skeps
* Date: August 1, 2023
******************************************************************************/

#include <iostream>

#include <cmath>
#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"
#include "lineDrawerSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    LineDrawerSymbol::LineDrawerSymbol(Boards::CellColor color, bool showSatisfaction): LineDrawerSymbol(color, showSatisfaction, 0) {

    }

    LineDrawerSymbol::LineDrawerSymbol(Boards::CellColor color, bool showSatisfaction, unsigned char direction): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{}, color, showSatisfaction), direction(direction) {

    }

    void LineDrawerSymbol::onInvoke(BoardState& state, const Boards::Cell& cell) {
        Boards::CellColor color = getColor();

        Boards::CellNeighbor currentNeighbors = cell.getAdjacentNeighbors().at(direction);
        while (currentNeighbors.cell != nullptr) {
            if (color != Boards::nocell) {
                if (!state.setCellColor(currentNeighbors.cell, color)) {
                    color = state.getCellColors().at(currentNeighbors.cell).getColor();
                }
            } else if (state.getCellColors().at(currentNeighbors.cell).getColor() != state.getCellColors().at(&cell).getColor()) {
                color = state.getCellColors().at(currentNeighbors.cell).getColor();
            }
            if (currentNeighbors.cell == &cell) {
                break;
            }
            currentNeighbors = currentNeighbors.cell->getNeighbors().at(currentNeighbors.continuedDirection);
        }
    }

    void LineDrawerSymbol::hover(const BoardState&, const Boards::Cell* cell) {
        if (cell != nullptr) {
            float closestAngle;
            float minimumDiff = 10.0f;
            unsigned char i = 0;
            for (auto iter = cell->getAdjacentNeighbors().begin(); iter != cell->getAdjacentNeighbors().end(); iter++, i++) {
                float testDiff = fabsf(iter->rotation - angle);
                if (testDiff > M_PI) testDiff = M_PI * 2 - testDiff;
                if (testDiff < minimumDiff) {
                    direction = i;
                    closestAngle = iter->rotation;
                    minimumDiff = testDiff;
                }
            }
            angle = closestAngle;
        }
    }
    
    void LineDrawerSymbol::onAdjust(const BoardState&, const Boards::Cell* cell) {
        if (cell != nullptr) {
            direction++;
            if (direction >= cell->getAdjacentNeighbors().size()) {
                direction = 0;
            }
            angle = cell->getAdjacentNeighbors().at(direction).rotation;
        }
    }

    Symbol* LineDrawerSymbol::cloneInternal() {
        return new LineDrawerSymbol(getColor(), showSatisfaction);
    }

    class: public SymbolFactory {
    public:
        Symbol* produce(Boards::CellColor color, bool showSatisfaction, void* data) const {
            return new LineDrawerSymbol(color, showSatisfaction, *(unsigned char*)data);
        }
        Symbol* producePlaceable(Boards::CellColor color, bool showSatisfaction, void*) const {
            return new LineDrawerSymbol(color, showSatisfaction);
        }
        unsigned int getDataSize(Symbol*) const {
            return sizeof(unsigned char);
        }
        unsigned int getPlaceableDataSize(Symbol*) const {
            return 0;
        }
        virtual void writeData(void* data, Symbol* symbol) const {
            LineDrawerSymbol* trueSymbol = (LineDrawerSymbol*)symbol;
            *(unsigned char*)data = trueSymbol->direction;
        }
        virtual void writePlaceableData(void* data, Symbol* symbol) const {

        }
    } trueLineDrawererFactory;

    const SymbolFactory* lineDrawerFactory = &trueLineDrawererFactory;

}
