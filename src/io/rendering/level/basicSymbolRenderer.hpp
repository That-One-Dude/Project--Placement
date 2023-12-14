/******************************************************************************
* File: basicSymbolRenderer.hpp
* Author: Tony Skeps
* Date: August 13, 2023
******************************************************************************/

#include "symbolRenderer.hpp"

#ifndef BASIC_SYMBOL_RENDERER_HPP
#define BASIC_SYMBOL_RENDERER_HPP

namespace Game::IO::Rendering::Level {

    class BasicSymbolRenderer: public SymbolRenderer {
    public:
        BasicSymbolRenderer(unsigned int texture): texture(texture) {}
        void init() {}
    protected:
        void renderInternal(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::Symbol&) {
            drawTexture(texture);
        }
    private:
        const unsigned int texture;
    };

}

#endif