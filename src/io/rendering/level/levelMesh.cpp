/******************************************************************************
* File: levelMesh.hpp
* Author: Tony Skeps
* Date: April 20, 2023
******************************************************************************/

#include <vector>
#include "../../../gl/glFunctions.hpp"
#include "levelMesh.hpp"

#include <iostream>

using namespace Game::GL;

namespace Game::IO::Rendering::Level {
    
    Vertex::Vertex(vec3 pos, vec3 normal, vec3 tangent, vec3 biTangent, vec2 uv, int layer = 0): pos(pos), normal(normal), tangent(tangent), biTangent(biTangent), uv(uv.x, uv.y, layer) {}

    LevelMesh::LevelMesh(): buffer(0) {}

    LevelMesh::~LevelMesh() {
        glDeleteBuffers(1, &buffer);
    }

    void LevelMesh::constructMesh(const Logic::Level::Level& level) {

        if (buffer == 0) {
            glGenBuffers(1, &buffer);
        }

        std::vector<Vertex> vertices;

        vec2 uvTopLeftCorner = vec2(0.0f, 0.0f);
        vec2 uvBottomLeftCorner = vec2(0.0f, 1.0f);
        vec2 uvTopRightCorner = vec2(1.0f, 0.0f);
        vec2 uvBottomRightCorner = vec2(1.0f, 1.0f);

        // Top and bottom faces
        /*for (int i = 0; i < level.xSize; i++) {
            for (int j = 0; j < level.zSize; j++) {
                
                if ((level.getTile(i, level.ySize - 1, j) & 0b00001000) != 0) {
                    // Top Face
                    vertices.push_back(Vertex(vec3(i + 0.0f, level.ySize, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopLeftCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, level.ySize, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                    vertices.push_back(Vertex(vec3(i + 0.0f, level.ySize, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));

                    vertices.push_back(Vertex(vec3(i + 0.0f, level.ySize, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, level.ySize, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, level.ySize, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomRightCorner));
                }

                for (int k = 0; k < level.ySize - 1; k++) {
                    if ((level.getTile(i, k, j) & 0b00001000) == 0 && (level.getTile(i, k + 1, j) & 0b00001000) != 0) {
                        // Bottom Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopRightCorner));
                    } else if ((level.getTile(i, k, j) & 0b00001000) != 0 && (level.getTile(i, k + 1, j) & 0b00001000) == 0) {
                        // Top Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));

                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomRightCorner));
                    }
                    if ((level.getTile(i, k, j) & 0b00000100) != 0) {
                        // Top Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.1f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.1f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.1f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner, 1));

                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.1f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.1f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.1f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomRightCorner, 1));
                        // Bottom Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 0.9f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 0.9f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 0.9f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner, 1));

                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 0.9f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 0.9f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 0.9f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopRightCorner, 1));
                    }
                }
            }
        }

        // Left and right faces
        for (int i = 0; i < level.zSize; i++) {
            for (int j = 0; j < level.ySize; j++) {
                if ((level.getTile(0, j, i) & 0b00001000) != 0) {
                    // Left Face
                    vertices.push_back(Vertex(vec3(0, j + 0.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                    vertices.push_back(Vertex(vec3(0, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    vertices.push_back(Vertex(vec3(0, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                    vertices.push_back(Vertex(vec3(0, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                    vertices.push_back(Vertex(vec3(0, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    vertices.push_back(Vertex(vec3(0, j + 1.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                }

                if ((level.getTile(level.xSize - 1, j, i) & 0b00001000) != 0) {
                    // Right Face
                    vertices.push_back(Vertex(vec3(level.xSize, j + 0.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                    vertices.push_back(Vertex(vec3(level.xSize, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                    vertices.push_back(Vertex(vec3(level.xSize, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                    vertices.push_back(Vertex(vec3(level.xSize, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    vertices.push_back(Vertex(vec3(level.xSize, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                    vertices.push_back(Vertex(vec3(level.xSize, j + 1.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                }

                for (int k = 0; k < level.xSize - 1; k++) {
                    if ((level.getTile(k, j, i) & 0b00001000) == 0 && (level.getTile(k + 1, j, i) & 0b00001000) != 0) {
                        // Left Face
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    } else if ((level.getTile(k, j, i) & 0b00001000) != 0 && (level.getTile(k + 1, j, i) & 0b00001000) == 0) {
                        // Right Face
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    }

                    if ((level.getTile(k, j, i) & 0b00000010) != 0) {
                        // Left Face
                        vertices.push_back(Vertex(vec3(k + 0.9f, j + 0.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 0.9f, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 0.9f, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner, 1));

                        vertices.push_back(Vertex(vec3(k + 0.9f, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 0.9f, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 0.9f, j + 1.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner, 1));
                        // Right Face
                        vertices.push_back(Vertex(vec3(k + 1.1f, j + 0.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 1.1f, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 1.1f, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner, 1));

                        vertices.push_back(Vertex(vec3(k + 1.1f, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 1.1f, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(k + 1.1f, j + 1.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner, 1));
                    }
                }
            }
        }

        // Front and back faces
        for (int i = 0; i < level.xSize; i++) {
            for (int j = 0; j < level.ySize; j++) {
                if ((level.getTile(i, j, 0) & 0b00001000) != 0) {
                    // Back Face
                    vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, 0), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, 0), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                    vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, 0), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                    vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, 0), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, 0), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, 0), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                }

                if ((level.getTile(i, j, level.zSize - 1) & 0b00001000) != 0) {
                    // Front Face
                    vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, level.zSize), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                    vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, level.zSize), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, level.zSize), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                    vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, level.zSize), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                    vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, level.zSize), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, level.zSize), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                }
                
                for (int k = 0; k < level.zSize - 1; k++) {
                    if ((level.getTile(i, j, k) & 0b00001000) == 0 && (level.getTile(i, j, k + 1) & 0b00001000) != 0) {
                        // Back Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    } else if ((level.getTile(i, j, k) & 0b00001000) != 0 && (level.getTile(i, j, k + 1) & 0b00001000) == 0) {
                        // Front Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    }

                    if ((level.getTile(i, j, k) & 0b00000001) != 0) {
                        // Back Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, k + 0.9f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 0.9f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 0.9f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner, 1));

                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 0.9f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 0.9f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, k + 0.9f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner, 1));
                        // Front Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, k + 1.1f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.1f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.1f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner, 1));

                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.1f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.1f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner, 1));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, k + 1.1f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner, 1));
                    }
                }
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        numVertices = vertices.size();*/
    }

    void LevelMesh::bindBuffer() {
        glBindVertexBuffer(0, buffer, 0, sizeof(Vertex));
    }

}
