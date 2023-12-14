/******************************************************************************
* File: painterSymbolRenderer.cpp
* Author: Tony Skeps
* Date: August 12, 2023
******************************************************************************/

#include <iostream>

#include "../../../logic/level/boardState.hpp"
#include "../../../logic/level/boards/cellInstance.hpp"
#include "../../../logic/level/symbols/painterSymbol.hpp"
#include "painterSymbolRenderer.hpp"

namespace Game::IO::Rendering::Level {

    struct SymbolVertex {
        vec3 pos;
        vec3 uv;
    };

    /*class: public GenericSymbolRenderer<Logic::Level::Symbols::PainterSymbol> {
    public:
        void init() {*/
            /*// TODO generalize this
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            size_t offset = 0;
            glEnableVertexAttribArray(0);
            glVertexAttribFormat(0, 3, GL_FLOAT, false, offset);
            glVertexAttribBinding(0, 0);
            offset += sizeof(float) * 3;

            glEnableVertexAttribArray(1);
            glVertexAttribFormat(1, 3, GL_FLOAT, false, offset);
            glVertexAttribBinding(1, 0);
            offset += sizeof(float) * 3;

            glBindVertexArray(0);

            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            SymbolVertex vertices[6] {
                {vec3(-1.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f)},
                {vec3(-1.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)},
                {vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f)},
                {vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f)}
            };

            unsigned short indices[] {0, 1, 2, 2, 1, 3};

            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            const char *vertexShaderSource = "#version 440 core\n"
                "layout (location = 0) in vec3 pos;\n"
                "layout (location = 1) in vec3 uv;\n"

                "layout (location = 0) uniform mat4 transform;\n"

                "out VS_OUT {\n"
                "   vec3 uv;\n"
                "} vs_out;\n"

                "void main()\n"
                "{\n"
                "   vs_out.uv = uv;\n"
                //"   vs_out.uv.z = max(0, min(3 - 1, floor(uv.z + 0.5)) );\n"
                "   gl_Position = transform * vec4(pos.xyz, 1.0);\n"
                "}\0";

            unsigned int vertexShader;
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

            const char *fragmentShaderSource = "#version 440 core\n"

                "uniform sampler2DArray symbols;\n"
                "uniform int symbolColor;\n"

                "layout (std140, binding = 1) uniform palletData {\n"
                "   vec4 colors[256];\n"
                "};\n"

                "in VS_OUT {\n"
                "   vec3 uv;\n"
                "} fs_in;\n"

                "out vec4 FragColor;\n"

                "void main()\n"
                "{\n"
                /*"   vec3 blur[9] = vec3[](\n"
                /*"       vec3(-2.0f, -2.0f, 1.0f / 273.0f), vec3(-1.0f, -2.0f, 4.0f / 273.0f), vec3(0.0f, -2.0f, 7.0f / 273.0f), vec3(1.0f, -2.0f, 4.0f / 273.0f), vec3(2.0f, -2.0f, 1.0f / 273.0f),\n"
                "       vec3(-2.0f, -1.0f, 4.0f / 273.0f), vec3(-1.0f, -1.0f, 16.0f / 273.0f), vec3(0.0f, -1.0f, 26.0f / 273.0f), vec3(1.0f, -1.0f, 16.0f / 273.0f), vec3(2.0f, -1.0f, 4.0f / 273.0f),\n"
                "       vec3(-2.0f, 0.0f, 7.0f / 273.0f), vec3(-1.0f, 0.0f, 26.0f / 273.0f), vec3(0.0f, 0.0f, 41.0f / 273.0f), vec3(1.0f, 0.0f, 26.0f / 273.0f), vec3(2.0f, 0.0f, 7.0f / 273.0f),\n"
                "       vec3(-2.0f, 1.0f, 4.0f / 273.0f), vec3(-1.0f, 1.0f, 16.0f / 273.0f), vec3(0.0f, 1.0f, 26.0f / 273.0f), vec3(1.0f, 1.0f, 16.0f / 273.0f), vec3(2.0f, 1.0f, 4.0f / 273.0f),\n"
                "       vec3(-2.0f, 2.0f, 1.0f / 273.0f), vec3(-1.0f, 2.0f, 4.0f / 273.0f), vec3(0.0f, 2.0f, 7.0f / 273.0f), vec3(1.0f, 2.0f, 4.0f / 273.0f), vec3(2.0f, 2.0f, 1.0f / 273.0f)\n"*//*
                "       vec3(-sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f, 1.0f / 9.0f), vec3(0.0f, -1.0f, 1.0f / 9.0f), vec3(sqrt(2.0f) / 2.0f, -sqrt(2.0f) / 2.0f, 1.0f / 9.0f),\n"
                "       vec3(-1.0f, 0.0f, 1.0f / 9.0f), vec3(0.0f, 0.0f, 1.0f / 9.0f), vec3(1.0f, 0.0f, 1.0f / 9.0f),\n"
                "       vec3(-sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f, 1.0f / 9.0f), vec3(0.0f, 1.0f, 1.0f / 9.0f), vec3(sqrt(2.0f) / 2.0f, sqrt(2.0f) / 2.0f, 1.0f / 9.0f)\n"
                "   );\n"
                "   vec4 blurcolor = vec4(0.0f,0.0f,0.0f,0.0f);\n"
                "   for (int i = 0; i < 9; i++) {\n"
                "       blurcolor = max(texture(symbols, fs_in.uv + vec3(blur[i].xy * 0.05f,0.0f)), blurcolor);\n"
                "   }\n"
                "   blurcolor = blurcolor * vec4(vec3(1.0f, 1.0f, 1.0f) - colors[symbolColor].xyz, 1.0f);\n"
                "   vec4 symbolcolor = texture(symbols, fs_in.uv) * colors[symbolColor];\n"
                "   FragColor = symbolcolor + (1 - symbolcolor.w) * blurcolor;\n"*//*
                "   vec4 symbolpixel = texture(symbols, fs_in.uv);\n"
                "   vec4 color = colors[symbolColor];\n"
                "   vec4 borderColor = vec4(vec3(1.0f, 1.0f, 1.0f) - color.xyz, color.w);\n"
                "   vec4 truecolor = symbolpixel.x * color + (1.0f - symbolpixel.x) * borderColor;\n"
                "   FragColor = vec4(truecolor.xyz, symbolpixel.w);\n"
                "}\0";
            
            unsigned int fragmentShader;
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);

            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if(!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            glUseProgram(shaderProgram);
            unsigned int symbolsTexture = glGetUniformLocation(shaderProgram, "symbols");
            glUniform1i(symbolsTexture, 1);
            symbolColorUniformLocation = glGetUniformLocation(shaderProgram, "symbolColor");*/
    /*    }
    protected:
        void genericRender(const Logic::Level::BoardState& state, const Logic::Level::Symbols::SymbolInstance& symbolInstance, const Logic::Level::Symbols::PainterSymbol& symbol) {
            /*glUseProgram(shaderProgram);

            glUniformMatrix4fv(0, 1, false, (float*)&matrix);
            Logic::Level::Boards::CellColor color = symbol.getColor();
            if (color == Logic::Level::Boards::nocell) {
                color = state.getCellColors().at(&(symbolInstance.cell)).getColor();
            }
            glUniform1i(symbolColorUniformLocation, color);

            glBindVertexArray(vao);
            glBindVertexBuffer(0, vbo, 0, sizeof(SymbolVertex));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);*/

            /*drawTexture(0);
        }
    private:
        unsigned int vbo, vao, ebo;
        unsigned int shaderProgram;
        int symbolColorUniformLocation;
    };// value;*/

    //SymbolRenderer* const painterSymbolRenderer = &value;

}
