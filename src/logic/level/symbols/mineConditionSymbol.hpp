/******************************************************************************
* File: mineConditionSymbol.hpp
* Author: Tony Skeps
* Date: September 30, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef MINE_CONDITION_SYMBOL_HPP
#define MINE_CONDITION_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class MineConditionSymbol: public Symbol {
    public:
        MineConditionSymbol(Boards::CellColor, bool showSatisfaction, unsigned int numMines);
        virtual bool isSatisfied(const BoardState&, const Boards::Cell& cell) const;
        unsigned int numMines;
    protected:
        virtual Symbol* cloneInternal();
    };

    extern const SymbolFactory* mineConditionFactory;

}

#endif