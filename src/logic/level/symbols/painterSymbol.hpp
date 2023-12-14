/******************************************************************************
* File: painterSymbol.hpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef PAINTER_SYMBOL_HPP
#define PAINTER_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class PainterSymbol: public Symbol {
    public:
        PainterSymbol(Boards::CellColor, bool showSatisfaction);
        virtual void onInvoke(BoardState&, const Boards::Cell&);
    protected:
        virtual Symbol* cloneInternal();
    };

    extern const SymbolFactory* painterFactory;

}

#endif