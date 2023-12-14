/******************************************************************************
* File: colorConditionSymbolRenderer.cpp
* Author: Tony Skeps
* Date: December 10, 2023
******************************************************************************/

#include "colorConditionSymbolRenderer.hpp"

namespace Game::IO::Rendering::Level {

    void ColorConditionSymbolRenderer::genericRenderFloating(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::ColorConditionSymbol& symbol) {
        if (symbol.getColor() == Logic::Level::Boards::nocell) {
            drawTexture(noCellTexture);
        } else {
            drawTexture(texture);
        }
    }

}