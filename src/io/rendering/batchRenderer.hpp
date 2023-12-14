/******************************************************************************
* File: batchRenderer.hpp
* Author: Tony Skeps
* Date: May 25, 2022
******************************************************************************/

#include <string>
#include <SFML/OpenGL.hpp>

#ifndef BATCH_RENDERER_HPP
#define BATCH_RENDERER_HPP

namespace Game::IO::Rendering {

    void noOp(const int& numBytes);

    // Render Functions expect you to set things like the shader, as well as set uniforms
    typedef void (*RenderFunction) (const int& numBytes);
    // Enter Functions have a VAO bound, and expects you to enable vertex attributes and do other stuff related to initializing a VAO, except for binding the VBO
    typedef void (*EnterFunction) ();
    // Enter Functions have a VAO bound, and expects you to disable vertex attributes and do other stuff related to cleaning a VAO, except for unbinding the VBO
    typedef void (*ExitFunction) ();

    struct BatchConfig {
        RenderFunction render;
        EnterFunction enter;
        ExitFunction exit;
        int priority;
        int bytesPerVertex;
        GLuint vao;
        std::string name;

        BatchConfig();
        BatchConfig(RenderFunction render, EnterFunction enter, ExitFunction exit, int priority, int bytesPerVertex, GLuint vao, std::string name);
        
        bool operator==(const BatchConfig& other) const;
        
        bool operator!=(const BatchConfig& other) const;
    };

    class BatchInternal;

    class BatchManager {
    public:
        BatchManager(const int& maxNumBatches, const int& bytesPerBatch);
        ~BatchManager();

        BatchManager(const BatchManager& c) = delete;
        BatchManager& operator=(const BatchManager& c) = delete;

        void render(void* bytes, const int& numBytes, const BatchConfig& config);
        void finishRenderFrame();
    private:
        BatchInternal* batches;
        const int maxNumBatches;
        const int bytesPerBatch;
    };

}

#endif
