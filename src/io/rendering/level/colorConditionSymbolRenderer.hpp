/******************************************************************************
* File: colorConditionSymbolRenderer.hpp
* Author: Tony Skeps
* Date: December 10, 2023
******************************************************************************/

#include "../../../logic/level/symbols/colorConditionSymbol.hpp"
#include "symbolRenderer.hpp"

#ifndef COLOR_CONDITION_SYMBOL_RENDERER_HPP
#define COLOR_CONDITION_SYMBOL_RENDERER_HPP

namespace Game::IO::Rendering::Level {

    class ColorConditionSymbolRenderer: public GenericSymbolRenderer<Logic::Level::Symbols::ColorConditionSymbol> {
    public:
        ColorConditionSymbolRenderer(unsigned int texture, unsigned int noCellTexture): texture(texture), noCellTexture(noCellTexture) {}
        void init() {}
    protected:
        void genericRenderFloating(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::ColorConditionSymbol&);
    private:
        const unsigned int texture;
        const unsigned int noCellTexture;
    };

}

#endif