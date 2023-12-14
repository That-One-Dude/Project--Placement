/******************************************************************************
* File: noiseGenerator.hpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include "noise/noise.hpp"
#include "worldGenerator.hpp"

#ifndef NOISE_GENERATOR_HPP
#define NOISE_GENERATOR_HPP

namespace Game::Logic::World::Generation {

    class NoiseGenerator: public WorldGenerator{
    public:
        NoiseGenerator(Noise::Noise3D* noise): noise(noise) {}
        virtual Blocks::Block* getBlockAt(BlockPos pos);
    private:
        Noise::Noise3D* noise;
    };

}

#endif