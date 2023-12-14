/******************************************************************************
* File: debugRegionColorerSymbol.cpp
* Author: Tony Skeps
* Date: August 2, 2023
******************************************************************************/

#include <iostream>

#include <unordered_set>
#include <queue>
#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"
#include "debugRegionColorerSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    DebugRegionColorerSymbol::DebugRegionColorerSymbol(Boards::CellColor symbolColor, bool showSatisfaction): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{}, symbolColor, showSatisfaction) {}

    void DebugRegionColorerSymbol::onInvoke(BoardState& boardState, const Boards::Cell& cell) {

        std::unordered_set<const Boards::Cell*> searchedCells;
        std::queue<const Boards::Cell*> toSearch;

        toSearch.push(&cell);
        const Boards::Cell* currentSearch;

        unsigned int i = 1;

        while (!toSearch.empty()) {
            currentSearch = toSearch.front();
            toSearch.pop();

            searchedCells.insert(currentSearch);

            for (auto iter = currentSearch->getAdjacentNeighbors().begin(); iter != currentSearch->getAdjacentNeighbors().end(); iter++) {
                if (iter->cell != nullptr && searchedCells.count(iter->cell) == 0 && boardState.getCellColors().at(&cell).getColor() == boardState.getCellColors().at(iter->cell).getColor()) {
                    toSearch.push(iter->cell);
                    i++;
                }
            }

            /*if (&cell != currentSearch) {
                std::pair<std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator, std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator> pairs = boardState.getSymbolInstances(currentSearch);

                for (auto iter = pairs.first; iter != pairs.second; iter++) {
                    if (iter->second.symbol.getColor() == getColor()) {
                        
                    }
                }
            }*/
        }

        for (auto iter = searchedCells.begin(); iter != searchedCells.end(); iter++) {
            boardState.setCellColor(*iter, getColor());
        }
    }

}
