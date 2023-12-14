/******************************************************************************
* File: blockPos.hpp
* Author: Tony Skeps
* Date: March 7, 2023
******************************************************************************/

#include <functional>

#ifndef BLOCK_POS_HPP
#define BLOCK_POS_HPP

namespace Game::Logic::World {
    struct BlockPos {
        int x, y, z;
        BlockPos() {}
        BlockPos(int x, int y, int z): x(x), y(y), z(z) {}
    };

    bool operator==(BlockPos lhs, BlockPos rhs);
}

namespace std {
    template <> struct hash<Game::Logic::World::BlockPos> {
        size_t operator()(const Game::Logic::World::BlockPos &vect) const noexcept {
            std::hash<decltype(vect.x)> hasher;

            size_t hash1 = hasher(vect.x);
            size_t hash2 = hasher(vect.y);
            size_t hash3 = hasher(vect.z);

            return std::hash<decltype(vect.x)>{}(
                (hash1 ^ (hash2 << hash3) ^ hash3));
        }
    };
}

#endif