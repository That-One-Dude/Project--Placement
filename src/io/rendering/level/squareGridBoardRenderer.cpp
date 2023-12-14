/******************************************************************************
* File: squareGridBoardRenderer.cpp
* Author: Tony Skeps
* Date: July 19, 2023
******************************************************************************/

#include <iostream>

#include "../../../gl/glFunctions.hpp"
#include "../../../gl/glMath.hpp"
#include "../../../logic/level/boards/squareGridBoard.hpp"
#include "squareGridBoardRenderer.hpp"

using namespace Game::GL;

namespace Game::IO::Rendering::Level {

    struct CellVertex {
        vec3 pos;
        unsigned int cellIndex;
        vec2 border;
    };

    class: public GenericBoardRenderer<Logic::Level::Boards::SquareGridBoard> {
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
        void beginLevel(const Logic::Level::Boards::SquareGridBoard* board, const Logic::Level::BoardState& boardState) {
            
        }
        void render(const Logic::Level::Boards::SquareGridBoard* board, const Logic::Level::BoardState& boardState) {}
        // might want to change how this is handled for combo boards
        void fillVertexBuffer(const Logic::Level::Boards::SquareGridBoard* board, unsigned int buffer, unsigned int elementBuffer, unsigned int& vertexSize, unsigned short& numElements, std::unordered_map<const Logic::Level::Boards::Cell*, mat4>& transformMap) const {
            unsigned int numCells = board->getCells().size();

            vertexSize = sizeof(CellVertex);
            numElements = numCells * 6;
            
            CellVertex vertices[numCells * 4];
            short elements[numCells * 6];

            unsigned int xPos = 0, yPos = 0;

            for (int i = 0; i < numCells; i++) {
                while (!(board->getCellGrid()[yPos * board->xSize + xPos].is)) {
                    xPos++;
                    if (xPos >= board->xSize) {
                        xPos = 0;
                        yPos++;
                    }
                }

                const float sqrt2 = 1.417f;
                /*vertices[4 * i].cellIndex = i;
                vertices[4 * i].pos = vec3(2 * (xPos / (float)board->xSize) - 1, 2 * (yPos / (float)board->ySize) - 1, 0.0f);
                vertices[4 * i].border = vec2(-1.0f / sqrt2, -1.0f / sqrt2);
                //std::cout << vertices[4 * i].pos.x << ", " << vertices[4 * i].pos.y << std::endl;
                vertices[4 * i + 1].cellIndex = i;
                vertices[4 * i + 1].pos = vec3(2 * ((xPos + 1) / (float)board->xSize) - 1, 2 * (yPos / (float)board->ySize) - 1, 0.0f);
                vertices[4 * i + 1].border = vec2(1.0f / sqrt2, -1.0f / sqrt2);
                //std::cout << vertices[4 * i + 1].pos.x << ", " << vertices[4 * i + 1].pos.y << std::endl;
                vertices[4 * i + 2].cellIndex = i;
                vertices[4 * i + 2].pos = vec3(2 * (xPos / (float)board->xSize) - 1, 2 * ((yPos + 1) / (float)board->ySize) - 1, 0.0f);
                vertices[4 * i + 2].border = vec2(-1.0f / sqrt2, 1.0f / sqrt2);
                //std::cout << vertices[4 * i + 2].pos.x << ", " << vertices[4 * i + 2].pos.y << std::endl;
                vertices[4 * i + 3].cellIndex = i;
                vertices[4 * i + 3].pos = vec3(2 * ((xPos + 1) / (float)board->xSize) - 1, 2 * ((yPos + 1) / (float)board->ySize) - 1, 0.0f);
                vertices[4 * i + 3].border = vec2(1.0f / sqrt2, 1.0f / sqrt2);
                //std::cout << vertices[4 * i + 3].pos.x << ", " << vertices[4 * i + 3].pos.y << std::endl;*/

                float xOffset = (float)(board->xSize) / 2;
                float yOffset = (float)(board->ySize) / 2;

                vertices[4 * i].cellIndex = i;
                vertices[4 * i].pos = vec3(xPos - xOffset, yPos - yOffset, 0.0f);
                vertices[4 * i].border = vec2(-1.0f / sqrt2, -1.0f / sqrt2);
                vertices[4 * i + 1].cellIndex = i;
                vertices[4 * i + 1].pos = vec3(xPos - xOffset + 1.0f, yPos - yOffset, 0.0f);
                vertices[4 * i + 1].border = vec2(1.0f / sqrt2, -1.0f / sqrt2);
                vertices[4 * i + 2].cellIndex = i;
                vertices[4 * i + 2].pos = vec3(xPos - xOffset, yPos - yOffset + 1.0f, 0.0f);
                vertices[4 * i + 2].border = vec2(-1.0f / sqrt2, 1.0f / sqrt2);
                vertices[4 * i + 3].cellIndex = i;
                vertices[4 * i + 3].pos = vec3(xPos - xOffset + 1.0f, yPos - yOffset + 1.0f, 0.0f);
                vertices[4 * i + 3].border = vec2(1.0f / sqrt2, 1.0f / sqrt2);

                elements[6 * i] = 4 * i;
                elements[6 * i + 1] = 4 * i + 2;
                elements[6 * i + 2] = 4 * i + 1;
                elements[6 * i + 3] = 4 * i + 1;
                elements[6 * i + 4] = 4 * i + 2;
                elements[6 * i + 5] = 4 * i + 3;

                //std::cout << "test " << xPos << ", " << yPos << std::endl;

                transformMap[board->getCells().at(i)] = mat4(
                    0.5f, 0.0f, 0.0f, (xPos + 0.5) - xOffset,
                    0.0f, 0.5f, 0.0f, (yPos + 0.5) - yOffset,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                );

                xPos++;
                if (xPos >= board->xSize) {
                    xPos = 0;
                    yPos++;
                }

            }


            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(CellVertex) * numCells * 4, vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * numCells * 6, elements, GL_STATIC_DRAW);
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
    } value;
    
    BoardRenderer* const squareGridBoardRenderer = &value;

}
