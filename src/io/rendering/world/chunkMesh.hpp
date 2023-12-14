/******************************************************************************
* File: chunkMesh.hpp
* Author: Tony Skeps
* Date: March 12, 2023
******************************************************************************/

#include "../../../gl/glMath.hpp"
#include "../../../logic/world/chunk.hpp"

#ifndef CHUNK_RENDERER_HPP
#define CHUNK_RENDERER_HPP

namespace Game::IO::Rendering::World {

    struct Vertex {
        GL::vec3 pos;
        GL::vec3 normal;
        GL::vec3 tangent;
        GL::vec3 biTangent;
        GL::vec2 uv;
        Vertex() {}
        Vertex(GL::vec3 pos, GL::vec3 normal, GL::vec3 tangent, GL::vec3 biTangent, GL::vec2 uv);
    };

    class ChunkMesh {
    public:
        ChunkMesh();
        ~ChunkMesh();
        void bindBuffer();
        void constructMesh(const Logic::World::Chunk& chunk, const Logic::World::Chunk* up, const Logic::World::Chunk* right, const Logic::World::Chunk* forward);
        unsigned int getNumVertices() {return numVertices;}
        bool isLoaded() {return chunkLoaded;}
        void setLoaded() {chunkLoaded = true;}
        void resetLoaded() {chunkLoaded = false;}
        bool isUpLoaded() {return upLoaded;}
        bool isRightLoaded() {return rightLoaded;}
        bool isForwardLoaded() {return forwardLoaded;}
    private:
        unsigned int buffer;
        unsigned int numVertices;
        bool chunkLoaded;
        bool upLoaded, rightLoaded, forwardLoaded;
    };

}

#endif