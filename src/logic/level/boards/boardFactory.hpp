/******************************************************************************
* File: boardFactory.hpp
* Author: Tony Skeps
* Date: August 14, 2023
******************************************************************************/

#include <unordered_map>
#include "../../../namespacedId.hpp"
#include "board.hpp"

#ifndef BOARD_FACTORY_HPP
#define BOARD_FACTORY_HPP

namespace Game::Logic::Level::Boards {

    class CompositeBoardFactory {
    public:
        CompositeBoardFactory() {}
        ~CompositeBoardFactory();
        void addFactory(NamespacedId, const BoardFactory*);
        Board* createBoard(NamespacedId, void*) const;
        virtual unsigned int getDataSize(Board* board) const;
        virtual void writeData(void* data, Board* board) const;
    private:
        std::unordered_map<NamespacedId, const BoardFactory*, NamespacedIdHash> factories;
    };

}

#endif