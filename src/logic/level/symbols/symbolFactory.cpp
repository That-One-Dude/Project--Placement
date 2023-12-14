/******************************************************************************
* File: symbolFactory.cpp
* Author: Tony Skeps
* Date: August 14, 2023
******************************************************************************/

#include "symbolFactory.hpp"

namespace Game::Logic::Level::Symbols {

    CompositeSymbolFactory::~CompositeSymbolFactory() {
        /*for (auto iter = factories.begin(); iter != factories.end(); iter++) {
            delete iter->second;
        }*/
    }

    void CompositeSymbolFactory::addFactory(NamespacedId id, const SymbolFactory* factory) {
        factories[id] = factory;
    }

    Symbol* CompositeSymbolFactory::createSymbol(NamespacedId id, Boards::CellColor color, bool showSatisfaction, void* data) const {

        Symbol* result = factories.at(id)->produce(color, showSatisfaction, data);

        result->setRendererId(id);

        return result;

    }

    Symbol* CompositeSymbolFactory::createPlaceableSymbol(NamespacedId id, Boards::CellColor color, bool showSatisfaction, void* data) const {

        Symbol* result = factories.at(id)->producePlaceable(color, showSatisfaction, data);

        result->setRendererId(id);

        return result;

    }

    unsigned int CompositeSymbolFactory::getDataSize(Symbol* symbol) const {
        return factories.at(symbol->getRendererId())->getDataSize(symbol);
    }

    unsigned int CompositeSymbolFactory::getPlaceableDataSize(Symbol* symbol) const {
        return factories.at(symbol->getRendererId())->getPlaceableDataSize(symbol);
    }

    void CompositeSymbolFactory::writeData(void* data, Symbol* symbol) const {
        factories.at(symbol->getRendererId())->writeData(data, symbol);
    }

    void CompositeSymbolFactory::writePlaceableData(void* data, Symbol* symbol) const {
        factories.at(symbol->getRendererId())->writePlaceableData(data, symbol);
    }

}
