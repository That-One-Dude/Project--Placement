/******************************************************************************
* File: lineDrawerSymbol.hpp
* Author: Tony Skeps
* Date: August 1, 2023
******************************************************************************/

#include <cmath>
#include "symbol.hpp"

#ifndef LINE_DRAWER_SYMBOL_HPP
#define LINE_DRAWER_SYMBOL_HPP

namespace Game::Logic::Level::Symbols {

    class LineDrawerSymbol: public Symbol {
    public:
        LineDrawerSymbol(Boards::CellColor color, bool showSatisfaction, unsigned char direction);
        LineDrawerSymbol(Boards::CellColor color, bool showSatisfaction);
        virtual void onInvoke(BoardState&, const Boards::Cell&);
        virtual void hover(const BoardState&, const Boards::Cell*);
        virtual void onAdjust(const BoardState&, const Boards::Cell*);
        unsigned char direction;
        float angle = M_PI_2;
    protected:
        virtual Symbol* cloneInternal();
    };

    extern const SymbolFactory* lineDrawerFactory;

}

#endif