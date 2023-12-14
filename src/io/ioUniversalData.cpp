/******************************************************************************
* File: ioUniversalData.cpp
* Author: Tony Skeps
* Date: August 29, 2022
******************************************************************************/

#include <cmath>
#include "../universalData.hpp"
#include "ioUniversalData.hpp"

namespace Game::IO {

    float fov = M_PIf * 70.0f / 360.0f;
    GL::mat4 camera = GL::perspective(fov, viewportRatio, 0.1f, 300.0f);//Game::GL::frustum(-viewportSize / viewportRatio, viewportSize / viewportRatio, -viewportSize, viewportSize, 0.1f, 100.0f);

}
