/******************************************************************************
* File: mineConditionSymbol.hpp
* Author: Tony Skeps
* Date: September 30, 2023
******************************************************************************/

#include "../boardState.hpp"
#include "../boards/cellInstance.hpp"
#include "basicSymbolFactory.hpp"
#include "mineConditionSymbol.hpp"

namespace Game::Logic::Level::Symbols {

    MineConditionSymbol::MineConditionSymbol(Boards::CellColor symbolColor, bool showSatisfaction, unsigned int numMines): Symbol(std::unordered_set<NamespacedId, NamespacedIdHash>{}, symbolColor, showSatisfaction), numMines(numMines) {}

    bool MineConditionSymbol::isSatisfied(const BoardState& boardState, const Boards::Cell& cell) const {
        unsigned int mineCount = 0;
        if (getColor() == Boards::nocell) {
            for (auto iter = cell.getNeighbors().cbegin(); iter != cell.getNeighbors().cend(); iter++) {
                if (iter->cell != nullptr && boardState.getCellColors().at(iter->cell).getColor() != boardState.getCellColors().at(&cell).getColor()) {
                    mineCount++;
                }
            }
        } else {
            for (auto iter = cell.getNeighbors().cbegin(); iter != cell.getNeighbors().cend(); iter++) {
                if (iter->cell != nullptr && boardState.getCellColors().at(iter->cell).getColor() == getColor()) {
                    mineCount++;
                }
            }
        }
        return mineCount == numMines;
    }

    Symbol* MineConditionSymbol::cloneInternal() {
        return new MineConditionSymbol(getColor(), showSatisfaction, numMines);
    }

    class: public SymbolFactory {
    public:
        Symbol* produce(Boards::CellColor color, bool showSatisfaction, void* data) const {
            return new MineConditionSymbol(color, showSatisfaction, *((unsigned int*)(data)));
        }
        virtual unsigned int getDataSize(Symbol* symbol) const {
            return sizeof(unsigned int);
        }
        virtual void writeData(void* data, Symbol* symbol) const {
            MineConditionSymbol* trueSymbol = (MineConditionSymbol*)symbol;
            *(unsigned int*)data = trueSymbol->numMines;
        }
    } trueMineConditionFactory;

    const SymbolFactory* mineConditionFactory = &trueMineConditionFactory;

}
