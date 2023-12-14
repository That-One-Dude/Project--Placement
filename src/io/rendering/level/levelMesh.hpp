/******************************************************************************
* File: levelMesh.cpp
* Author: Tony Skeps
* Date: April 20, 2023
******************************************************************************/

#include "../../../gl/glMath.hpp"
#include "../../../logic/level/level.hpp"

#ifndef LEVEL_MESH_HPP
#define LEVEL_MESH_HPP

namespace Game::IO::Rendering::Level {

    struct Vertex {
        GL::vec3 pos;
        GL::vec3 normal;
        GL::vec3 tangent;
        GL::vec3 biTangent;
        GL::vec3 uv;
        Vertex() {}
        Vertex(GL::vec3 pos, GL::vec3 normal, GL::vec3 tangent, GL::vec3 biTangent, GL::vec2 uv, int layer);
    };

    class LevelMesh {
    public:
        LevelMesh();
        ~LevelMesh();
        void bindBuffer();
        void constructMesh(const Logic::Level::Level& level);
        unsigned int getNumVertices() {return numVertices;}
    private:
        unsigned int buffer;
        unsigned int numVertices;
    };

}

#endif