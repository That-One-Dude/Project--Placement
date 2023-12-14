/******************************************************************************
* File: levelDataStorageHelper.hpp
* Author: Tony Skeps
* Date: August 17, 2023
******************************************************************************/

#include "symbols/symbol.hpp"
#include "boards/board.hpp"
#include "../../namespacedId.hpp"
#include <unordered_set>

#ifndef LEVEL_DATA_HPP
#define LEVEL_DATA_HPP

namespace Game::Logic::Level {
    
    class LevelDataStorageHelper {
    public:
        void addId(NamespacedId id);
    private:
        unsigned int offset;
        std::unordered_set<NamespacedId,NamespacedIdHash> idSet;

    };

}

#endif