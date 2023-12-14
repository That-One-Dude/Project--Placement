/******************************************************************************
* File: noiseField.hpp
* Author: Tony Skeps
* Date: March 19, 2023
******************************************************************************/

#include <vector>
#include "noise.hpp"

#ifndef NOISE_FIELD_HPP
#define NOISE_FIELD_HPP

namespace Game::Logic::World::Generation::Noise {

    struct octave {
        float frequency;
        float amplitude;
        octave(float frequency, float amplitude): frequency(frequency), amplitude(amplitude) {};
    };

    class NoiseField {
    protected:
        NoiseField(std::vector<octave> octaves): octaves(octaves) {}
        std::vector<octave> octaves;
    };

    class NoiseField2D: public NoiseField, public Noise2D {
    public:
        NoiseField2D(std::vector<octave> octaves, Noise2D* noise): NoiseField(octaves), noise(noise) {}
        virtual float getNoise(float x, float y);
    private:
        Noise2D* noise;
    };

    class NoiseField3D: public NoiseField, public Noise3D {
    public:
        NoiseField3D(std::vector<octave> octaves, Noise3D* noise): NoiseField(octaves), noise(noise) {}
        virtual float getNoise(float x, float y, float z);
    private:
        Noise3D* noise;
    };

    class NoiseField4D: public NoiseField, public Noise4D {
    public:
        NoiseField4D(std::vector<octave> octaves, Noise4D* noise): NoiseField(octaves), noise(noise) {}
        virtual float getNoise(float x, float y, float z, float w);
    private:
        Noise4D* noise;
    };

}

#endif