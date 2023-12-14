/******************************************************************************
* File: painterSymbol.cpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"
#include "basicSymbolFactory.hpp"
#include "painterSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    PainterSymbol::PainterSymbol(Boards::CellColor symbolColor, bool showSatisfaction): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{}, symbolColor, showSatisfaction) {}

    void PainterSymbol::onInvoke(BoardState& boardState, const Boards::Cell& cell) {
        //boardState.cellColors.at(&cell) = symbolColor;
        Boards::CellColor color = getColor();
        if (color != Boards::nocell) {
            boardState.setCellColor(&cell, color);
            boardState.lockCellColor(&cell);
        } else {
            color = boardState.getCellColors().at(&cell).getColor();
        }
        for (auto iter = cell.getNeighbors().cbegin(); iter != cell.getNeighbors().cend(); iter++) {
            if (iter->cell != nullptr) {
                //boardState.cellColors.at(iter->cell) = symbolColor;
                boardState.setCellColor(iter->cell, color);
            }
        }
    }

    Symbol* PainterSymbol::cloneInternal() {
        return new PainterSymbol(getColor(), showSatisfaction);
    }

    /*class: public SymbolFactory {
    public:
        Symbol* produce(Boards::CellColor color, void*) const {
            return new PainterSymbol(color);
        }
    } value;*/

    BasicSymbolFactory<PainterSymbol> truePainterFactory;

    const SymbolFactory* painterFactory = &truePainterFactory;

}