/******************************************************************************
* File: mainGameStateRenderer.cpp
* Author: Tony Skeps
* Date: June 13, 2022
******************************************************************************/

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "../../../../gl/glFunctions.hpp"
#include "../../../../gl/glMath.hpp"
#include "../../../../logic/gameStates/main/mainGameState.hpp"
#include "../../../../universalData.hpp"
#include "../../../ioUniversalData.hpp"
#include "../../level/boardRenderer.hpp"
#include "../../level/squareGridBoardRenderer.hpp"
#include "../../level/hexagonalGridBoardRenderer.hpp"
#include "../../level/truncatedSquareGridBoardRenderer.hpp"
#include "../../level/symbolRenderer.hpp"
#include "../../level/painterSymbolRenderer.hpp"
#include "../../level/basicSymbolRenderer.hpp"
#include "../../level/colorConditionSymbolRenderer.hpp"
#include "../../level/lineDrawerSymbolRenderer.hpp"
#include "../../level/mineConditionSymbolRenderer.hpp"
#include "mainGameStateRenderer.hpp"

#include <iostream>

using namespace Game::GL;

namespace Game::IO::Rendering::GameStates::Main {

    struct SceneData {
        mat4 camera;
    };

    SceneData sceneData;

    vec4 palletData[256] {
        vec4(1.0f, 1.0f, 1.0f, 1.0f),
        vec4(0.929411764706f, 0.109803921569f, 0.141176470588f, 1.0f),
        vec4(0.141176470588f, 0.694117647059f, 0.298039215686f, 1.0f),
        vec4(163.0f / 255.0f, 73.0f / 255.0f, 164.0f / 255.0f, 1.0f),
        vec4(255.0f / 255.0f, 127.0f / 255.0f, 39.0f / 255.0f, 1.0f),
        vec4(255.0f / 255.0f, 242.0f / 255.0f, 0.0f / 255.0f, 1.0f),
        vec4(63.0f / 255.0f, 72.0f / 255.0f, 204.0f / 255.0f, 1.0f),
        vec4(153.0f / 255.0f, 217.0f / 255.0f, 234.0f / 255.0f, 1.0f),
    };

    struct associations{
        int index;
    } cellAssociations[1024];

    struct Vertex {
        vec3 pos;
        unsigned int cellIndex;
    } debugCellVertices[] {
        {
            vec3(-1, 1, 0), 0
        },
        {
            vec3(1, 1, 0), 0
        },
        {
            vec3(0, -1, 0), 0
        }
    };

    struct BoardVertex {
        vec3 pos;
        vec2 uv;
    } boardVertices[] {
        {vec3{-1.0f, -1.0f, 0.0f}, vec2{0.0f, 0.0f}},
        {vec3{-1.0f, 1.0f, 0.0f}, vec2{0.0f, 1.0f}},
        {vec3{1.0f, -1.0f, 0.0f}, vec2{1.0f, 0.0f}},
        {vec3{1.0f, 1.0f, 0.0f}, vec2{1.0f, 1.0f}}
    };

    unsigned short indices[] {0, 1, 2, 2, 1, 3};

    Level::BoardRenderer* boardRenderer = nullptr;

    std::unordered_map<const Logic::Level::Boards::Cell*, mat4> transformMap;

    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;

    unsigned short numElements;
    unsigned int vertexSize;

    unsigned int sceneDataUniformBuffer;
    unsigned int palletDataUniformBuffer, cellAssociationsDataUniformBuffer;

    unsigned int vertexShader, fragmentShader;
    unsigned int shaderProgram;

    int transformUniformLocation;

    unsigned int levelFrameBuffer;
    unsigned int levelTexture;

    unsigned int intermediateFrameBuffer;
    unsigned int intermediateTexture;

    unsigned int postFrameBuffer;
    unsigned int postTexture;
    unsigned int multisamplePostCellTexture;

    unsigned int intermediatePostFrameBuffer;
    unsigned int intermediatePostTexture;
    unsigned int postCellTexture;

    const unsigned int levelTextureWidth = 1024;

    unsigned int boardVbo;
    unsigned int boardVao;
    unsigned int boardEbo;

    unsigned int boardVertexShader, boardFragmentShader;
    unsigned int boardShaderProgram;

    std::unordered_map<NamespacedId, Level::BoardRenderer*, NamespacedIdHash> boardRenderers;
    std::unordered_map<NamespacedId, Level::SymbolRenderer*, NamespacedIdHash> symbolRenderers;

    const Logic::Level::Level* lastLevel = nullptr;
    unsigned int levelShader = 0;

