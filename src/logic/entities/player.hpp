/******************************************************************************
* File: player.hpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include "../controllers/controller.hpp"
#include "entity.hpp"

#ifndef PLAYER_HPP
#define PLAYER_HPP

namespace Game::Logic::Entities {

    class Player: public Entity, public Controllers::Controller {
    public:
        Player(GL::vec3 position);
        virtual void tick(World::World*);
        virtual void recieveInput(IO::Input::InputData);
        virtual GL::mat4 getCamera();
    private:
        float horizontalFacing, verticalFacing;
        float pushX, pushY;
        float timeSinceLastPlace = 0.0f;
        bool isPlacing = false;
        bool isJumping = false;
    };

}

#endif
