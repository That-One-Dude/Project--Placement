/******************************************************************************
* File: simplexNoise.hpp
* Author: Tony Skeps
* Date: March 18, 2023
******************************************************************************/

#include "noise.hpp"

#ifndef SIMPLEX_NOISE_HPP
#define SIMPLEX_NOISE_HPP

namespace Game::Logic::World::Generation::Noise {

    class SimplexNoise {
    protected:
        SimplexNoise(unsigned int seed);
        unsigned char randomValues[256];
    };

    class SimplexNoise2D: public SimplexNoise, public Noise2D {
    public:
        SimplexNoise2D(unsigned int seed);
        virtual float getNoise(float x, float y);
    private:
        unsigned char gradModRandomValues[256];
    };

    class SimplexNoise3D: public SimplexNoise, public Noise3D {
    public:
        SimplexNoise3D(unsigned int seed);
        virtual float getNoise(float x, float y, float z);
    private:
        unsigned char gradModRandomValues[256];
    };

    class SimplexNoise4D: public SimplexNoise, public Noise4D {
    public:
        SimplexNoise4D(unsigned int seed);
        virtual float getNoise(float x, float y, float z, float w);
    private:
        unsigned char gradModRandomValues[256];
    };

}

#endif