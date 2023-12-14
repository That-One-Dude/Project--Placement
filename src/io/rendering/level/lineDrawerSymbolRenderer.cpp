/******************************************************************************
* File: lineDrawerSymbolRenderer.cpp
* Author: Tony Skeps
* Date: August 17, 2023
******************************************************************************/

#include "lineDrawerSymbolRenderer.hpp"

namespace Game::IO::Rendering::Level {

    void LineDrawerSymbolRenderer::genericRender(const Logic::Level::BoardState& board, const Logic::Level::Symbols::SymbolInstance& symbolInstance, const Logic::Level::Symbols::LineDrawerSymbol& symbol) {
        float angle = symbolInstance.cell.getAdjacentNeighbors().at(symbol.direction).rotation;

        mat4 matrix = rotationZ(angle);
        drawTexture(matrix, texture);
    }

    void LineDrawerSymbolRenderer::genericRenderFloating(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::LineDrawerSymbol& symbol) {
        mat4 matrix = rotationZ(symbol.angle);
        
        drawTexture(matrix, texture);
    }

}
