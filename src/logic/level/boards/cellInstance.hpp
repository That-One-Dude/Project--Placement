/******************************************************************************
* File: cellInstance.hpp
* Author: Tony Skeps
* Date: July 9, 2023
******************************************************************************/

#include <unordered_set>

#include "cell.hpp"
//#include "../boardState.hpp"

#ifndef CELL_INSTANCE_HPP
#define CELL_INSTANCE_HPP

namespace Game::Logic::Level::Boards {

    class CellChangeEventListener {
    public:
        virtual ~CellChangeEventListener() {}
        virtual void onThisCellChange(/*BoardState&*/) = 0;
    };

    class CellInstance {
    public:
        CellInstance(Boards::CellColor color/*, BoardState& board*/): color(color), originalColor(color)/*, board(board)*/ {}
        bool setColor(Boards::CellColor);
        const Boards::CellColor& getColor() const {return color;}
        const Boards::CellColor& getOriginalColor() const {return originalColor;}
        bool lock() {if (locked) return false; locked = true; return true;}
        bool isLocked() const {return locked;}
        void addCellChangeEventListener(CellChangeEventListener* listener) {listeners.insert(listener);}
        void removeCellChangeEventListener(CellChangeEventListener* listener) {listeners.erase(listener);}
        void reset();
    private:
        Boards::CellColor color, originalColor;
        std::unordered_set<CellChangeEventListener*> listeners;
        bool locked = false;
        /*BoardState& board*/;
    };

}

#endif