/******************************************************************************
* File: debugRegionColorerSymbol.hpp
* Author: Tony Skeps
* Date: August 2, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef DEBUG_REGION_COLORER_SYMBOL_HPP
#define DEBUG_REGION_COLORER_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class DebugRegionColorerSymbol: public Symbol {
    public:
        DebugRegionColorerSymbol(Boards::CellColor, bool showSatisfaction);
        virtual void onInvoke(BoardState&, const Boards::Cell&);
    };

}

#endif