/******************************************************************************
* File: block.hpp
* Author: Tony Skeps
* Date: March 4, 2023
******************************************************************************/

#include "../../namespacedId.hpp"

#ifndef BLOCK_HPP
#define BLOCK_HPP

namespace Game::Logic::Blocks {

    struct BlockInternal {
        int id;
    };

    class Block {
    public:
        Block(const NamespacedId id):id(id) {}

        virtual void onPickup() = 0;
        virtual void onThrow() = 0;
        
        unsigned int getInternalId() {return internalId;}
        const NamespacedId id;
    private:
        unsigned int internalId;
    };

}

#endif
