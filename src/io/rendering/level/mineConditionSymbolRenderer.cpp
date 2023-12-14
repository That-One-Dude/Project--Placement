/******************************************************************************
* File: mineConditionSymbolRenderer.cpp
* Author: Tony Skeps
* Date: November 12, 2023
******************************************************************************/

#include <iostream>

#include <cmath>
#include "mineConditionSymbolRenderer.hpp"

namespace Game::IO::Rendering::Level {

    void MineConditionSymbolRenderer::genericRenderFloating(const Logic::Level::BoardState& board, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::MineConditionSymbol& symbol) {

        if (symbol.numMines > 0) {
            
            drawTexture(circleTexture);

            if (symbol.numMines == 1) {
                drawTexture(texture);
            } else {
                float count = symbol.numMines;
                if (count < 6) count = (count / 2) + 3;
                GL::mat4 offset(
                    3.0f / count, 0.0f, 0.0f, 0.0f,
                    0.0f, 3.0f / count, 0.0f, 0.71875f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                );
                GL::mat4 rotation = GL::rotationZ(2 * M_PI / symbol.numMines);
                for (int i = 0; i < symbol.numMines; i++) {
                    drawTexture(offset, texture);
                    offset = rotation * offset;
                }
            }
        } else {
            drawTexture(noMineTexture);
        }
    }

}
