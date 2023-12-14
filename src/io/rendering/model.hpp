/******************************************************************************
* File: model.hpp
* Author: Tony Skeps
* Date: October 19, 2022
******************************************************************************/

#include <SFML/OpenGL.hpp>
#include "../../tinygltf/tiny_gltf.h"
#include "../../loadableRegistry.hpp"

#ifndef MODEL_HPP
#define MODEL_HPP

namespace Game::IO::Rendering {

    /*class Model {
        friend Model* createModelVersion1(unsigned char* input);
    public:
        ~Model();
        const unsigned int numVertices;
        const unsigned int cpuVertexSize;
        const unsigned char* cpuVertexData;
        const unsigned short gpuVertexSize;
        const unsigned int numGPUBytes;
        const GLuint vbo;
        //GLuint vao; // Maybe? The VAO might share a lot of similarities between other models
    private:
        Model(unsigned int numVertices, unsigned int cpuVertexSize, unsigned char* cpuVertexData, unsigned short gpuVertexSize, GLuint vbo);
    };

    Model* createModelVersion1(unsigned char* input);*/

    // Takes in data in the Compact Generic Vertices format
    tinygltf::Model* loadModelFromId(NamespacedId);

    extern GeneralizedLoadableRegistry modelRegistry;

    /*class NewModel {
    public:
        // Takes in data in the Compact Generic Vertices format
        NewModel(void* data);
        ~NewModel();
        GLuint getVBO();
        char* getNonGPUData();

    private:
        GLuint vbo;
        char* nonGPUData;
        unsigned short numVertices;
        unsigned short vertexSize;
        //GLuint vao; // Maybe? The VAO might share a lot of similarities between other models
    };

    // Takes in a Compact Generic Vertices file
    NewModel loadNewModelFromFile(std::string file);

    class Model {
    public:
        unsigned char version;
        unsigned short numVertices;
        short vertexSize;
        char* vertexData;
    };

    Model loadModel(void* data);

    // Takes in a Compact Generic Vertices file
    Model loadModelFromFile(std::string file);*/

}

#endif
