/******************************************************************************
* File: level.cpp
* Author: Tony Skeps
* Date: April 20, 2023
******************************************************************************/

#include <iostream>

#include "../../universalData.hpp"
#include "symbols/symbolInstance.hpp"
#include "boards/cellInstance.hpp"

#include "level.hpp"

namespace Game::Logic::Level {

    Level::Level(const LevelData* levelData): levelData(levelData) {
        const std::vector<const Boards::Cell*>& cells = levelData->board->getCells();

        for (auto iter = cells.begin(); iter != cells.end(); iter++) {
            //boardState.cellColors[&(*iter)] = iter->getBaseColor();
            //boardState.cellColors.insert({{&(*iter), Boards::CellInstance(iter->getBaseColor())}});
            boardState.cellColors.insert({{*iter, Boards::CellInstance((*iter)->getBaseColor())}});
            Boards::CellInstance& cell = boardState.cellColors.at(*iter);
            cellColorOrder.push_back(&(cell.getColor()));
        }

        for (int i = 0; i < levelData->numPreplacedSymbols; i++) {
            const Boards::Cell* cell = cells.at(levelData->preplacedSymbols[i].cell);
            Symbols::Symbol& symbol = *(levelData->preplacedSymbols[i].symbol);
            auto elem = boardState.symbolInstances.insert(std::pair<const Boards::Cell*, Symbols::SymbolInstance>(cell, Symbols::SymbolInstance(symbol, *cell, true)));
            boardState.symbolOrder.push_back(&(elem->second));
            symbol.onPlace(boardState, *cell);
        }

        for (int i = 0; i < levelData->numPlaceableSymbols; i++) {
            GL::vec2 offset = levelData->placeableSymbols[i].initialOffset;
            GL::vec2 direction = levelData->placeableSymbols[i].initialDirection;
            for (int j = 0; j < levelData->placeableSymbols[i].count; j++) {
                //symbolsToPlace.push_back(PlacementData{2.0f * j + 1.0f, 2.0f * i - (levelData->numPlaceableSymbols - 1), levelData->placeableSymbols[i].symbol->clone()});
                symbolsToPlace.push_back(PlacementData{offset.x, offset.y, levelData->placeableSymbols[i].symbol->clone()});
                offset = offset + direction;
                direction = levelData->placeableSymbols[i].recursiveTransformation * direction;
            }
        }

        std::cout << recalculate() << std::endl;
        
    }

    PlaceResult Level::placeSymbol(const Boards::Cell* cell) {
        if (cell != nullptr) {
            if (boardState.getSymbolInstances().count(cell) > 0) {
                return PlaceResult::FAILED;
            }
            // Curious
            Symbols::Symbol& symbol = *(currentlyPickedUpSymbol->symbol);
            currentlyPickedUpSymbol = nullptr;
            symbolsToPlace.erase(location);
            //boardState.symbolInstances.push_back(Symbols::SymbolInstance(symbol, cell));
            auto elem = boardState.symbolInstances.insert(std::pair<const Boards::Cell*, Symbols::SymbolInstance>(cell, Symbols::SymbolInstance(symbol, *cell)));
            boardState.symbolOrder.push_back(&(elem->second));
            symbol.onPlace(boardState, *cell);
            // I could do this instead of the line below, but then there would be bugs with copy paste symbols and symbols that override other symbols
            // symbol->onInvoke(boardState);
            // return isSatisfied();
            bool result = recalculate();
            if (result) {
                return PlaceResult::SUCCESS;
            } else {
                return PlaceResult::SUCCESS_UNSATISFIED_BOARD;
            }
        } else {
            currentlyPickedUpSymbol = nullptr;
            return PlaceResult::SUCCESS_UNSATISFIED_BOARD;
        }
    }

    PlacementData* Level::pickUpSymbol(const Symbols::Symbol* symbol) {

        // TODO: Figure out removal
        /*for (auto iter = boardState.symbolInstances.begin(); iter != boardState.symbolInstances.end(); iter++) {
            if (&(iter->symbol) == symbol) {
                boardState.symbolInstances.erase(iter);
                break;
            }
        }*/
        for (auto iter = boardState.symbolInstances.begin(); iter != boardState.symbolInstances.end(); iter++) {
            if (&(iter->second.symbol) == symbol) {
                if (iter->second.preplaced) {
                    return nullptr;
                }
                Symbols::Symbol* trueSymbol = &(iter->second.symbol);
                trueSymbol->onRemove(boardState, *(iter->first));
                boardState.symbolInstances.erase(iter);
                for (auto iter2 = boardState.symbolOrder.begin(); iter2 != boardState.symbolOrder.end(); iter2++) {
                    if (&((*iter2)->symbol) == symbol) {
                        boardState.symbolOrder.erase(iter2);
                        break;
                    }
                }
                recalculate();
                symbolsToPlace.push_back(PlacementData{0.0f, 0.0f, trueSymbol});
                location = symbolsToPlace.end() - 1;
                currentlyPickedUpSymbol = &*location;
                return currentlyPickedUpSymbol;
            }
        }
        for (auto iter = symbolsToPlace.begin(); iter != symbolsToPlace.end(); iter++) {
            if (iter->symbol == symbol) {
                PlacementData toMovePlacementData = *iter;
                symbolsToPlace.erase(iter);
                symbolsToPlace.push_back(toMovePlacementData);
                location = symbolsToPlace.end() - 1;
                currentlyPickedUpSymbol = &*location;
                return currentlyPickedUpSymbol;
            }
        }
        return nullptr;
    }

    void Level::hoverSymbol(const Boards::Cell* cell) {
        currentlyPickedUpSymbol->symbol->hover(boardState, cell);
    }

    void Level::adjustSymbol(const Boards::Cell* cell) {
        currentlyPickedUpSymbol->symbol->onAdjust(boardState, cell);
    }

    bool Level::recalculate() {
        for (auto iter = boardState.cellColors.begin(); iter != boardState.cellColors.end(); iter++) {
            iter->second.reset();
        }
        for (auto iter = boardState.symbolOrder.begin(); iter != boardState.symbolOrder.end(); iter++) {
            (*iter)->resetSatisfaction();
        }
        for (auto iter = boardState.symbolOrder.begin(); iter != boardState.symbolOrder.end(); iter++) {
            (*iter)->symbol.onInvoke(boardState, (*iter)->cell);
            update();
        }
        for (auto iter = boardState.symbolOrder.begin(); iter != boardState.symbolOrder.end(); iter++) {
            (*iter)->symbol.onLateInvoke(boardState, (*iter)->cell);
        }
        for (auto iter = boardState.symbolOrder.begin(); iter != boardState.symbolOrder.end(); iter++) {
            (*iter)->updateSatisfaction(boardState);
        }
        return isSatisfied();
    }

    void Level::update() {
        while (!boardState.updatedCells.empty()) {
            std::unordered_set<const Boards::Cell*> updatedCells = boardState.updatedCells;
            boardState.updatedCells.clear();
            for (auto iter = boardState.symbolOrder.begin(); iter != boardState.symbolOrder.end(); iter++) {
                (*iter)->symbol.onUpdate(boardState, (*iter)->cell, updatedCells);
            }
        }
    }

    bool Level::isSatisfied() const {
        bool result = symbolsToPlace.empty() && currentlyPickedUpSymbol == nullptr;
        for (auto iter = boardState.symbolOrder.begin(); iter != boardState.symbolOrder.end(); iter++) {
            bool check = (*iter)->isSatisfied();
            result = result && check;
        }
        return result;
    }

}
