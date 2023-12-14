/******************************************************************************
* File: symbolInstance.cpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

#include "symbolInstance.hpp"

namespace Game::Logic::Level::Symbols {

    void SymbolInstance::updateSatisfaction(const BoardState& boardState) {
        if (unoverridedSatisfactionUnknown) {
            unoverridedSatisfaction = symbol.isSatisfied(boardState, cell);
            unoverridedSatisfactionUnknown = false;
        }
    }

    bool SymbolInstance::isSatisfied() const {
        if (satisfactionOverridden) {
            return overridedSatisfaction;
        }
        return unoverridedSatisfaction;
    }

}