    // Find a better system for this

    unsigned int arrayTexture;

    std::string textureNames[] {
        "data/images/symbols/painter.png",
        "data/images/symbols/requiresColor.png",
        "data/images/symbols/requiresPartner.png",
        "data/images/symbols/lineExtender.png",
        "data/images/symbols/singleTilePainter.png",
        "data/images/symbols/regionExpander.png",
        "data/images/symbols/mine.png",
        "data/images/symbols/ring.png",
        "data/images/symbols/noMine.png",
        "data/images/symbols/checkmark.png",
        "data/images/symbols/noSymbol.png",
        "data/images/symbols/itText.png",
        "data/images/symbols/meanText.png",
        "data/images/symbols/thisText.png",
        "data/images/symbols/baba.png",
        "data/images/symbols/flag.png"
    };

    const unsigned int textureWidth = 128, textureHeight = 128, numLayers = sizeof(textureNames) / sizeof(std::string);

    void mainGameStateInit() {

        boardRenderers[NamespacedId("square_grid")] = Level::squareGridBoardRenderer;
        boardRenderers[NamespacedId("hexagonal_grid")] = Level::hexagonalGridBoardRenderer;
        boardRenderers[NamespacedId("truncated_square_grid")] = Level::truncatedSquareGridBoardRenderer;
        for (auto iter = boardRenderers.begin(); iter != boardRenderers.end(); iter++) {
            iter->second->init();
        }

        Level::initSymbols();
        symbolRenderers[NamespacedId("painter")] = new Level::BasicSymbolRenderer(0);
        symbolRenderers[NamespacedId("color_condition")] = new Level::ColorConditionSymbolRenderer(1, 10);
        symbolRenderers[NamespacedId("group_condition")] = new Level::BasicSymbolRenderer(2);
        symbolRenderers[NamespacedId("line_drawer")] = new Level::LineDrawerSymbolRenderer(3);
        symbolRenderers[NamespacedId("mine_condition")] = new Level::MineConditionSymbolRenderer(6, 7, 8);
        symbolRenderers[NamespacedId("single_tile_painter")] = new Level::BasicSymbolRenderer(4);
        for (auto iter = symbolRenderers.begin(); iter != symbolRenderers.end(); iter++) {
            iter->second->init();
        }

        Level::correctTexture = 9;
        Level::wrongTexture = 8;

        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);
        /*glGenVertexArrays(1, &vao);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(debugCellVertices), debugCellVertices, GL_STATIC_DRAW);

        size_t offset = 0;
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, false, offset);
        glVertexAttribBinding(0, 0);
        offset += sizeof(float) * 3;

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 1, GL_INT, false, offset);
        glVertexAttribBinding(1, 0);
        offset += sizeof(int) * 1;*/

