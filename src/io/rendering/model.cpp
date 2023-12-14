/******************************************************************************
* File: model.hpp
* Author: Tony Skeps
* Date: October 30, 2022
******************************************************************************/

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string>
#include <iostream>
#include "../../gl/glFunctions.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "model.hpp"

using namespace Game::GL;

namespace Game::IO::Rendering {

    /*// Metadata
    // - version
    // - numVertices
    // - numCPUFields in a vertex
    // - numGPUFields and which aforementioned fields are shared
    // 
    // Vertex Format
    // - Bytes per field
    //
    // Vertex Data Pointers
    // - Field

    Model::Model(unsigned int numVertices, unsigned int cpuVertexSize, unsigned char* cpuVertexData, unsigned short gpuVertexSize, GLuint vbo): 
        numVertices(numVertices), cpuVertexSize(cpuVertexSize), cpuVertexData(cpuVertexData), gpuVertexSize(gpuVertexSize), numGPUBytes(gpuVertexSize * numVertices), vbo(vbo) {}

    Model::~Model() {
        if (cpuVertexData != nullptr) {
            delete[] cpuVertexData;
        }
        if (vbo != 0) {
            GLuint delVBO = vbo;
            glDeleteBuffers(1, &delVBO);
        }
    }

    Model* createModelVersion1(unsigned char* input) {
        unsigned char* start = input - 1;

        // step 0 - Metadata

        unsigned int numVertices = *((unsigned short*)input);
        input += sizeof(unsigned short);
        numVertices++;

        unsigned char numberOfGPUFields = *input;
        input += sizeof(unsigned char);
        bool hasCPUFields = (numberOfGPUFields & 0b10000000) != 0;
        numberOfGPUFields &= 0b11111;

        if (numberOfGPUFields > 16) {
            numberOfGPUFields = 16;
        }

        unsigned char numberOfCPUFields;

        if (hasCPUFields) {
            numberOfCPUFields = *input;
            numberOfCPUFields++;
            input += sizeof(unsigned char);
        } else {
            numberOfCPUFields = 0;
        }

        bool includeSharedFields = hasCPUFields && (numberOfGPUFields > 0);

        unsigned short sharedFields = 0;
        
        if (includeSharedFields) {
            if (numberOfGPUFields > 8) {
                sharedFields = *((unsigned short*)input);
                input += sizeof(unsigned short);
            } else {
                unsigned char temp = *input;
                sharedFields = temp;
                input += sizeof(unsigned char);
            }
        }

        // step 1 - vertex format
        unsigned char numberOfReferencedFields = 0;

        unsigned short cpuVertexSize = 0;
        unsigned short gpuVertexSize = 0;

        struct VertexFormat{
            bool direct;
            bool cpu = false;
            bool gpu = false;
            unsigned char numBytes;
        } vertexFormatData[256 + 16];

        unsigned short index;
        unsigned char byte;
        unsigned short numberOfFields = numberOfGPUFields;
        if (hasCPUFields) {
            numberOfFields += numberOfCPUFields + 1;
        }

        for (index = 0; index < numberOfGPUFields; index++) {
            if ((index & 0b1) != 0) {
                byte >>= 4;
            } else {
                byte = *input;
                input++;
            }
            vertexFormatData[index].direct = (byte & 0b1000) > 0;
            if (!vertexFormatData[index].direct) {
                numberOfReferencedFields += 1;
            }

            // Welp, goodbye, stardard calculations
            // 1 - 0 - 000 - 1 byte objects
            // 2 - 1 - 001 - 2 1 byte objects, 2 byte objects
            // 3 - 2 - 010 - 3 1 byte objects
            // 4 - 3 - 011 - 4 1 byte objects, 2 2 byte objects, 4 byte objects
            // 6 - 4 * 1 + 2 - 101 - 3 2 byte objects
            // 8 - 4 * 2 - 110 - 4 2 byte objects, 2 4 byte objects, 8 byte objects (?)
            // 12 - 4 * 3 - 101 - 3 4 byte objetcs
            // 16 - 4 * 4 - 111 - 4 4 byte objects, 2 8 byte objects (?)
            // maybe allow 24 and 32, though they are counted as 2 attributes

            vertexFormatData[index].numBytes = ((byte & 0b11) + 1) * (1 + 3 * ((byte >> 2) & 0b1)) + (((byte & 0b111) == 0b101) << 1);

            vertexFormatData[index].gpu = true;
            gpuVertexSize += vertexFormatData[index].numBytes;
            if (sharedFields & (0b1 << index) != 0) {
                vertexFormatData[index].cpu = true;
                numberOfFields--;
                cpuVertexSize += vertexFormatData[index].numBytes;
            }

        }

        for (; index < numberOfFields; index++) {
            if ((index & 0b1) != 0) {
                byte >>= 4;
            } else {
                byte = *input;
                input++;
            }
            vertexFormatData[index].direct = (byte & 0b1000) > 0;
            if (!vertexFormatData[index].direct) {
                numberOfReferencedFields += 1;
            }

            vertexFormatData[index].numBytes = ((byte & 0b11) + 1) * (1 + 3 * ((byte >> 2) & 0b1)) + (((byte & 0b111) == 0b101) << 1);

            vertexFormatData[index].cpu = true;
            cpuVertexSize += vertexFormatData[index].numBytes;
        }

        // step 2 - Vertex Data Pointers
        unsigned int* vertexPointers = (unsigned int*)input;

        input += sizeof(unsigned int) * numberOfReferencedFields;

        // step 3 - Construct Vertices
        GLuint vbo = 0;
        unsigned char* cpuData = nullptr;

        if (gpuVertexSize > 0) {
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, gpuVertexSize * numVertices, nullptr, GL_STATIC_DRAW);
        }

        if (hasCPUFields) {
            cpuData = new unsigned char[cpuVertexSize * numVertices];
        }

        unsigned short referencedFieldNumber;
        unsigned char gpuData[256];
        unsigned char gpuIndex;
        unsigned short cpuIndex;

        unsigned char* cpuDataPosition = cpuData;
        unsigned char* gpuDataPosition;

        for (int i = 0; i < numVertices; i++) {
            referencedFieldNumber = 0;
            gpuDataPosition = gpuData;
            for (int j = 0; j < numberOfFields; j++) {
                unsigned char count = vertexFormatData[j].numBytes;
                bool isCPU = vertexFormatData[j].cpu;
                bool isGPU = vertexFormatData[j].cpu;
                if (vertexFormatData[j].direct) {
                    if (count & 0b111 == 0) { // 8 bytes
                        for (unsigned char k = 0; k < count >> 3; k++) {
                            if (isCPU) {
                                ((unsigned long*)cpuDataPosition)[k] = ((unsigned long*)input)[k];
                            }
                            if (isGPU) {
                                ((unsigned long*)gpuDataPosition)[k] = ((unsigned long*)input)[k];
                            }
                        }
                    } else if (count & 0b11 == 0) { // 4 bytes
                        for (unsigned char k = 0; k < count >> 2; k++) {
                            if (isCPU) {
                                ((unsigned int*)cpuDataPosition)[k] = ((unsigned int*)input)[k];
                            }
                            if (isGPU) {
                                ((unsigned int*)gpuDataPosition)[k] = ((unsigned int*)input)[k];
                            }
                        }
                    } else if (count & 0b1 == 0) { // 2 bytes
                        for (unsigned char k = 0; k < count >> 1; k++) {
                            if (isCPU) {
                                ((unsigned short*)cpuDataPosition)[k] = ((unsigned short*)input)[k];
                            }
                            if (isGPU) {
                                ((unsigned short*)gpuDataPosition)[k] = ((unsigned short*)input)[k];
                            }
                        }
                    } else {
                        for (unsigned char k = 0; k < count; k++) {
                            if (isCPU) {
                                cpuDataPosition[k] = input[k];
                            }
                            if (isGPU) {
                                gpuDataPosition[k] = input[k];
                            }
                        }
                    }
                    input += count;
                } else {
                    unsigned int index = *((unsigned short*)input);
                    if (count & 0b111 == 0) { // 8 bytes
                        index *= count >> 3;
                        for (unsigned char k = 0; k < count >> 3; k++) {
                            if (isCPU) {
                                ((unsigned long*)cpuDataPosition)[k] = ((unsigned long*)(start + vertexPointers[referencedFieldNumber]))[index + k];
                            }
                            if (isGPU) {
                                ((unsigned long*)gpuDataPosition)[k] = ((unsigned long*)(start + vertexPointers[referencedFieldNumber]))[index + k];
                            }
                        }
                    } else if (count & 0b11 == 0) { // 4 bytes
                        index *= count >> 2;
                        for (unsigned char k = 0; k < count >> 2; k++) {
                            if (isCPU) {
                                ((unsigned int*)cpuDataPosition)[k] = ((unsigned int*)(start + vertexPointers[referencedFieldNumber]))[index + k];
                            }
                            if (isGPU) {
                                ((unsigned int*)gpuDataPosition)[k] = ((unsigned int*)(start + vertexPointers[referencedFieldNumber]))[index + k];
                            }
                        }
                    } else if (count & 0b1 == 0) { // 2 bytes
                        index *= count >> 1;
                        for (unsigned char k = 0; k < count >> 1; k++) {
                            if (isCPU) {
                                ((unsigned short*)cpuDataPosition)[k] = ((unsigned short*)(start + vertexPointers[referencedFieldNumber]))[index + k];
                            }
                            if (isGPU) {
                                ((unsigned short*)gpuDataPosition)[k] = ((unsigned short*)(start + vertexPointers[referencedFieldNumber]))[index + k];
                            }
                        }
                    } else {
                        index *= count;
                        for (unsigned int k = 0; k < count; k++) {
                            if (isCPU) {
                                cpuDataPosition[k] = (start + vertexPointers[referencedFieldNumber])[index + k];
                            }
                            if (isGPU) {
                                gpuDataPosition[k] = (start + vertexPointers[referencedFieldNumber])[index + k];
                            }
                        }
                    }
                    referencedFieldNumber++;
                    input += sizeof(unsigned short);
                }
                if (isCPU) {
                    cpuDataPosition += count;
                }
                if (isGPU) {
                    gpuDataPosition += count;
                }
            }
        }

        return new Model(numVertices, cpuVertexSize, cpuData, gpuVertexSize, vbo);

        /*unsigned short sharedFields;
        unsigned char numberOfGPUFields;
        unsigned char numberOfCPUFields = *((unsigned char*)input);
        if ((numberOfCPUFields & 0b11000000) == 0) {
            numberofSharedFields = 16;
        } else {
            if ((numberOfCPUFields & 0b11000000) != 0) {
                unsigned char numberOfGPUFields = *((unsigned char*)input);
                input += sizeof(unsigned char);
                unsigned char numberofSharedFields = numberOfGPUFields & 0b1111;
                numberOfGPUFields >>= 4;
                unsigned char numberOfCPUFields = *((unsigned char*)input);
            } else if ((numberOfCPUFields & 0b01000000) != 0) {

            } else if ((numberOfCPUFields & 0b10000000) != 0) {
                
            }
        }

        unsigned char numberOfGPUFields = *((unsigned char*)input);
        input += sizeof(unsigned char);
        unsigned char numberofSharedFields = numberOfGPUFields & 0b1111;
        numberOfGPUFields >>= 4;
        unsigned char numberOfCPUFields = *((unsigned char*)input);
        input += sizeof(unsigned char);
        unsigned char numberOfReferencedFields = numberOfCPUFields;
        unsigned short numVertices = *((unsigned short*)input);
        input += sizeof(unsigned short);

        // step 1 - Vertex Format
        unsigned short cpuVertexSize = 0;
        unsigned short gpuVertexSize = 0;

        struct VertexSizes{
            bool direct;
            bool cpu;
            bool gpu;
            unsigned char numBytes;
        } vertexSizes[80];

        unsigned char byte;

        for (unsigned char i = 0; i < numberOfCPUFields; i++) {
            if ((i & 0b1) != 0) {
                byte >>= 4;
            } else {
                byte = *input;
                input++;
            }
            vertexSizes[i].direct = (byte & 0b1000) > 0;
            if (vertexSizes[i].direct) {
                numberOfReferencedFields -= 1;
            }

            // Welp, goodbye, stardard calculations
            // 1 - 0 - 000 - 1 byte objects
            // 2 - 1 - 001 - 2 1 byte objects, 2 byte objects
            // 3 - 2 - 010 - 3 1 byte objects
            // 4 - 3 - 011 - 4 1 byte objects, 2 2 byte objects, 4 byte objects
            // 6 - 4 * 1 + 2 - 101 - 3 2 byte objects
            // 8 - 4 * 2 - 110 - 4 2 byte objects, 2 4 byte objects, 8 byte objects (?)
            // 12 - 4 * 3 - 101 - 3 4 byte objetcs
            // 16 - 4 * 4 - 111 - 4 4 byte objects, 2 8 byte objects (?)
            // maybe allow 24 and 32, though they are counted as 2 attributes

            vertexSizes[i].numBytes = ((byte & 0b11) + 1) * (1 + 3 * ((byte >> 2) & 0b1)) + (((byte & 0b111) == 0b101) << 1);
            
        }

        // step 2 - Vertex Data Pointers
        unsigned int* vertexPointers = (unsigned int*)input;

        input += sizeof(unsigned int) * numberOfReferencedFields;

        // step 3 - Construct Vertices
        
        unsigned int arraySize = numVertices;
        arraySize *= numberOfCPUFields;

        unsigned char* resultData = new unsigned char[arraySize];
        unsigned char* position = resultData;

        GLuint VBO;
        glGenBuffers(1,&VBO);

        for (unsigned short i = 0; i < numVertices; i++) {
            unsigned char fieldNumber = 0;
            for (unsigned char j = 0; j < numberOfCPUFields; j++) {
                unsigned char count = vertexSizes[j].numBytes;
                if (vertexSizes[j].direct) {
                    if (count & 0b111 == 0) { // 8 bytes
                        for (unsigned char k = 0; k < count >> 3; k++) {
                            ((unsigned long*)position)[k] = ((unsigned long*)input)[k];
                        }
                    } else if (count & 0b11 == 0) { // 4 bytes
                        for (unsigned char k = 0; k < count >> 2; k++) {
                            ((unsigned int*)position)[k] = ((unsigned int*)input)[k];
                        }
                    } else if (count & 0b1 == 0) { // 2 bytes
                        for (unsigned char k = 0; k < count >> 1; k++) {
                            ((unsigned short*)position)[k] = ((unsigned short*)input)[k];
                        }
                    } else {
                        for (unsigned char k = 0; k < count; k++) {
                            position[k] = input[k];
                        }
                    }
                    input += count;
                } else {
                    unsigned int index = *((unsigned short*)input);
                    if (count & 0b111 == 0) { // 8 bytes
                        index *= count >> 3;
                        for (unsigned char k = 0; k < count >> 3; k++) {
                            ((unsigned long*)position)[k] = ((unsigned long*)(start + vertexPointers[fieldNumber]))[index + k];
                        }
                    } else if (count & 0b11 == 0) { // 4 bytes
                        index *= count >> 2;
                        for (unsigned char k = 0; k < count >> 2; k++) {
                            ((unsigned int*)position)[k] = ((unsigned int*)(start + vertexPointers[fieldNumber]))[index + k];
                        }
                    } else if (count & 0b1 == 0) { // 2 bytes
                        index *= count >> 1;
                        for (unsigned char k = 0; k < count >> 1; k++) {
                            ((unsigned short*)position)[k] = ((unsigned short*)(start + vertexPointers[fieldNumber]))[index + k];
                        }
                    } else {
                        index *= count;
                        for (unsigned int k = 0; k < count; k++) {
                            position[k] = (start + vertexPointers[fieldNumber])[index + k];
                        }
                    }
                    fieldNumber++;
                    input += sizeof(unsigned short);
                }
                position += count;
            }
        }*/
    //}

