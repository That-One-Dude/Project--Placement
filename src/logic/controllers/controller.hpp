/******************************************************************************
* File: controller.hpp
* Author: Tony Skeps
* Date: March 9, 2023
******************************************************************************/

#include "../../gl/glMath.hpp"
#include "../../io/input/inputData.hpp"

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

namespace Game::Logic::Controllers {

    class Controller {
    public:
        Controller() {}
        virtual void recieveInput(IO::Input::InputData) = 0;
        // Please move this to a better spot later
        virtual GL::mat4 getCamera() = 0;
    };

}

#endif