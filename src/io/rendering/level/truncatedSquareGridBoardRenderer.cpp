/******************************************************************************
* File: truncatedSquareGridBoardRenderer.cpp
* Author: Tony Skeps
* Date: August 30, 2023
******************************************************************************/

#include <iostream>

#include <cmath>
#include "../../../gl/glFunctions.hpp"
#include "../../../gl/glMath.hpp"
#include "../../../logic/level/boards/truncatedSquareGridBoard.hpp"
#include "truncatedSquareGridBoardRenderer.hpp"

using namespace Game::GL;

namespace Game::IO::Rendering::Level {

    struct CellVertex {
        vec3 pos;
        unsigned int cellIndex;
        vec2 border;
    };

    class: public GenericBoardRenderer<Logic::Level::Boards::TruncatedSquareGridBoard> {
    public:
        void init() {
            glGenVertexArrays(1, &vertexAttributeObject);
            glBindVertexArray(vertexAttributeObject);

            size_t offset = 0;
            glEnableVertexAttribArray(0);
            glVertexAttribFormat(0, 3, GL_FLOAT, false, offset);
            glVertexAttribBinding(0, 0);
            offset += sizeof(float) * 3;

            glEnableVertexAttribArray(1);
            glVertexAttribIFormat(1, 1, GL_INT, offset);
            glVertexAttribBinding(1, 0);
            offset += sizeof(int) * 1;

            glEnableVertexAttribArray(2);
            glVertexAttribFormat(2, 2, GL_FLOAT, false, offset);
            glVertexAttribBinding(2, 0);
            offset += sizeof(float) * 2;

            glBindVertexArray(0);

            // change this to a registry later pls.
            const char *vertexShaderSource = "#version 440 core\n"
                "layout (location = 0) in vec3 pos;\n"
                "layout (location = 1) in int cellIndex;\n"
                "layout (location = 2) in vec2 border;\n"

                //"uniform vec3 offset;\n"

                /*"layout (std140, binding = 0) uniform sceneData {\n"
                "   mat4 camera;\n"
                "};\n"*/

                "int run(out vec2 outborder)\n"
                "{\n"
                /*"   vec3 movedPos = pos + offset;\n"
                "   vs_out.normal = normal;\n"
                "   vs_out.tangent = tangent;\n"
                "   vs_out.biTangent = biTangent;\n"
                "   vs_out.uv = uv;\n"*/
                "   gl_Position = vec4(pos.xyz, 1.0);\n"
                "   outborder = border;\n"
                "   return cellIndex;\n"
                "}\0";

            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);

