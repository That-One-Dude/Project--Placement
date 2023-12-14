/******************************************************************************
* File: world.hpp
* Author: Tony Skeps
* Date: March 7, 2023
******************************************************************************/

#include <unordered_map>
#include <vector>
#include "blockPos.hpp"
#include "chunk.hpp"
#include "../entities/entity.hpp"

#ifndef WORLD_HPP
#define WORLD_HPP

namespace Game::Logic::World {
    class World {
    public:
        World();
        void tick();
        Chunk* getChunk(BlockPos pos) {return chunks.at(pos);}
        bool chunkLoaded(BlockPos pos) {return chunks.count(pos) > 0;}
        std::unordered_map<BlockPos, Chunk*>::iterator getChunkIterator() {return chunks.begin();}
        std::unordered_map<BlockPos, Chunk*>::iterator getChunkIteratorEnd() {return chunks.end();}
        void addEntity(Entities::Entity*);
        Entities::Entity* getEntity(int index);
        unsigned int getNumEntities();
        BlockPos rayCastBlock(const GL::vec3& startPoint, const GL::vec3& direction, float range);
    private:
        std::unordered_map<BlockPos, Chunk*> chunks;
        std::vector<Entities::Entity*> entities;
    };
}

#endif