/******************************************************************************
* File: boardState.hpp
* Author: Tony Skeps
* Date: June 30, 2023
******************************************************************************/

#include <unordered_map>
#include <vector>
#include <unordered_set>

#include "boards/cell.hpp"
#include "boards/cellInstanceClass.hpp"
//#include "symbols/symbolInstanceClass.hpp"
#include "symbols/symbolInstance.hpp"
#include "levelClass.hpp"

#ifndef BOARD_STATE_HPP
#define BOARD_STATE_HPP

namespace Game::Logic::Level {

    class BoardState {
        friend Level;
    public:
        bool setCellColor(const Boards::Cell* cell, Boards::CellColor color);
        bool lockCellColor(const Boards::Cell* cell);
        const std::unordered_map<const Boards::Cell*, Boards::CellInstance>& getCellColors() const {return cellColors;}

        //const std::vector<std::pair<const Boards::Cell*, Symbols::SymbolInstance*>>& getSymbolOrder() const {return symbolOrder;}
        const std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>& getSymbolInstances() const {return symbolInstances;}
        std::pair<std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::iterator, std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::iterator> getSymbolInstances(const Boards::Cell* cell) {return symbolInstances.equal_range(cell);}
        std::pair<std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator, std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance>::const_iterator> getSymbolInstances(const Boards::Cell* cell) const {return symbolInstances.equal_range(cell);}

        // returns true if this made more symbols to update
        // should I allow this to be called by symbols?
    private:
        // Should it be a pair?
        std::vector<Symbols::SymbolInstance*> symbolOrder;
        // I will allow the capability of multiple symbols per cell, but I will refrain from using it right now
        std::unordered_multimap<const Boards::Cell*, Symbols::SymbolInstance> symbolInstances;
        std::unordered_map<const Boards::Cell*, Boards::CellInstance> cellColors;
        std::unordered_set<const Boards::Cell*> updatedCells;
    };

}

#endif