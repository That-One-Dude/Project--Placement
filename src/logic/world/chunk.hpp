/******************************************************************************
* File: chunk.hpp
* Author: Tony Skeps
* Date: March 5, 2023
******************************************************************************/

#include "../blocks/block.hpp"

#ifndef CHUNK_HPP
#define CHUNK_HPP

namespace Game::Logic::World {
    const int chunkWidth = 32, chunkHeight = 32, chunkLength = 32;

    class Chunk {
    public:
        Chunk();
        void markDirty() {dirty = true;}
        void markClean() {dirty = false;}
        bool isDirty() {return dirty;}
        Blocks::BlockInternal getBlock(int x, int y, int z) const {return data[x][y][z];}
        void setBlock(int x, int y, int z, Blocks::BlockInternal block) {data[x][y][z] = block;}
        void generate(int xOffset, int yOffset, int zOffset);
    private:
        bool dirty = true;
        Blocks::BlockInternal data[chunkWidth][chunkHeight][chunkLength];
    };
}

#endif