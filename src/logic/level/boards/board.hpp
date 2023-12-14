/******************************************************************************
* File: board.hpp
* Author: Tony Skeps
* Date: June 27, 2023
******************************************************************************/

#include <vector>
#include "cell.hpp"
#include "../symbols/symbol.hpp"
#include "../../../namespacedId.hpp"

#ifndef BOARD_HPP
#define BOARD_HPP

namespace Game::Logic::Level::Boards {

    class Board {
    public:
        Board() {}
        ~Board() {}
        virtual void fillCells(std::vector<const Cell*>&) const = 0;
        void fillCells() {this->fillCells(cells);}
        //virtual const Cell* getSelectedCell(float x, float y) const = 0;
        // only 1024 cells max
        const std::vector<const Cell*>& getCells() const {return cells;}
        NamespacedId getRendererId() const {return rendererId;}
        void setRendererId(NamespacedId rendererId) {this->rendererId = rendererId;}
    private:
        std::vector<const Cell*> cells;
        NamespacedId rendererId;
    };

    class BoardFactory {
    public:
        BoardFactory() {}
        ~BoardFactory() {}
        virtual Board* produce(void*) const = 0;
        virtual unsigned int getDataSize(Board* board) const = 0;
        virtual void writeData(void* data, Board* board) const = 0;
    };

    // A utility for use in board classes
    union CellOrNot {
        CellOrNot() {}
        ~CellOrNot() {
            if (is) {
                // going to have to do some testing
                cell.cell.~Cell();
            }
        }
        bool is = false;
        struct {
            bool is;
            Cell cell;
        } cell;
    };
}

#endif
