/******************************************************************************
* File: symbolRenderer.cpp
* Author: Tony Skeps
* Date: August 14, 2023
******************************************************************************/

#include <iostream>

#include "../../../logic/level/boardState.hpp"
#include "../../../logic/level/boards/cellInstance.hpp"
#include "symbolRenderer.hpp"

namespace Game::IO::Rendering::Level {

    int correctTexture, wrongTexture;

    struct SymbolVertex {
        vec3 pos;
        vec2 uv;
    };

    unsigned int vbo, vao, ebo;
    unsigned int shaderProgram;
    int transformUniformLocation;
    int symbolTextureUniformLocation;
    int symbolColorUniformLocation;

    mat4 cellTransformation;

    void initSymbols() {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        size_t offset = 0;
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, false, offset);
        glVertexAttribBinding(0, 0);
        offset += sizeof(float) * 3;

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 2, GL_FLOAT, false, offset);
        glVertexAttribBinding(1, 0);
        offset += sizeof(float) * 2;

        glBindVertexArray(0);

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        SymbolVertex vertices[6] {
            {vec3(-1.0f, -1.0f, 0.0f), vec2(0.0f, 0.0f)},
            {vec3(-1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)},
            {vec3(1.0f, -1.0f, 0.0f), vec2(1.0f, 0.0f)},
            {vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)}
        };

        unsigned short indices[] {0, 1, 2, 2, 1, 3};

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        const char *vertexShaderSource = "#version 440 core\n"
            "layout (location = 0) in vec3 pos;\n"
            "layout (location = 1) in vec2 uv;\n"

            "uniform mat4 transform;\n"

            "out VS_OUT {\n"
            "   vec2 uv;\n"
            "   vec2 screenPos;\n"
            "} vs_out;\n"

            "void main()\n"
            "{\n"
            "   vs_out.uv = uv;\n"
            //"   vs_out.uv.z = max(0, min(3 - 1, floor(uv.z + 0.5)) );\n"
            "   gl_Position = transform * vec4(pos.xyz, 1.0);\n"
            "   vs_out.screenPos = (gl_Position.xy + vec2(1.0f, 1.0f)) / 2.0f;\n"
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
            //"uniform sampler2DMS screenTexture;\n"
            "uniform int symbolTexture;\n"
            "uniform int symbolColor;\n"

            "layout (std140, binding = 1) uniform palletData {\n"
            "   vec4 colors[256];\n"
            "};\n"

            "in VS_OUT {\n"
            "   vec2 uv;\n"
            "   vec2 screenPos;\n"
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
            "   FragColor = symbolcolor + (1 - symbolcolor.w) * blurcolor;\n"*/
            "   vec4 symbolpixel = texture(symbols, vec3(fs_in.uv, symbolTexture));\n"
            "   vec4 color;\n"
            "   vec4 borderColor;\n"
            "   if (symbolColor == 255) {\n"
            "       color = vec4(0.0f,0.0f,0.0f,1.0f);\n"
            "       borderColor = vec4(vec3(1.0f, 1.0f, 1.0f) * symbolpixel.w, 1 - ((1 - symbolpixel.x) * symbolpixel.w));\n"
            //"       color = texelFetch(screenTexture, ivec2(gl_FragCoord), 0);\n"
            //"       if (symbolpixel.r == 1.0f) discard;\n"
            "   } else {\n"
            "       color = colors[symbolColor];\n"
            "       borderColor = vec4(vec3(1.0f, 1.0f, 1.0f) - color.xyz, symbolpixel.w);\n"
            "   }\n"
            "   vec4 truecolor = symbolpixel.x * color + (1.0f - symbolpixel.x) * borderColor;\n"
            "   FragColor = vec4(truecolor.xyz, borderColor.w);\n"
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
        int symbolsTexture = glGetUniformLocation(shaderProgram, "symbols");
        glUniform1i(symbolsTexture, 1);
        //int screenTexture = glGetUniformLocation(shaderProgram, "screenTexture");
        //glUniform1i(screenTexture, 3);
        transformUniformLocation = glGetUniformLocation(shaderProgram, "transform");
        symbolTextureUniformLocation = glGetUniformLocation(shaderProgram, "symbolTexture");
        symbolColorUniformLocation = glGetUniformLocation(shaderProgram, "symbolColor");
    }

    void bindSymbolStuff() {
        glUseProgram(shaderProgram);

        glBindVertexArray(vao);
        glBindVertexBuffer(0, vbo, 0, sizeof(SymbolVertex));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    void SymbolRenderer::render(const Logic::Level::BoardState& state, const Logic::Level::Symbols::SymbolInstance& symbolInstance, mat4 cellTransform) {
        cellTransformation = cellTransform;
        glUniformMatrix4fv(transformUniformLocation, 1, false, (float*)&cellTransform);
        
        Logic::Level::Boards::CellColor color = symbolInstance.symbol.getColor();
        if (color == Logic::Level::Boards::nocell) {
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA);
            //color = state.getCellColors().at(&(symbolInstance.cell)).getColor();
        }
        glUniform1i(symbolColorUniformLocation, color);

        renderInternal(state, symbolInstance);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (symbolInstance.symbol.showSatisfaction) {
            glUniformMatrix4fv(transformUniformLocation, 1, false, (float*)&cellTransform);
            if (symbolInstance.isSatisfied()) {
                glUniform1i(symbolColorUniformLocation, 2);
                drawTexture(correctTexture);
            } else {
                glUniform1i(symbolColorUniformLocation, 1);
                drawTexture(wrongTexture);
            }
        }
    }

    void SymbolRenderer::renderFloating(const Logic::Level::BoardState& state, const Logic::Level::Boards::Cell* cell, const Logic::Level::Symbols::Symbol& symbol, mat4 cellTransform) {
        cellTransformation = cellTransform;
        glUniformMatrix4fv(transformUniformLocation, 1, false, (float*)&cellTransform);
        
        Logic::Level::Boards::CellColor color = symbol.getColor();
        if (color == Logic::Level::Boards::nocell) {
            glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA);
            //color = state.getCellColors().at(&(symbolInstance.cell)).getColor();
        }
        glUniform1i(symbolColorUniformLocation, color);

        renderInternal(state, cell, symbol);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void SymbolRenderer::drawTexture(mat4 transformation, unsigned int texture) {
        mat4 newTransform = cellTransformation * transformation;

        glUniformMatrix4fv(transformUniformLocation, 1, false, (float*)&newTransform);

        drawTexture(texture);
    }

    void SymbolRenderer::drawTexture(unsigned int texture) {
        glUniform1i(symbolTextureUniformLocation, texture);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    }

}
