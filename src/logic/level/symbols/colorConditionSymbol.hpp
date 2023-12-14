/******************************************************************************
* File: colorConditionSymbol.hpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef COLOR_CONDITION_SYMBOL_HPP
#define COLOR_CONDITION_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class ColorConditionSymbol: public Symbol {
    public:
        ColorConditionSymbol(Boards::CellColor, bool showSatisfaction);
        virtual bool isSatisfied(const BoardState&, const Boards::Cell& cell) const;
    protected:
        virtual Symbol* cloneInternal();
    };

    extern const SymbolFactory* colorConditionFactory;

}

#endif