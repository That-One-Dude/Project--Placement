/******************************************************************************
* File: boardFactory.cpp
* Author: Tony Skeps
* Date: August 14, 2023
******************************************************************************/

#include "boardFactory.hpp"

namespace Game::Logic::Level::Boards {

    CompositeBoardFactory::~CompositeBoardFactory() {
        /*for (auto iter = factories.begin(); iter != factories.end(); iter++) {
            delete iter->second;
        }*/
    }

    void CompositeBoardFactory::addFactory(NamespacedId id, const BoardFactory* factory) {
        factories[id] = factory;
    }

    Board* CompositeBoardFactory::createBoard(NamespacedId id, void* data) const {

        Board* result = factories.at(id)->produce(data);

        result->fillCells();
        result->setRendererId(id);

        return result;

    }

    unsigned int CompositeBoardFactory::getDataSize(Board* symbol) const {
        return factories.at(symbol->getRendererId())->getDataSize(symbol);
    }

    void CompositeBoardFactory::writeData(void* data, Board* symbol) const {
        factories.at(symbol->getRendererId())->writeData(data, symbol);
    }

}
