/******************************************************************************
* File: firstPersonController.hpp
* Author: Tony Skeps
* Date: March 9, 2023
******************************************************************************/

#include <cmath>
#include "firstPersonController.hpp"

#include <iostream>

namespace Game::Logic::Controllers {

    const float highestAngle = M_PI_2 - 0.1f;

    FirstPersonController::FirstPersonController(): horizontalFacing(0.0f), verticalFacing(0.0f), pushX(0.0f), pushY(0.0f) {}

    void FirstPersonController::recieveInput(IO::Input::InputData input) {

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
        pushY = input.yDirection;*/
    }

    GL::mat4 FirstPersonController::getCamera() {
        return GL::rotationX(verticalFacing) * GL::rotationY(horizontalFacing) * GL::translation(GL::vec3(-8.0f, -64.0f, -8.0f));
        //return GL::yaxisrotation(horizontalFacing);
        /*return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };*/
    }

}