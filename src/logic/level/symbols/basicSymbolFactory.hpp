/******************************************************************************
* File: basicSymbolFactory.hpp
* Author: Tony Skeps
* Date: August 16, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef BASIC_SYMBOL_FACTORY_HPP
#define BASIC_SYMBOL_FACTORY_HPP

namespace Game::Logic::Level::Symbols {

    template<typename T> class BasicSymbolFactory: public SymbolFactory {
    public:
        Symbol* produce(Boards::CellColor color, bool showSatisfaction, void*) const {
            return new T(color, showSatisfaction);
        }
        unsigned int getDataSize(Symbol* symbol) const {return 0;}
        void writeData(void* data, Symbol* symbol) const {}
    };

}

#endif