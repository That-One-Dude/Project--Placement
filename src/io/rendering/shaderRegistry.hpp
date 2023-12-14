/******************************************************************************
* File: shaderRegistry.hpp
* Author: Tony Skeps
* Date: July 31, 2023
******************************************************************************/

#include "../../loadableRegistry.hpp"

#ifndef SHADER_REGISTRY_HPP
#define SHADER_REGISTRY_HPP

namespace Game::IO::Rendering {

    extern LoadableRegistry *const vertexShaderRegistry, *const fragmentShaderRegistry;

}

#endif