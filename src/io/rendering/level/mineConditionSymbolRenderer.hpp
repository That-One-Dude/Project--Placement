/******************************************************************************
* File: mineConditionSymbolRenderer.hpp
* Author: Tony Skeps
* Date: November 12, 2023
******************************************************************************/

#include "../../../logic/level/symbols/mineConditionSymbol.hpp"
#include "symbolRenderer.hpp"

#ifndef MINE_CONDITION_SYMBOL_RENDERER_HPP
#define MINE_CONDITION_SYMBOL_RENDERER_HPP

namespace Game::IO::Rendering::Level {

    class MineConditionSymbolRenderer: public GenericSymbolRenderer<Logic::Level::Symbols::MineConditionSymbol> {
    public:
        MineConditionSymbolRenderer(unsigned int texture, unsigned int circleTexture, unsigned int noMineTexture): texture(texture), circleTexture(circleTexture), noMineTexture(noMineTexture) {}
        void init() {}
    protected:
        virtual void genericRenderFloating(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::MineConditionSymbol&);
    private:
        const unsigned int texture;
        const unsigned int circleTexture;
        const unsigned int noMineTexture;
    };

}

#endif