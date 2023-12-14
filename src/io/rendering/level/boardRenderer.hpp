/******************************************************************************
* File: boardRenderer.hpp
* Author: Tony Skeps
* Date: July 18, 2023
******************************************************************************/

#include <unordered_map>
#include "../../../gl/glFunctions.hpp"
#include "../../../gl/glMath.hpp"
#include "../../../logic/level/boards/board.hpp"

using namespace Game::GL;

#ifndef BOARD_RENDERER_HPP
#define BOARD_RENDERER_HPP

namespace Game::IO::Rendering::Level {

    /*struct CellVertex {
        vec3 pos;
        int cellIndex;
    };*/

    class BoardRenderer {
    public:
        virtual ~BoardRenderer() {}
        virtual void init() = 0;
        virtual void beginLevelInternal(Logic::Level::Boards::Board*, const Logic::Level::BoardState&) = 0;
        virtual void renderInternal(Logic::Level::Boards::Board*, const Logic::Level::BoardState&) = 0;
        virtual void addSymbolTransforms(Logic::Level::Boards::Board*, const Logic::Level::BoardState&, std::unordered_map<const Logic::Level::Boards::Cell*, mat4>& transformMap) const {}
        // Think about how this would work 
        virtual void fillVertexBufferInternal(const Logic::Level::Boards::Board*, unsigned int buffer, unsigned int elementBuffer, unsigned int& vertexSize, unsigned short& numElements, std::unordered_map<const Logic::Level::Boards::Cell*, mat4>& transformMap) const = 0;
        // The VAO and Vertex Shaders are regardless of the precise board
        virtual unsigned int getVertexAttributeObject() const = 0;
        virtual unsigned int getVertexShader() const = 0;
    };

    template <typename T> class GenericBoardRenderer: public BoardRenderer {
        virtual void beginLevel(const T*, const Logic::Level::BoardState&) = 0;
        virtual void render(const T*, const Logic::Level::BoardState&) = 0;
        virtual void fillVertexBuffer(const T*, unsigned int buffer, unsigned int elementBuffer, unsigned int& vertexSize, unsigned short& numElements, std::unordered_map<const Logic::Level::Boards::Cell*, mat4>& transformMap) const = 0;
        void beginLevelInternal(Logic::Level::Boards::Board* board, const Logic::Level::BoardState& boardState) {
            beginLevel(static_cast<T*>(board), boardState);
        }
        void renderInternal(Logic::Level::Boards::Board* board, const Logic::Level::BoardState& boardState) {
            render(static_cast<T*>(board), boardState);
        }
        void fillVertexBufferInternal(const Logic::Level::Boards::Board* board, unsigned int buffer, unsigned int elementBuffer, unsigned int& vertexSize, unsigned short& numElements, std::unordered_map<const Logic::Level::Boards::Cell*, mat4>& transformMap) const {
            fillVertexBuffer(static_cast<const T*>(board), buffer, elementBuffer, vertexSize, numElements, transformMap);
        }
    };

}

#endif
