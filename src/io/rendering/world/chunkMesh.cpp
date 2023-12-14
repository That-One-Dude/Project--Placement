/******************************************************************************
* File: chunkMesh.hpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include <vector>
#include "../../../gl/glFunctions.hpp"
#include "chunkMesh.hpp"

#include <iostream>

using namespace Game::GL;

namespace Game::IO::Rendering::World {
    
    Vertex::Vertex(vec3 pos, vec3 normal, vec3 tangent, vec3 biTangent, vec2 uv): pos(pos), normal(normal), tangent(tangent), biTangent(biTangent), uv(uv) {}

    ChunkMesh::ChunkMesh(): buffer(0) {}

    ChunkMesh::~ChunkMesh() {
        glDeleteBuffers(1, &buffer);
    }

    void ChunkMesh::constructMesh(const Logic::World::Chunk& chunk, const Logic::World::Chunk* up, const Logic::World::Chunk* right, const Logic::World::Chunk* forward) {

        upLoaded = up != nullptr;
        rightLoaded = right != nullptr;
        forwardLoaded = forward != nullptr;

        if (buffer == 0) {
            glGenBuffers(1, &buffer);
        }

        std::vector<Vertex> vertices;

        float scale = 0.5f;

        vec2 uvTopLeftCorner = vec2(0.0025f, 0.0025f) * scale;
        vec2 uvBottomLeftCorner = vec2(0.0025f, 0.9975f) * scale;
        vec2 uvTopRightCorner = vec2(0.9975f, 0.0025f) * scale;
        vec2 uvBottomRightCorner = vec2(0.9975f, 0.9975f) * scale;

        // Top and bottom faces
        for (int i = 0; i < Logic::World::chunkWidth; i++) {
            for (int j = 0; j < Logic::World::chunkLength; j++) {
                
                if (upLoaded) {
                    if (chunk.getBlock(i, Logic::World::chunkHeight - 1, j).id == 0 && up->getBlock(i, 0, j).id != 0) {
                        // Bottom Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, Logic::World::chunkHeight, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, Logic::World::chunkHeight, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, Logic::World::chunkHeight, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(i + 1.0f, Logic::World::chunkHeight, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, Logic::World::chunkHeight, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, Logic::World::chunkHeight, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopRightCorner));
                    } else if (chunk.getBlock(i, Logic::World::chunkHeight - 1, j).id != 0 && up->getBlock(i, 0, j).id == 0) {
                        // Top Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, Logic::World::chunkHeight, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, Logic::World::chunkHeight, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, Logic::World::chunkHeight, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));

                        vertices.push_back(Vertex(vec3(i + 0.0f, Logic::World::chunkHeight, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, Logic::World::chunkHeight, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, Logic::World::chunkHeight, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomRightCorner));
                    }
                }

                for (int k = 0; k < Logic::World::chunkHeight - 1; k++) {
                    if (chunk.getBlock(i, k, j).id == 0 && chunk.getBlock(i, k + 1, j).id != 0) {
                        // Bottom Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 1.0f), vec3(0.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), uvTopRightCorner));
                    } else if (chunk.getBlock(i, k, j).id != 0 && chunk.getBlock(i, k + 1, j).id == 0) {
                        // Top Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));

                        vertices.push_back(Vertex(vec3(i + 0.0f, k + 1.0f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, k + 1.0f, j + 1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), uvBottomRightCorner));
                    }
                }
            }
        }

        // Left and right faces
        for (int i = 0; i < Logic::World::chunkLength; i++) {
            for (int j = 0; j < Logic::World::chunkHeight; j++) {
                if (rightLoaded) {
                    if (chunk.getBlock(Logic::World::chunkWidth - 1, j, i).id == 0 && right->getBlock(0, j, i).id != 0) {
                        // Left Face
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 0.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 1.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    } else if (chunk.getBlock(Logic::World::chunkWidth - 1, j, i).id != 0 && right->getBlock(0, j, i).id == 0) {
                        // Right Face
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 0.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(Logic::World::chunkWidth, j + 1.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    }
                }

                for (int k = 0; k < Logic::World::chunkWidth - 1; k++) {
                    if (chunk.getBlock(k, j, i).id == 0 && chunk.getBlock(k + 1, j, i).id != 0) {
                        // Left Face
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    } else if (chunk.getBlock(k, j, i).id != 0 && chunk.getBlock(k + 1, j, i).id == 0) {
                        // Right Face
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 0.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(k + 1.0f, j + 1.0f, i + 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    }
                }
            }
        }

        // Front and back faces
        for (int i = 0; i < Logic::World::chunkWidth; i++) {
            for (int j = 0; j < Logic::World::chunkHeight; j++) {
                if (forwardLoaded) {
                    if (chunk.getBlock(i, j, Logic::World::chunkLength - 1).id == 0 && forward->getBlock(i, j, 0).id != 0) {
                        // Back Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    } else if (chunk.getBlock(i, j, Logic::World::chunkLength - 1).id != 0 && forward->getBlock(i, j, 0).id == 0) {
                        // Front Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, Logic::World::chunkLength), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    }
                }
                
                for (int k = 0; k < Logic::World::chunkLength - 1; k++) {
                    if (chunk.getBlock(i, j, k).id == 0 && chunk.getBlock(i, j, k + 1).id != 0) {
                        // Back Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));

                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                    } else if (chunk.getBlock(i, j, k).id != 0 && chunk.getBlock(i, j, k + 1).id == 0) {
                        // Front Face
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));

                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 0.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvBottomRightCorner));
                        vertices.push_back(Vertex(vec3(i + 0.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopLeftCorner));
                        vertices.push_back(Vertex(vec3(i + 1.0f, j + 1.0f, k + 1.0f), vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), uvTopRightCorner));
                    }
                }
            }
        }

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        numVertices = vertices.size();
    }

    void ChunkMesh::bindBuffer() {
        glBindVertexBuffer(0, buffer, 0, sizeof(Vertex));
    }

}
