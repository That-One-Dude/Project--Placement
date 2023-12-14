/******************************************************************************
* File: symbolRenderer.hpp
* Author: Tony Skeps
* Date: August 2, 2023
******************************************************************************/

#include "../../../gl/glMath.hpp"
#include "../../../gl/glFunctions.hpp"
#include "../../../logic/level/boards/board.hpp"
#include "../../../logic/level/symbols/symbolInstance.hpp"

using namespace Game::GL;

#ifndef SYMBOL_RENDERER_HPP
#define SYMBOL_RENDERER_HPP

namespace Game::IO::Rendering::Level {

    extern int correctTexture, wrongTexture;

    void initSymbols();
    void bindSymbolStuff();

    class SymbolRenderer {
    public:
        virtual ~SymbolRenderer() {}
        virtual void init() = 0;
        void render(const Logic::Level::BoardState&, const Logic::Level::Symbols::SymbolInstance&, mat4);
        void renderFloating(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::Symbol&, mat4);
    protected:
        virtual void renderInternal(const Logic::Level::BoardState& state, const Logic::Level::Symbols::SymbolInstance& symbolInstance) {
            renderInternal(state, &symbolInstance.cell, symbolInstance.symbol);
        }
        virtual void renderInternal(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const Logic::Level::Symbols::Symbol&) = 0;
        void drawTexture(mat4, unsigned int texture);
        void drawTexture(unsigned int texture);
    };

    template <typename T> class GenericSymbolRenderer: public SymbolRenderer {
    protected:
        virtual void genericRender(const Logic::Level::BoardState& state, const Logic::Level::Symbols::SymbolInstance& symbolInstance, const T& symbol) {genericRenderFloating(state, &(symbolInstance.cell), symbol);}
        virtual void genericRenderFloating(const Logic::Level::BoardState&, const Logic::Level::Boards::Cell*, const T&) = 0;
        void renderInternal(const Logic::Level::BoardState& boardState, const Logic::Level::Symbols::SymbolInstance& symbolInstance) {
            genericRender(boardState, symbolInstance, static_cast<const T&>(symbolInstance.symbol));
        }
        void renderInternal(const Logic::Level::BoardState& boardState, const Logic::Level::Boards::Cell* cell, const Logic::Level::Symbols::Symbol& symbol) {
            genericRenderFloating(boardState, cell, static_cast<const T&>(symbol));
        }
    };

}

#endif