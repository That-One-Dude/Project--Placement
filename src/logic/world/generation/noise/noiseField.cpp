/******************************************************************************
* File: noiseField.cpp
* Author: Tony Skeps
* Date: March 19, 2023
******************************************************************************/

#include "noiseField.hpp"

namespace Game::Logic::World::Generation::Noise {

    float NoiseField2D::getNoise(float x, float y) {
        float result = 0.0f;
        for (auto item = octaves.begin(); item != octaves.end(); item++) {
            result += noise->getNoise(x * item->frequency, y * item->frequency) * item->amplitude;
        }
        return result;
    }

    float NoiseField3D::getNoise(float x, float y, float z) {
        float result = 0.0f;
        for (auto item = octaves.begin(); item != octaves.end(); item++) {
            result += noise->getNoise(x * item->frequency, y * item->frequency, z * item->frequency) * item->amplitude;
        }
        return result;
    }

    float NoiseField4D::getNoise(float x, float y, float z, float w) {
        float result = 0.0f;
        for (auto item = octaves.begin(); item != octaves.end(); item++) {
            result += noise->getNoise(x * item->frequency, y * item->frequency, z * item->frequency, w * item->frequency) * item->amplitude;
        }
        return result;
    }

}
