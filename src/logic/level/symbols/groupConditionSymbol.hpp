/******************************************************************************
* File: groupConditionSymbol.hpp
* Author: Tony Skeps
* Date: August 1, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef GROUP_CONDITION_SYMBOL_HPP
#define GROUP_CONDITION_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class GroupConditionSymbol: public Symbol {
    public:
        GroupConditionSymbol(Boards::CellColor, bool showSatisfaction);
        virtual bool isSatisfied(const BoardState&, const Boards::Cell& cell) const;
    protected:
        virtual Symbol* cloneInternal();
    };

    extern const SymbolFactory* groupConditionFactory;

}

#endif