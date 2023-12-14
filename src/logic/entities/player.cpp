/******************************************************************************
* File: player.cpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include <cmath>
#include "player.hpp"

using namespace Game::GL;

namespace Game::Logic::Entities {

    const float highestAngle = M_PI_2 - 0.1f;
    const float speed = 3.0f;

    Player::Player(vec3 position): Entity(NamespacedId("player"), position) {}

    void Player::recieveInput(IO::Input::InputData input) {

        /*horizontalFacing += input.xDeltaLook;
        verticalFacing += input.yDeltaLook;
        while (horizontalFacing >= M_PIf) {
            horizontalFacing -= M_PIf * 2;
        }
        while (horizontalFacing < -M_PIf) {
            horizontalFacing += M_PIf * 2;
        }
        if (verticalFacing > highestAngle) {
            verticalFacing = highestAngle;
        }
        if (verticalFacing < -highestAngle) {
            verticalFacing = -highestAngle;
        }

        pushX = input.xDirection;
        pushY = input.yDirection;

        isPlacing = input.action;
        isJumping = input.jump;*/
    }

    void Player::tick(World::World* world) {
        float c = cosf(horizontalFacing), s = sinf(horizontalFacing);
        vec3 offset(pushX * c + pushY * s, isJumping? -1.0f: 0.0f, -pushX * s + pushY * c);
        position = position + offset * -0.05f * speed;
    }

    mat4 Player::getCamera() {
        return rotationX(verticalFacing) * rotationY(horizontalFacing) * translation(GL::vec3(0.0f, -1.5f, 0.0f)) * translation(-1 * position);
    }

}