        glGenBuffers(1, &sceneDataUniformBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, sceneDataUniformBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(SceneData), nullptr, GL_STREAM_DRAW);

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, sceneDataUniformBuffer);

        glGenBuffers(1, &palletDataUniformBuffer);

        glBindBuffer(GL_UNIFORM_BUFFER, palletDataUniformBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(palletData), palletData, GL_STATIC_DRAW);

        glBindBufferBase(GL_UNIFORM_BUFFER, 1, palletDataUniformBuffer);

        glGenBuffers(1, &cellAssociationsDataUniformBuffer);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, cellAssociationsDataUniformBuffer);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(cellAssociations), nullptr, GL_STATIC_DRAW);
        //std::cout << cellAssociations[0].padding1 << cellAssociations[0].padding2 << cellAssociations[0].padding3 << std::endl;

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cellAssociationsDataUniformBuffer);

        //std::cout << glGetError() << " Y " << vbo << std::endl;

        const char *vertexShaderSource = "#version 440 core\n"

            "layout (std140, binding = 1) uniform palletData {\n"
            "   vec4 colors[256];\n"
            "};\n"

            "layout (std430, binding = 0) buffer cellAssociations {\n"
            "   int palletIndices[1024];\n"
            "};\n"

            "layout (std140, binding = 0) uniform sceneData {\n"
            "   mat4 camera;\n"
            "};\n"

            "layout (location = 0) uniform mat4 transform;\n"

            "out VS_OUT {\n"
            "   vec4 color;\n"
            "   vec3 normal;\n"
            "   flat int cellIndex;\n"
            "} vs_out;\n"

            "int run(out vec2 normal);\n"

            "void main()\n"
            "{\n"
            "   vec2 norm;\n"
            "   int cellIndex = run(norm);\n"
            "   vs_out.normal.xy = norm;\n"
            "   vs_out.color = colors[palletIndices[cellIndex]];\n"
            "   vs_out.cellIndex = cellIndex;\n"
            "   gl_Position.xy -= norm * 0.01;\n"
            "   gl_Position = camera * transform * gl_Position;\n"
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

        const char *fragmentShaderSource = "#version 420 core\n"

            "in VS_OUT {\n"
            "   vec4 color;\n"
            "   vec3 normal;\n"
            "   flat int cellIndex;\n"
            "} fs_in;\n"

            "layout(location = 0) out vec4 FragColor;\n"
            "layout(location = 1) out unsigned int CellValue;\n"

            "void main()\n"
            "{\n"
            /*"   vec2 vector0 =  normalize(fs_in.norm0.xy);\n"
            "   vec2 vector1 =  normalize(fs_in.norm1.xy);\n"
            "   vec2 vector2 =  normalize(fs_in.norm2.xy);\n"
            "   vec2 sum0 = vector0 + vector1;\n"
            "   vec2 sum1 = vector1 + vector2;\n"
            "   vec2 sum2 = vector2 + vector0;\n"
            //"   float checkA = dot(fs_in.normal, sum0) / dot(vector0, sum0);\n"
            //"   if (vector0.x == 0.0f && vector0.y == 0.0f) checkA = 0;\n"
            //"   float checkB = dot(fs_in.normal, sum1) / dot(vector1, sum1);\n"
            //"   if (vector1.x == 0.0f && vector1.y == 0.0f) checkB = 0;\n"
            //"   float checkC = dot(fs_in.normal, sum2) / dot(vector2, sum2);\n"
            //"   if (vector2.x == 0.0f && vector2.y == 0.0f) checkC = 0;\n"
            "   float checkA = fs_in.norm0.z + fs_in.norm1.z;\n"
            "   float checkB = fs_in.norm1.z + fs_in.norm2.z;\n"
            "   float checkC = fs_in.norm2.z + fs_in.norm0.z;\n"
            "   float check = max(checkA, max(checkB, checkC));\n"
            "   if (check > 0.9) {check = 1;} else check = 0;\n"*/
            //"   float check = length(fs_in.normal);\n"
            "   vec3 trueNorm = normalize(fs_in.normal);\n"
            "   float check = length(trueNorm.xy) * sqrt(2.0f);\n"
            "   FragColor = fs_in.color;// * check;\n"
            "   CellValue = fs_in.cellIndex;\n"
            "}\0";
        
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

        

        /*glGenFramebuffers(1, &levelFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, levelFrameBuffer);  

        glGenTextures(1, &levelTexture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, levelTexture);
  
        //glTexImage2D(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, levelTextureWidth, levelTextureWidth, GL_TRUE);

        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, levelTexture, 0);

        glGenFramebuffers(1, &intermediateFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFrameBuffer);  

        glGenTextures(1, &intermediateTexture);
        glBindTexture(GL_TEXTURE_2D, intermediateTexture);
  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, levelTextureWidth, levelTextureWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTexture, 0);*/



        glGenFramebuffers(1, &postFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, postFrameBuffer);

        glGenTextures(1, &postTexture);
        //glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, postTexture);

        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, viewportXSize, viewportYSize, GL_TRUE);

        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, postTexture, 0);

        glActiveTexture(GL_TEXTURE2);

        glGenRenderbuffers(1, &multisamplePostCellTexture);
        glBindRenderbuffer(GL_RENDERBUFFER, multisamplePostCellTexture);
  
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_R16UI, viewportXSize, viewportYSize);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, multisamplePostCellTexture);



        glGenFramebuffers(1, &intermediatePostFrameBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, intermediatePostFrameBuffer);

        glActiveTexture(GL_TEXTURE0);

        glGenTextures(1, &intermediatePostTexture);
        glBindTexture(GL_TEXTURE_2D, intermediatePostTexture);
  
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewportXSize, viewportYSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediatePostTexture, 0);

        glActiveTexture(GL_TEXTURE2);

        glGenRenderbuffers(1, &postCellTexture);
        glBindRenderbuffer(GL_RENDERBUFFER, postCellTexture);
  
        glRenderbufferStorage(GL_RENDERBUFFER, GL_R16UI, viewportXSize, viewportYSize);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, postCellTexture);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "issues" << std::endl;
        }

        postCellTexture = intermediatePostTexture;

        glActiveTexture(GL_TEXTURE0);



        glGenBuffers(1, &boardVbo);
        glGenVertexArrays(1, &boardVao);
        glGenBuffers(1, &boardEbo);

        glBindBuffer(GL_ARRAY_BUFFER, boardVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(boardVertices), boardVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, boardEbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(boardVao);

        size_t offset = 0;
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, false, offset);
        glVertexAttribBinding(0, 0);
        offset += sizeof(float) * 3;

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 2, GL_FLOAT, true, offset);
        glVertexAttribBinding(1, 0);
        offset += sizeof(float) * 2;

        glBindVertexArray(0);

        const char *boardVertexShaderSource = "#version 440 core\n"

            "layout (location = 0) in vec3 pos;\n"
            "layout (location = 1) in vec2 uv;\n"

            /*"layout (std140, binding = 0) uniform sceneData {\n"
            "   mat4 camera;\n"
            "};\n"*/

            "out VS_OUT {\n"
            "   vec2 uv;\n"
            "} vs_out;\n"

            "void main()\n"
            "{\n"
            "   gl_Position = vec4(pos.xyz, 1.0);\n"
            "   vs_out.uv = uv;\n"
            "}\0";

        boardVertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(boardVertexShader, 1, &boardVertexShaderSource, NULL);
        glCompileShader(boardVertexShader);

        glGetShaderiv(boardVertexShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(boardVertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX2::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        const char *boardFragmentShaderSource = "#version 420 core\n"

            "out vec4 FragColor;\n"

            "in VS_OUT {\n"
            "   vec2 uv;\n"
            "} fs_in;\n"

            "uniform sampler2D screenTexture;\n"

            "void main()\n"
            "{\n"
            "    FragColor = texture(screenTexture, fs_in.uv);\n"
            "}\0";
        
        boardFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(boardFragmentShader, 1, &boardFragmentShaderSource, NULL);
        glCompileShader(boardFragmentShader);

        glGetShaderiv(boardFragmentShader, GL_COMPILE_STATUS, &success);

        if(!success)
        {
            glGetShaderInfoLog(boardFragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        boardShaderProgram = glCreateProgram();
        glAttachShader(boardShaderProgram, boardVertexShader);
        glAttachShader(boardShaderProgram, boardFragmentShader);
        glLinkProgram(boardShaderProgram);

        // Find a better system for this
        glGenTextures(1, &arrayTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D_ARRAY, arrayTexture);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, textureWidth, textureHeight, numLayers);

        sf::Image image;

        for (int i = 0; i < numLayers; i++) {
            if (!image.loadFromFile(textureNames[i])) {
                std::cout << "Texture " << textureNames[i] << " did not load" << std::endl;
            }
            image.flipVertically();
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, textureWidth, textureHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        }

        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
        
    }

    void resizeTextures(unsigned int xSize, unsigned int ySize, unsigned int aaLevel) {

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, postTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, xSize, ySize, GL_TRUE);

        glActiveTexture(GL_TEXTURE2);

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisamplePostCellTexture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_R16UI, viewportXSize, viewportYSize, GL_TRUE);

        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, intermediatePostTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, xSize, ySize, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glActiveTexture(GL_TEXTURE2);

        glBindTexture(GL_TEXTURE_2D, postCellTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R16UI, xSize, ySize, 0, GL_RED, GL_UNSIGNED_SHORT, NULL);

    }

    void beginLevel(const Logic::Level::Level* currentLevel) {
        if (lastLevel != nullptr) {
            glDetachShader(shaderProgram, levelShader);
            transformMap.clear();
        }
        boardRenderer = boardRenderers.at(currentLevel->getLevelData()->board->getRendererId());
        levelShader = boardRenderer->getVertexShader();
        glAttachShader(shaderProgram, levelShader);
        glLinkProgram(shaderProgram);

        int  success;
        char infoLog[512];

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        vao = boardRenderer->getVertexAttributeObject();

        boardRenderer->fillVertexBufferInternal(currentLevel->getLevelData()->board, vbo, ebo, vertexSize, numElements, transformMap);


    }

    void renderLevel(const Game::Logic::Level::Level* level) {

        if (level != lastLevel) {
            beginLevel(level);
            lastLevel = level;
        }

        //glViewport(0, 0, levelTextureWidth, levelTextureWidth);

        //Logic::Level::Boards::CellColor* cellColors = (Logic::Level::Boards::CellColor*)glMapNamedBuffer(cellAssociationsDataUniformBuffer, GL_WRITE_ONLY);
        associations* cellColors = (associations*)glMapNamedBuffer(cellAssociationsDataUniformBuffer, GL_READ_WRITE);
        //int test;
        //glGetIntegerv( GL_MIN_MAP_BUFFER_ALIGNMENT, &test);
        //std::cout << test << std::endl;
        //int cellColors[1024];

        /*unsigned char* test = (unsigned char*)cellColors;

        std::cout << std::endl;

        for (int i = 0; i < sizeof(cellAssociations); i+= 16) {
            for (int j = 0; j < 16; j++) {
                std::cout << (short)test[i + j] << " ";
            }
            std::cout << std::endl;
        }*/

        int i = 0;
        for (auto iter = level->getCellColorOrder().begin(); iter != level->getCellColorOrder().end(); iter++) {
            cellColors[i].index = **iter;
            //std::cout << (unsigned int)**iter << " " << i << std::endl;
            i++;
        }

        //glNamedBufferSubData(cellAssociationsDataUniformBuffer, 0, sizeof(cellColors), cellColors);*/

        glUnmapNamedBuffer(cellAssociationsDataUniformBuffer);

        glUseProgram(shaderProgram);

        mat4 mat(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -3.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mat = mat * level->getLevelData()->transform;

        glUniformMatrix4fv(0, 1, false, (float*)&mat);
        //std::cout << "Test" << std::endl;
        glBindVertexArray(vao);
        //std::cout << glGetError() << " " << vao << std::endl;

        glBindVertexBuffer(0, vbo, 0, vertexSize);
        //std::cout << glGetError() << " " << vbo << std::endl;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        //std::cout << glGetError() << " " << ebo << std::endl;

        const GLenum bufferAttachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(sizeof(bufferAttachments) / sizeof(GLenum), bufferAttachments);

        glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        
        //std::cout << glGetError() << std::endl;

        Level::bindSymbolStuff();
        for (auto iter = level->getBoardState().getSymbolInstances().begin(); iter != level->getBoardState().getSymbolInstances().end(); iter++) {
            Level::SymbolRenderer* symbolRenderer = symbolRenderers.at(iter->second.symbol.getRendererId());
            symbolRenderer->render(level->getBoardState(), iter->second, camera * mat * transformMap.at(iter->first));
        }

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glViewport(0, 0, viewportXSize, viewportYSize);

        glEnable(GL_DEPTH_TEST);

        glUseProgram(boardShaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, intermediatePostTexture);

        glBindVertexArray(boardVao);

        glBindVertexBuffer(0, boardVbo, 0, sizeof(BoardVertex));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boardEbo);

        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

        Level::bindSymbolStuff();

        glDisable(GL_DEPTH_TEST);

        Logic::Level::PlacementData* placementData = level->getCurrentlyPickedUpSymbol();
        
        if (placementData != nullptr) {
            
        }

        for (auto iter = level->getPlacemenetData().begin(); iter != level->getPlacemenetData().end(); iter++) {
            Level::SymbolRenderer* symbolRenderer = symbolRenderers.at(iter->symbol->getRendererId());
            symbolRenderer->renderFloating(level->getBoardState(), nullptr, *iter->symbol, mat4(0.1f * viewportRatio, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f) * mat4(1.0f, 0.0f, 0.0f, iter->xPos, 0.0f, 1.0f, 0.0f, iter->yPos, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
        }


    }

    void renderMainGameState(Game::Logic::GameStates::Main::MainGameState* mainGameState, unsigned int viewportXSize, unsigned int viewportYSize) {

        sceneData.camera = camera;

        glBindBuffer(GL_UNIFORM_BUFFER, sceneDataUniformBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(SceneData), &sceneData);

        glBindFramebuffer(GL_FRAMEBUFFER, postFrameBuffer);

        glViewport(0, 0, viewportXSize, viewportYSize);

        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glClearColor(-1, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        renderLevel(mainGameState->getLevel());

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediatePostFrameBuffer);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glDrawBuffer(GL_COLOR_ATTACHMENT1);
        glBlitFramebuffer(0, 0, viewportXSize, viewportYSize, 0, 0, viewportXSize, viewportYSize, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0, 0, viewportXSize, viewportYSize, 0, 0, viewportXSize, viewportYSize, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(boardShaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, intermediatePostTexture);

        glUniform1i(glGetUniformLocation(boardShaderProgram, "screenTexture"), 0);

        glBindVertexArray(boardVao);

        glBindVertexBuffer(0, boardVbo, 0, sizeof(BoardVertex));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boardEbo);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

        /*unsigned short pixel = -1;

        glBindFramebuffer(GL_READ_FRAMEBUFFER, Rendering::GameStates::Main::intermediatePostFrameBuffer);
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glReadPixels(300, 400, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, &pixel);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        std::cout << pixel << std::endl;*/

    }

}