/******************************************************************************
* File: renderingManager.cpp
* Author: Tony Skeps
* Date: June 3, 2022
******************************************************************************/

#include <unordered_map>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <chrono>
#include "../../logic/gameStates/gameState.hpp"
#include "../../gl/glFunctions.hpp"
#include "../../gl/glMath.hpp"
#include "../../universalData.hpp"
#include "../ioUniversalData.hpp"
#include "batchRenderer.hpp"
#include "stateRenderer.hpp"
#include "gameStates/main/mainGameStateRenderer.hpp"
#include "renderingManager.hpp"

using namespace Game::GL;

namespace Game::IO::Rendering {

    bool mouseControl = true;

    int waitTicks = 0;

    int oldViewportXSize, oldViewportYSize;
    bool ViewportChanged = false;

    std::unordered_map<std::string, StateRenderer> stateRenderers;

    void setFPS(int fps) {
        waitTicks = 1000000000 / fps;
    }

    void releaseMouseControl() {
        mouseControl = false;
    }

    int frameCount = 0;

    unsigned int shaderProgram;
    GLuint VAO;

    void debugRenderFunction(int numBytes) {

        glUseProgram(shaderProgram);
        //glBindVertexArray(VAO);
        
    }

    void debugEnterFunction() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void debugExitFunction() {
        
    }

    float vertices[] = {
        // first triangle
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f,  0.5f, 0.0f,  // top left 
        // second triangle
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
    };

    void debugStateRendererFunction(Game::Logic::GameStates::GameState* state, BatchManager& batchManager) {

        batchManager.render(vertices, sizeof(vertices), BatchConfig((RenderFunction)debugRenderFunction, (EnterFunction)debugEnterFunction, (ExitFunction)debugExitFunction, 0, sizeof(float) * 3, VAO, "test"));
        
    }

    void noOp(Game::Logic::GameStates::GameState* state, unsigned int viewportSizeX, unsigned int viewportSizeY) {
        
    }

    #ifdef DEBUG
    void GLAPIENTRY MessageCallback( GLenum source,
                    GLenum type,
                    GLuint id,
                    GLenum severity,
                    GLsizei length,
                    const GLchar* message,
                    const void* userParam )
    {
    /*fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
                type, severity, message );*/
    }
    #endif


    void renderLoop(bool* active, std::mutex* logicLock, std::mutex* windowLock, sf::RenderWindow* window) {


        stateRenderers["noOp"] = noOp;
        stateRenderers["mainGameState"] = (StateRenderer)GameStates::Main::renderMainGameState;

        setFPS(60);

        windowLock->lock();
        window->setActive(true);

        oldViewportXSize = viewportXSize;
        oldViewportYSize = viewportYSize;

        //BatchManager batchManager(128, 1 << 30);

        GameStates::Main::mainGameStateInit();

        #ifdef DEBUG
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
        #endif

        if (!sf::Shader::isAvailable()) {
            std::cout << "Shaders are not available" << std::endl;
            // ERROR
            return;
        }

        // Debug Stuff

        float vertices[] = {
            // first triangle
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f,  0.5f, 0.0f,  // top left 
            // second triangle
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left
        };

        const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";
        
        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";
        
        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        glGenVertexArrays(1, &VAO);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
        glCullFace(GL_FRONT);

        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

        std::chrono::steady_clock::time_point start, end;

        windowLock->unlock();

        while (*active) {

            start = std::chrono::steady_clock::now();

            windowLock->lock();
            window->setActive(true);

            if (mouseControl) {
                //sf::Mouse::setPosition(sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2), *window);
            }

            if (viewportXSize != oldViewportXSize || viewportYSize != oldViewportYSize) {
                glViewport(0, 0, viewportXSize, viewportYSize);
                oldViewportXSize = viewportXSize;
                oldViewportYSize = viewportYSize;
                viewportRatio = ((float)viewportXSize) / viewportYSize;
                camera = GL::perspective(fov, viewportRatio, 0.1f, 300.0f);
            }
            //windowLock->unlock();

            glClear(GL_DEPTH_BUFFER_BIT);

            logicLock->lock();
            Game::Logic::GameStates::GameState* currentGameState = Game::gameState.getCurrentGameState();

            stateRenderers[currentGameState->rendererName](currentGameState, viewportXSize, viewportYSize);
            logicLock->unlock();

            //batchManager.finishRenderFrame();

            //windowLock->lock();

            window->display();

            window->setActive(false);
            windowLock->unlock();

            { // FPS debug

                frameCount++;

                std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

                std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

                if (time_span.count() > 1.0) {
                    std::cout << "Frames Per Second: " << (frameCount / time_span.count()) << std::endl;
                    frameCount = 0;
                    t1 = t2;
                }

            }

            end = std::chrono::steady_clock::now();
            std::chrono::nanoseconds duration = std::chrono::nanoseconds(waitTicks) - (end - start);

            if (duration.count() > 0) {
                std::this_thread::sleep_for(duration);
            } else {
                std::this_thread::yield();
            }

        }

    }

}
