/******************************************************************************
* File: inputData.hpp
* Author: Tony Skeps
* Date: June 15, 2022
******************************************************************************/

#ifndef INPUT_DATA_HPP
#define INPUT_DATA_HPP

namespace Game::IO::Input {

    struct InputData {
        bool up = false, down = false, left = false, right = false;
        float xDeltaLook = 0.0f, yDeltaLook = 0.0f;
        float xDirection = 0.0f, yDirection = 0.0f;
        void clear();
    };

}

#endif