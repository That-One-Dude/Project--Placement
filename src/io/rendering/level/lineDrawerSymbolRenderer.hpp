/******************************************************************************
* File: lineDrawerSymbolRenderer.hpp
* Author: Tony Skeps
* Date: August 17, 2023
******************************************************************************/

#include "../../../logic/level/symbols/lineDrawerSymbol.hpp"
#include "symbolRenderer.hpp"

#ifndef LINE_DRAWER_SYMBOL_RENDERER_HPP
#define LINE_DRAWER_SYMBOL_RENDERER_HPP

namespace Game::IO::Rendering::Level {

    class LineDrawerSymbolRenderer: public GenericSymbolRenderer<Logic::Level::Symbols::LineDrawerSymbol> {
    public:
        LineDrawerSymbolRenderer(unsigned int texture): texture(texture) {}
        void init() {}
    protected:
        void genericRender(const Logic::Level::BoardState&, const Logic::Level::Symbols::SymbolInstance&, const Logic::Level::Symbols::LineDrawerSymbol&);
        void genericRenderFloating(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::LineDrawerSymbol&);
    private:
        const unsigned int texture;
    };

}

#endif