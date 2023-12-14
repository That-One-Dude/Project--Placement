/******************************************************************************
* File: batchRenderer.cpp
* Author: Tony Skeps
* Date: May 27, 2022
******************************************************************************/

#include "../../gl/glFunctions.hpp"
#include "batchRenderer.hpp"
#include <iostream>
#include <chrono>

using namespace Game::GL;

namespace Game::IO::Rendering {

    void noOp() {}

    void noOp(const int& numBytes) {}

    // Batch Config

    BatchConfig::BatchConfig() : render(noOp), enter(noOp), exit(noOp), priority(0), bytesPerVertex(1), name("") {} 

    BatchConfig::BatchConfig(RenderFunction render, EnterFunction enter, ExitFunction exit, int priority, int bytesPerVertex, GLuint vao, std::string name) : render(render), enter(enter), exit(exit), priority(priority), bytesPerVertex(bytesPerVertex), vao(vao), name(name) {} 

    bool BatchConfig::operator==(const BatchConfig& other) const { 
        if (name != other.name) {
            return false;
        }
        return true;
    }

    bool BatchConfig::operator!=(const BatchConfig& other) const {return !(*this == other);}

    // BatchInternal

    class BatchInternal {
    public:
        BatchInternal();
        ~BatchInternal();

        void init(const int& maxNumBytes);

        void render();

        void add(void* newBytes, const int& numNewBytes);
        void setConfig(const BatchConfig& newConfig);
        void empty();

        bool isEmpty() const;
        int remainingCapacity() const;
        bool isEnoughRoom(const int& numAdditionalBytes) const;
        bool matches(const BatchConfig& config) const;
        int getPriority() const;

        BatchInternal& operator=(const BatchInternal& other) = default;
        BatchConfig config;
    private:
        

        GLuint vbo = 0;
        int numBytes;
        int maxNumBytes;
    };

    BatchInternal::BatchInternal() : numBytes(), maxNumBytes(), config() {}

    BatchInternal::~BatchInternal() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDeleteBuffers(1, &vbo);
    }

    void BatchInternal::init(const int& maxNumBytes) {
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, maxNumBytes, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        numBytes = 0;
        this->maxNumBytes = maxNumBytes;
    }

    void BatchInternal::render() {
        config.render(numBytes);
        glBindVertexArray(config.vao);
        glDrawArrays(GL_TRIANGLES, 0, numBytes / config.bytesPerVertex);
        glBindVertexArray(0);
        numBytes = 0;
    }

    void BatchInternal::add(void* newBytes, const int& numNewBytes) {
        /*int numNewLongs = numNewBytes >> 3;
        for (int i = 0; i < numNewLongs; i++) {
            ((long*)(bytes + numBytes))[i] = ((long*)newBytes)[i];
        }
        for (int i = numNewLongs << 3; i < numNewBytes; i++) {
            bytes[numBytes + i] = newBytes[i];
        }*/
        glBindVertexArray(config.vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, numBytes, numNewBytes, newBytes);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        numBytes += numNewBytes;
    }

    void BatchInternal::setConfig(const BatchConfig& newConfig) {
        glBindVertexArray(config.vao);
        config.exit();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        config = newConfig;
        glBindVertexArray(config.vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        config.enter();
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BatchInternal::empty() {
        numBytes = 0;
    }

    bool BatchInternal::isEmpty() const {
        return numBytes == 0;
    }

    int BatchInternal::remainingCapacity() const {
        return maxNumBytes - numBytes;
    }

    bool BatchInternal::isEnoughRoom(const int& numAdditionalBytes) const {
        return numAdditionalBytes <= (maxNumBytes - numBytes);
    }

    bool BatchInternal::matches(const BatchConfig& config) const {
        return this->config == config;
    }

    int BatchInternal::getPriority() const {
        return config.priority;
    }

    // BatchManager

    BatchManager::BatchManager(const int& maxNumBatches, const int& bytesPerBatch) : maxNumBatches(maxNumBatches), bytesPerBatch(bytesPerBatch), batches(new BatchInternal[maxNumBatches]) {
        for (int i = 0; i < maxNumBatches; i++) {
            batches[i].init(bytesPerBatch);
        }
    }

    BatchManager::~BatchManager() {
        delete[] batches;
    }

    void BatchManager::render(void* bytes, const int& numBytes, const BatchConfig& config) {

        if (numBytes > bytesPerBatch) {
            // ERROR
            return;
        }

        BatchInternal* toCheck = batches + 1;
        BatchInternal* toReplace = batches;

        if (toReplace->matches(config)) {
            if (!(toReplace->isEnoughRoom(numBytes))) {
                toReplace->render();
            }

            toReplace->add(bytes, numBytes);

            return;
        }

        int fullestSize = batches->remainingCapacity();
        bool notFoundEmpty = true;

        for (int i = 1; i < maxNumBatches; i++, toCheck++) {
            if (toCheck->matches(config)) {
                if (!(toCheck->isEnoughRoom(numBytes))) {
                    toCheck->render();
                }

                toCheck->add(bytes, numBytes);

                return;
            }

            if (notFoundEmpty) {
                int remainingCapacity = toCheck->remainingCapacity();
                if (remainingCapacity < fullestSize) {
                    toReplace = toCheck;
                    fullestSize = remainingCapacity;
                }

                if (toCheck->isEmpty()) {
                    toReplace = toCheck;
                    notFoundEmpty = false;
                }
            }
        }

        if (notFoundEmpty) {
            toReplace->render();
        }

        // TODO Rearrange based on priority
        // Do I want to use a heap or an array?
        toReplace->setConfig(config);
        toReplace->add(bytes, numBytes);

    }

    void BatchManager::finishRenderFrame() {

        //std::chrono::steady_clock::time_point start, end;

        //start = std::chrono::steady_clock::now();
        for (int i = 0; i < maxNumBatches; i++) {
            if (!batches[i].isEmpty()) {
                batches[i].render();
            }
        }

        //end = std::chrono::steady_clock::now();
        //std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

        //std::cout << "Duration: " << time_span.count() << std::endl;
    }
}
