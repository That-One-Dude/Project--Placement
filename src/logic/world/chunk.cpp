/******************************************************************************
* File: chunk.cpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include "generation/noise/simplexNoise.hpp"
#include "chunk.hpp"

#include <iostream>

namespace Game::Logic::World {
    Generation::Noise::SimplexNoise3D testNoise(1);

    float octaves[] {
        1.0f,
        2.0f,
        3.0f,
        4.0f,
        5.0f,
        6.0f,
        7.0f,
        8.0f
    };

    Chunk::Chunk() {
        for (int i = 0; i < chunkWidth; i++) {
            for (int j = 0; j < chunkLength; j++) {
                for (int k = 0; k < chunkHeight; k++) {
                    data[i][k][j].id = 0;
                }
            }
        }
    }

    void Chunk::generate(int xOffset, int yOffset, int zOffset) {
        for (int i = 0; i < chunkWidth; i++) {
            for (int j = 0; j < chunkLength; j++) {
                for (int k = 0; k < chunkHeight; k++) {
                    float coordX = (xOffset * chunkWidth + i) / 100.0f;
                    float coordY = (yOffset * chunkHeight + k) / 100.0f;
                    float coordZ = (zOffset * chunkLength + j) / 100.0f;
                    float density = 0;
                    for (int i = 0; i < sizeof(octaves) / sizeof(float); i++) {
                        density += testNoise.getNoise(coordX * octaves[i], coordY * octaves[i], coordZ * octaves[i]) / octaves[i];
                    }
                    density = (density + 1) / 2;
                    float heightSubtraction = (yOffset * chunkHeight + k) / 256.0f;
                    heightSubtraction = 2.5f * heightSubtraction * (1 - heightSubtraction);
                    density -= heightSubtraction;
                    if (density > 0.0f) {
                        data[i][k][j].id = 1;
                    }
                }
            }
        }
    }
}
