/******************************************************************************
* File: groupConditionSymbol.cpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

#include <iostream>

#include <unordered_set>
#include <queue>
#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"
#include "basicSymbolFactory.hpp"
#include "groupConditionSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    GroupConditionSymbol::GroupConditionSymbol(Boards::CellColor symbolColor, bool showSatisfaction): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{}, symbolColor, showSatisfaction) {}

    bool GroupConditionSymbol::isSatisfied(const BoardState& boardState, const Boards::Cell& cell) const {

        std::unordered_set<const Boards::Cell*> searchedCells;
        std::queue<const Boards::Cell*> toSearch;

        toSearch.push(&cell);
        searchedCells.insert(&cell);
        const Boards::Cell* currentSearch;

        while (!toSearch.empty()) {
            currentSearch = toSearch.front();
            toSearch.pop();

            for (auto iter = currentSearch->getAdjacentNeighbors().begin(); iter != currentSearch->getAdjacentNeighbors().end(); iter++) {
                if (iter->cell != nullptr && searchedCells.count(iter->cell) == 0 && boardState.getCellColors().at(&cell).getColor() == boardState.getCellColors().at(iter->cell).getColor()) {
                    toSearch.push(iter->cell);
                    searchedCells.insert(iter->cell);
                }
            }

            if (&cell != currentSearch) {
                std::pair<std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator, std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator> pairs = boardState.getSymbolInstances(currentSearch);

                for (auto iter = pairs.first; iter != pairs.second; iter++) {
                    if (iter->second.symbol.getColor() == getColor()) {
                        return true;
                    }
                }
            }
        }
        

        return false;
    }

    Symbol* GroupConditionSymbol::cloneInternal() {
        return new GroupConditionSymbol(getColor(), showSatisfaction);
    }

    BasicSymbolFactory<GroupConditionSymbol> trueGroupConditionFactory;

    const SymbolFactory* groupConditionFactory = &trueGroupConditionFactory;

}