    tinygltf::TinyGLTF loader;

    tinygltf::Model* loadModelFromId(NamespacedId id) {
        // Make a file manager
        std::string file = "models/" + id.namespaceName + "/" + id.id;

        /*int fd = open(file.c_str(), O_RDONLY);

        struct stat stat_buf;
        fstat(fd, &stat_buf);
        int numBytes = stat_buf.st_size;

        unsigned char* modeldata = (unsigned char*)mmap(NULL, numBytes, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

        close(fd);

        unsigned char version = *modeldata;
        Model* result;

        switch (version) {
            default: {
                result = createModelVersion1(modeldata + 1);
            }
        }

        munmap(modeldata, numBytes);*/
        tinygltf::Model* result;
        std::string err, warn;

        bool success = loader.LoadASCIIFromFile(result, &err, &warn, file);

        if (!warn.empty()) {
            std::cout << "Warn: " << warn << std::endl;
        }

        if (!err.empty()) {
            std::cout << "Error: " << err << std::endl;
        }

        if (!success) {
            result = nullptr;
        }

        return result;
    }

    // Takes in a compact generic vertices file
    /*Model loadModel(char* input) {
        Model result;
        unsigned char* start = (unsigned char*)input;

        // step 0 - metadata
        result.version = *((unsigned char*)input);
        input += sizeof(unsigned char);
        unsigned char numberOfFields = *((unsigned char*)input);
        input += sizeof(unsigned char);
        unsigned char numberOfReferencedFields = numberOfFields;
        result.numVertices = *((unsigned short*)input);
        input += sizeof(unsigned short);

        // step 1 - Vertex Format
        result.vertexSize = 0;

        struct VertexSizes{
            bool direct;
            unsigned char numBytes;
        } vertexSizes[16];

        unsigned char byte;

        for (unsigned char i = 0; i < numberOfFields; i++) {
            if ((i & 0b1) != 0) {
                byte >>= 4;
            } else {
                byte = *input;
                input++;
            }
            vertexSizes[i].direct = (byte & 0b1000) > 0;
            if (vertexSizes[i].direct) {
                numberOfReferencedFields -= 1;
            }

            // Welp, goodbye, stardard calculaions
            // 1 - 0 - 000 - 1 byte objects
            // 2 - 1 - 001 - 2 1 byte objects, 2 byte objects
            // 3 - 2 - 010 - 3 1 byte objects
            // 4 - 3 - 011 - 4 1 byte objects, 2 2 byte objects, 4 byte objects
            // 6 - 4 * 1 + 2 - 101 - 3 2 byte objects
            // 8 - 4 * 2 - 110 - 4 2 byte objects, 2 4 byte objects, 8 byte objects (?)
            // 12 - 4 * 3 - 101 - 3 4 byte objetcs
            // 16 - 4 * 4 - 111 - 4 4 byte objects, 2 8 byte objects (?)
            // maybe allow 24 and 32, though they are counted as 2 attributes

            vertexSizes[i].numBytes = ((byte & 0b11) + 1) * (1 + 3 * ((byte >> 2) & 0b1)) + (((byte & 0b111) == 0b101) << 1);
            
        }

        // step 2 - Vertex Data Pointers
        unsigned int* vertexPointers = (unsigned int*)input;

        input += sizeof(unsigned int) * numberOfReferencedFields;

        // step 3 - Construct Vertices
        
        unsigned int arraySize = result.numVertices;
        arraySize *= numberOfFields;

        unsigned char*& resultData = (unsigned char*&)(result.vertexData);
        resultData = new unsigned char[arraySize];
        unsigned char* position = resultData;

        for (unsigned short i = 0; i < result.numVertices; i++) {
            unsigned char fieldNumber = 0;
            for (unsigned char j = 0; j < numberOfFields; j++) {
                unsigned char count = vertexSizes[j].numBytes;
                if (vertexSizes[j].direct) {
                    if (count & 0b111 == 0) { // 8 bytes
                        for (unsigned char k = 0; k < count >> 3; k++) {
                            ((unsigned long*)position)[k] = ((unsigned long*)input)[k];
                        }
                    } else if (count & 0b11 == 0) { // 4 bytes
                        for (unsigned char k = 0; k < count >> 2; k++) {
                            ((unsigned int*)position)[k] = ((unsigned int*)input)[k];
                        }
                    } else if (count & 0b1 == 0) { // 2 bytes
                        for (unsigned char k = 0; k < count >> 1; k++) {
                            ((unsigned short*)position)[k] = ((unsigned short*)input)[k];
                        }
                    } else {
                        for (unsigned char k = 0; k < count; k++) {
                            position[k] = input[k];
                        }
                    }
                    input += count;
                } else {
                    unsigned int index = *((unsigned short*)input);
                    if (count & 0b111 == 0) { // 8 bytes
                        index *= count >> 3;
                        for (unsigned char k = 0; k < count >> 3; k++) {
                            ((unsigned long*)position)[k] = ((unsigned long*)(start + vertexPointers[fieldNumber]))[index + k];
                        }
                    } else if (count & 0b11 == 0) { // 4 bytes
                        index *= count >> 2;
                        for (unsigned char k = 0; k < count >> 2; k++) {
                            ((unsigned int*)position)[k] = ((unsigned int*)(start + vertexPointers[fieldNumber]))[index + k];
                        }
                    } else if (count & 0b1 == 0) { // 2 bytes
                        index *= count >> 1;
                        for (unsigned char k = 0; k < count >> 1; k++) {
                            ((unsigned short*)position)[k] = ((unsigned short*)(start + vertexPointers[fieldNumber]))[index + k];
                        }
                    } else {
                        index *= count;
                        for (unsigned int k = 0; k < count; k++) {
                            position[k] = (start + vertexPointers[fieldNumber])[index + k];
                        }
                    }
                    fieldNumber++;
                    input += sizeof(unsigned short);
                }
                position += count;
            }
        }
        
        return result;

    }

    // Theoretically speaking, the largest file size for a model this format supports is a little less than 19 Megabytes of model data, though I might adjust some things to allow for non gpu data, do note that this only stores the geometry, no textures or material data

    Model loadModelFromFile(std::string file) {
        int fd = open("models/debugTower.mdl", O_RDONLY);

        struct stat stat_buf;
        fstat(fd, &stat_buf);
        int numBytes = stat_buf.st_size;

        void* modeldata = (float*)mmap(NULL, numBytes, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

        close(fd);

        Model result = loadModel(modeldata);

        munmap(modeldata, numBytes);

        return result;
    }*/

    GeneralizedLoadableRegistry modelRegistry(31,(LoaderFunction)loadModelFromId,(UnloaderFunction)(operator delete));

}
