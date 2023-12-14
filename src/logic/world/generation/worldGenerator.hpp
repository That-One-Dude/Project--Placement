/******************************************************************************
* File: worldGenerator.hpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include "../../blocks/block.hpp"
#include "../blockPos.hpp"

#ifndef WORLD_GENERATOR_HPP
#define WORLD_GENERATOR_HPP

namespace Game::Logic::World::Generation {

    class WorldGenerator {
    public:
        virtual Blocks::Block* getBlockAt(BlockPos pos) = 0;
    };

}

#endif