            int  success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            }
        }
        void beginLevel(const Logic::Level::Boards::TruncatedSquareGridBoard* board, const Logic::Level::BoardState& boardState) {
            
        }
        void render(const Logic::Level::Boards::TruncatedSquareGridBoard* board, const Logic::Level::BoardState& boardState) {}
        // might want to change how this is handled for combo boards
        void fillVertexBuffer(const Logic::Level::Boards::TruncatedSquareGridBoard* board, unsigned int buffer, unsigned int elementBuffer, unsigned int& vertexSize, unsigned short& numElements, std::unordered_map<const Logic::Level::Boards::Cell*, mat4>& transformMap) const {
            unsigned int numCells = board->getCells().size();

            unsigned int numOctogons = board->truncationCellsStart;
            unsigned int numSquares = board->cellGridSize - numOctogons;

            unsigned int numVertices = numOctogons * 8 + numSquares * 4;

            vertexSize = sizeof(CellVertex);
            numElements = numOctogons * 18 + numSquares * 6;
            
            CellVertex vertices[numVertices];
            short elements[numElements];

            unsigned int xPos = 0, yPos = 0;

            // Octogons

            for (int i = 0; i < numOctogons; i++) {
                while (!(board->getCellGrid()[yPos * board->xSize + xPos].is)) {
                    xPos++;
                    if (xPos >= board->xSize) {
                        xPos = 0;
                        yPos++;
                    }
                }

                const float sinpiover8 = sin(M_PI / 8);
                const float cospiover8 = cos(M_PI / 8);

                const float vertexSin = (sinpiover8 / cospiover8) / 2;
                const float vertexCos = 0.5f;

                const float xOffset = (float)(board->xSize - 1) / 2;
                const float yOffset = (float)(board->ySize - 1) / 2;

                vertices[8 * i].cellIndex = i;
                vertices[8 * i].pos = vec3(vertexCos + xPos - xOffset, vertexSin + yPos - yOffset, 0.0f);
                vertices[8 * i].border = vec2(cospiover8, sinpiover8);
                vertices[8 * i + 1].cellIndex = i;
                vertices[8 * i + 1].pos = vec3(vertexSin + xPos - xOffset, vertexCos + yPos - yOffset, 0.0f);
                vertices[8 * i + 1].border = vec2(sinpiover8, cospiover8);
                vertices[8 * i + 2].cellIndex = i;
                vertices[8 * i + 2].pos = vec3(-vertexSin + xPos - xOffset, vertexCos + yPos - yOffset, 0.0f);
                vertices[8 * i + 2].border = vec2(-sinpiover8, cospiover8);
                vertices[8 * i + 3].cellIndex = i;
                vertices[8 * i + 3].pos = vec3(-vertexCos + xPos - xOffset, vertexSin + yPos - yOffset, 0.0f);
                vertices[8 * i + 3].border = vec2(-cospiover8, sinpiover8);
                vertices[8 * i + 4].cellIndex = i;
                vertices[8 * i + 4].pos = vec3(-vertexCos + xPos - xOffset, -vertexSin + yPos - yOffset, 0.0f);
                vertices[8 * i + 4].border = vec2(-cospiover8, -sinpiover8);
                vertices[8 * i + 5].cellIndex = i;
                vertices[8 * i + 5].pos = vec3(-vertexSin + xPos - xOffset, -vertexCos + yPos - yOffset, 0.0f);
                vertices[8 * i + 5].border = vec2(-sinpiover8, -cospiover8);
                vertices[8 * i + 6].cellIndex = i;
                vertices[8 * i + 6].pos = vec3(vertexSin + xPos - xOffset, -vertexCos + yPos - yOffset, 0.0f);
                vertices[8 * i + 6].border = vec2(sinpiover8, -cospiover8);
                vertices[8 * i + 7].cellIndex = i;
                vertices[8 * i + 7].pos = vec3(vertexCos + xPos - xOffset, -vertexSin + yPos - yOffset, 0.0f);
                vertices[8 * i + 7].border = vec2(cospiover8, -sinpiover8);
                
                elements[18 * i] = 8 * i;
                elements[18 * i + 1] = 8 * i + 2;
                elements[18 * i + 2] = 8 * i + 1;
                elements[18 * i + 3] = 8 * i;
                elements[18 * i + 4] = 8 * i + 3;
                elements[18 * i + 5] = 8 * i + 2;
                elements[18 * i + 6] = 8 * i;
                elements[18 * i + 7] = 8 * i + 4;
                elements[18 * i + 8] = 8 * i + 3;
                elements[18 * i + 9] = 8 * i;
                elements[18 * i + 10] = 8 * i + 5;
                elements[18 * i + 11] = 8 * i + 4;
                elements[18 * i + 12] = 8 * i;
                elements[18 * i + 13] = 8 * i + 6;
                elements[18 * i + 14] = 8 * i + 5;
                elements[18 * i + 15] = 8 * i;
                elements[18 * i + 16] = 8 * i + 7;
                elements[18 * i + 17] = 8 * i + 6;

                transformMap[board->getCells().at(i)] = mat4(
                    0.75f * 0.5f, 0.0f, 0.0f, xPos - xOffset,
                    0.0f, 0.75f * 0.5f, 0.0f, yPos - yOffset,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                );

                xPos++;
                if (xPos >= board->xSize) {
                    xPos = 0;
                    yPos++;
                }

            }

            // Squares

            xPos = 0;
            yPos = 0;

            for (int i = 0; i < numSquares; i++) {
                while (!(board->getCellGrid()[yPos * (board->xSize + 1) + xPos].is)) {
                    xPos++;
                    if (xPos >= (board->xSize + 1)) {
                        xPos = 0;
                        yPos++;
                    }
                }

                const float sinpiover8 = sin(M_PI / 8);
                const float cospiover8 = cos(M_PI / 8);

                const float vertexSin = (sinpiover8 / cospiover8) / 2;
                const float vertexCos = 0.5f;

                const float xOffset = (float)(board->xSize - 1) / 2;
                const float yOffset = (float)(board->ySize - 1) / 2;

                vertices[8 * numOctogons + 4 * i].cellIndex = numOctogons + i;
                vertices[8 * numOctogons + 4 * i].pos = vec3(-vertexSin + xPos - xOffset, -vertexCos + yPos - yOffset, 0.0f);
                vertices[8 * numOctogons + 4 * i].border = vec2(1.0f, 0.0f);
                vertices[8 * numOctogons + 4 * i + 1].cellIndex = numOctogons + i;
                vertices[8 * numOctogons + 4 * i + 1].pos = vec3(-vertexCos + xPos - xOffset, -vertexSin + yPos - yOffset, 0.0f);
                vertices[8 * numOctogons + 4 * i + 1].border = vec2(0.0f, 1.0f);
                vertices[8 * numOctogons + 4 * i + 2].cellIndex = numOctogons + i;
                vertices[8 * numOctogons + 4 * i + 2].pos = vec3(vertexSin - 1 + xPos - xOffset, -vertexCos + yPos - yOffset, 0.0f);
                vertices[8 * numOctogons + 4 * i + 2].border = vec2(-1.0f, 0.0f);
                vertices[8 * numOctogons + 4 * i + 3].cellIndex = numOctogons + i;
                vertices[8 * numOctogons + 4 * i + 3].pos = vec3(-vertexCos + xPos - xOffset, vertexSin - 1 + yPos - yOffset, 0.0f);
                vertices[8 * numOctogons + 4 * i + 3].border = vec2(0.0f, -1.0f);
                
                elements[18 * numOctogons + 6 * i] = 8 * numOctogons + 4 * i;
                elements[18 * numOctogons + 6 * i + 1] = 8 * numOctogons + 4 * i + 2;
                elements[18 * numOctogons + 6 * i + 2] = 8 * numOctogons + 4 * i + 1;
                elements[18 * numOctogons + 6 * i + 3] = 8 * numOctogons + 4 * i;
                elements[18 * numOctogons + 6 * i + 4] = 8 * numOctogons + 4 * i + 3;
                elements[18 * numOctogons + 6 * i + 5] = 8 * numOctogons + 4 * i + 2;

                transformMap[board->getCells().at(numOctogons + i)] = mat4(
                    0.40f * 0.5f, 0.0f, 0.0f, xPos - 0.5f - xOffset,
                    0.0f, 0.40f * 0.5f, 0.0f, yPos - 0.5f - yOffset,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                );

                xPos++;
                if (xPos >= (board->xSize + 1)) {
                    xPos = 0;
                    yPos++;
                }

            }


            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(CellVertex) * numVertices, vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * numElements, elements, GL_STATIC_DRAW);
        }
        virtual unsigned int getVertexAttributeObject() const {
            return vertexAttributeObject;
        }
        virtual unsigned int getVertexShader() const {
            return vertexShader;
        }
    private:
        unsigned int vertexAttributeObject;
        unsigned int vertexShader;
    } trueTruncatedSquareGridBoardRenderer;
    
    BoardRenderer* const truncatedSquareGridBoardRenderer = &trueTruncatedSquareGridBoardRenderer;

}
