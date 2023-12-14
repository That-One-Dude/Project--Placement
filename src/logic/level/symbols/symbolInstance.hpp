/******************************************************************************
* File: symbolInstance.hpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

//#include "../boardState.hpp"
#include "../boardStateClass.hpp"
#include "../boards/cell.hpp"
#include "symbol.hpp"

#ifndef SYMBOL_INSTANCE_HPP
#define SYMBOL_INSTANCE_HPP

namespace Game::Logic::Level::Symbols {

    class SymbolInstance {
    public:
        SymbolInstance(Symbol& symbol, const Boards::Cell& cell, bool preplaced = false): symbol(symbol), cell(cell), preplaced(preplaced) {}
        void updateSatisfaction(const BoardState&);
        bool isSatisfied() const;
        void forceSatisfaction(bool satisfaction) {satisfactionOverridden = true; overridedSatisfaction = satisfaction;}
        void resetSatisfaction() {satisfactionOverridden = false; unoverridedSatisfactionUnknown = true;}
        bool isSatisfactionOverridden() const {return satisfactionOverridden;}
        bool isSatisfiedUnoverrided() const {return unoverridedSatisfaction;}
        Symbol& symbol;
        // should I store this here?
        const Boards::Cell& cell;
        const bool preplaced;
    private:
        bool satisfactionOverridden = false;
        bool overridedSatisfaction;
        bool unoverridedSatisfactionUnknown = true;
        bool unoverridedSatisfaction;
    };

}

#endif