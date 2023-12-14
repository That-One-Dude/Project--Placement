/******************************************************************************
* File: noise.hpp
* Author: Tony Skeps
* Date: March 18, 2023
******************************************************************************/

#ifndef NOISE_HPP
#define NOISE_HPP

namespace Game::Logic::World::Generation::Noise {

    class Noise2D {
    public:
        virtual float getNoise(float x, float y) = 0;
    };

    class Noise3D {
    public:
        virtual float getNoise(float x, float y, float z) = 0;
    };

    class Noise4D {
    public:
        virtual float getNoise(float x, float y, float z, float w) = 0;
    };

}

#endif