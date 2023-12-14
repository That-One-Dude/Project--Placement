/******************************************************************************
* File: singleTilePainterSymbol.cpp
* Author: Tony Skeps
* Date: December 10, 2023
******************************************************************************/

#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"
#include "basicSymbolFactory.hpp"
#include "singleTilePainterSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    SingleTilePainterSymbol::SingleTilePainterSymbol(Boards::CellColor symbolColor, bool showSatisfaction): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{}, symbolColor, showSatisfaction) {}

    void SingleTilePainterSymbol::onInvoke(BoardState& boardState, const Boards::Cell& cell) {
        //boardState.cellColors.at(&cell) = symbolColor;
        Boards::CellColor color = getColor();
        if (color != Boards::nocell) {
            boardState.setCellColor(&cell, color);
        }
        boardState.lockCellColor(&cell);
    }

    Symbol* SingleTilePainterSymbol::cloneInternal() {
        return new SingleTilePainterSymbol(getColor(), showSatisfaction);
    }

    BasicSymbolFactory<SingleTilePainterSymbol> trueSingleTilePainterFactory;

    const SymbolFactory* singleTilePainterFactory = &trueSingleTilePainterFactory;

}