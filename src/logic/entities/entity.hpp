/******************************************************************************
* File: entity.hpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include "../../gl/glMath.hpp"
#include "../../namespacedId.hpp"
#include "../world/worldClass.hpp"

#ifndef ENTITY_HPP
#define ENTITY_HPP

namespace Game::Logic::Entities {

    class Entity {
    public:
        Entity(NamespacedId id, GL::vec3 position): id(id), position(position) {};
        virtual void tick(World::World*) = 0;
        const NamespacedId id;

        GL::vec3 position;
    };

}

#endif