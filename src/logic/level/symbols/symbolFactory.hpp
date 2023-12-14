/******************************************************************************
* File: symbolFactory.hpp
* Author: Tony Skeps
* Date: August 14, 2023
******************************************************************************/

#include <unordered_map>
#include "../../../namespacedId.hpp"
#include "symbol.hpp"

#ifndef SYMBOL_FACTORY_HPP
#define SYMBOL_FACTORY_HPP

namespace Game::Logic::Level::Symbols {

    class CompositeSymbolFactory {
    public:
        CompositeSymbolFactory() {}
        ~CompositeSymbolFactory();
        void addFactory(NamespacedId, const SymbolFactory*);
        Symbol* createSymbol(NamespacedId, Boards::CellColor color, bool showSatisfaction, void*) const;
        Symbol* createPlaceableSymbol(NamespacedId, Boards::CellColor color, bool showSatisfaction, void*) const;
        // Saving stuff
        unsigned int getDataSize(Symbol* symbol) const;
        unsigned int getPlaceableDataSize(Symbol* symbol) const;
        void writeData(void* data, Symbol* symbol) const;
        void writePlaceableData(void* data, Symbol* symbol) const;
    private:
        std::unordered_map<NamespacedId, const SymbolFactory*, NamespacedIdHash> factories;
    };

}

#endif