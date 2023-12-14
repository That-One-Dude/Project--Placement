/******************************************************************************
* File: firstPersonController.hpp
* Author: Tony Skeps
* Date: March 9, 2023
******************************************************************************/

#include "controller.hpp"

#ifndef FIRST_PERSON_CONTROLLER_HPP
#define FIRST_PERSON_CONTROLLER_HPP

namespace Game::Logic::Controllers {

    class FirstPersonController: public Controller {
    public:
        FirstPersonController();
        virtual void recieveInput(IO::Input::InputData);
        virtual GL::mat4 getCamera();
    private:
        float horizontalFacing, verticalFacing;
        float pushX, pushY;
    };

}

#endif