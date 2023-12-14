/******************************************************************************
* File: world.cpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include "world.hpp"

namespace Game::Logic::World {

    World::World() {
    }

    bool generated = false;

    void World::tick() {
        if (!generated) {
            for (int i = -4; i < 4; i++) {
                for (int j = -4; j < 4; j++) {
                    for (int k = 0; k < 8; k++) {
                        chunks[BlockPos(i, k, j)] = new Chunk();
                        chunks[BlockPos(i, k, j)]->generate(i, k, j);
                    }
                }
            }
            generated = true;
        }
        for(int i = 0; i < entities.size(); i++) {
            entities[i]->tick(this);
        }
    }

    void World::addEntity(Entities::Entity* entity) {
        entities.push_back(entity);
    }

    Entities::Entity* World::getEntity(int index) {
        return entities.at(index);
    }

    unsigned int World::getNumEntities() {
        return entities.size();
    }

}
