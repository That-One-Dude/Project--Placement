/******************************************************************************
* File: blockPos.hpp
* Author: Tony Skeps
* Date: March 18, 2023
******************************************************************************/

#include "blockPos.hpp"

namespace Game::Logic::World {

    bool operator==(BlockPos lhs, BlockPos rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    }

}
