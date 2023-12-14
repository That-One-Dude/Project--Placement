/******************************************************************************
* File: copyRegionConditionSymbol.cpp
* Author: Tony Skeps
* Date: August 30, 2023
******************************************************************************/

#include <unordered_set>
#include <queue>
#include <stack>
#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"

#include "copyRegionConditionSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    CopyRegionConditionSymbol::CopyRegionConditionSymbol(Boards::CellColor color, bool showSatisfaction, CellTree cellTree): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{NamespacedId("copy_region")}, color, showSatisfaction), cellTree(cellTree) {

    }

    bool CopyRegionConditionSymbol::isSatisfied(const BoardState& boardState, const Boards::Cell& cell) const {
        
        // Step 1: collect every valid starting point and every other copy region condition symbol here
        std::unordered_set<const Boards::Cell*> searchedCells;
        std::queue<const Boards::Cell*> toSearch;

        std::unordered_multimap<unsigned int, const Boards::Cell*> cellList;
        std::vector<const CopyRegionConditionSymbol*> otherSymbols;
        std::unordered_set<const Boards::Cell*> cellsToInclude;
        otherSymbols.push_back(this);

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

            cellList.insert({currentSearch->getAdjacentNeighbors().size(), currentSearch});
            cellsToInclude.insert(currentSearch);

            if (&cell != currentSearch) {
                std::pair<std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator, std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator> pairs = boardState.getSymbolInstances(currentSearch);

                for (auto iter = pairs.first; iter != pairs.second; iter++) {
                    if (iter->second.symbol.getTags().count(NamespacedId("copy_region")) > 0) {
                        otherSymbols.push_back((const CopyRegionConditionSymbol*)&(iter->second.symbol));
                    }
                }
            }
        }

        // Step 2 ... Search through all possible arrangements to see if they fit?

        std::stack<std::unordered_set<const Boards::Cell*>> markings;
        auto iter = otherSymbols.begin();

        while (iter != otherSymbols.begin() - 1) {
            // Step 2a. Check if we are needed
            markings.push({});
        }

        return false;
    }

    std::vector<CopyRegionConditionSymbol*> CopyRegionConditionSymbol::satisfactionRecursionHelper(const CopyRegionConditionSymbol* symbol, std::unordered_set<const Boards::Cell*> cells, std::vector<CopyRegionConditionSymbol*> otherSymbols, const std::unordered_multimap<unsigned int, const Boards::Cell*>& cellList) {
        // Try all possible initial locations

        auto iter = cellList.find(symbol->cellTree.numEdges);

        for (unsigned int i = 0; i < cellList.count(symbol->cellTree.numEdges); i++, iter++) {
            // Try all possible rotations
            for (unsigned int j = 0; j < symbol->cellTree.numEdges; j++) {

            }
        }

        // Try without this symbol
    }

    Symbol* CopyRegionConditionSymbol::cloneInternal() {
        return nullptr;
    }

}
