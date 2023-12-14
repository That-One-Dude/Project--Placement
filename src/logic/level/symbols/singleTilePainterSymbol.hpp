/******************************************************************************
* File: singleTilePainterSymbol.hpp
* Author: Tony Skeps
* Date: December 10, 2023
******************************************************************************/

#include "symbol.hpp"

#ifndef SINGLE_TILE_PAINTER_SYMBOL_HPP
#define SINGLE_TILE_PAINTER_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class SingleTilePainterSymbol: public Symbol {
    public:
        SingleTilePainterSymbol(Boards::CellColor, bool showSatisfaction);
        virtual void onInvoke(BoardState&, const Boards::Cell&);
    protected:
        virtual Symbol* cloneInternal();
    };

    extern const SymbolFactory* singleTilePainterFactory;

}

#endif
