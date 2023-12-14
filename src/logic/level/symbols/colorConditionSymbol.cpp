/******************************************************************************
* File: colorConditionSymbol.cpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"
#include "basicSymbolFactory.hpp"
#include "colorConditionSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    ColorConditionSymbol::ColorConditionSymbol(Boards::CellColor symbolColor, bool showSatisfaction): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{}, symbolColor, showSatisfaction) {}

    bool ColorConditionSymbol::isSatisfied(const BoardState& boardState, const Boards::Cell& cell) const {
        if (getColor() == Boards::nocell) {
            return true;
        }
        return boardState.getCellColors().at(&cell).getColor() == getColor();
    }

    Symbol* ColorConditionSymbol::cloneInternal() {
        return new ColorConditionSymbol(getColor(), showSatisfaction);
    }

    /*class: public SymbolFactory {
    public:
        Symbol* produce(Boards::CellColor color, void*) const {
            return new ColorConditionSymbol(color);
        }
    } value;*/

    BasicSymbolFactory<ColorConditionSymbol> trueColorConditionFactory;

    const SymbolFactory* colorConditionFactory = &trueColorConditionFactory;

}
