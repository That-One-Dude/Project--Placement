/******************************************************************************
* File: mouseHandler.cpp
* Author: Tony Skeps
* Date: August 20, 2023
******************************************************************************/

#include "../../../../gl/glMath.hpp"
#include "../../../../gl/glFunctions.hpp"
#include "../../../../logic/gameStates/main/mainGameState.hpp"
#include "../../../rendering/gameStates/main/mainGameStateRenderer.hpp"
#include "../../../ioUniversalData.hpp"
#include "../../../../universalData.hpp"
#include "mouseHandler.hpp"

#include <iostream>

using namespace Game::GL;

namespace Game::IO::Input::GameStates::Main {

    bool handleMouseEvents(const sf::Window& window, const sf::Event& event, Logic::GameStates::Main::MainGameState* gameState) {
        
        

        if (event.type == sf::Event::MouseMoved) {
            if (gameState->getLevel()->getCurrentlyPickedUpSymbol() != nullptr) {
                Logic::Level::PlacementData* placementData = gameState->getLevel()->getCurrentlyPickedUpSymbol();
                float mouseXPos = 2 * (event.mouseMove.x / (float)window.getSize().x) - 1;
                float mouseYPos = -2 * (event.mouseMove.y / (float)window.getSize().y) + 1;

                placementData->xPos = (mouseXPos / 0.1f) / viewportRatio;
                placementData->yPos = (mouseYPos / 0.1f);

                unsigned short pixel = -1;

                glBindFramebuffer(GL_READ_FRAMEBUFFER, Rendering::GameStates::Main::intermediatePostFrameBuffer);
                glReadBuffer(GL_COLOR_ATTACHMENT1);
                glReadPixels(event.mouseMove.x, window.getSize().y - event.mouseMove.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, &pixel);
                glReadBuffer(GL_COLOR_ATTACHMENT0);
                
                const Logic::Level::Boards::Cell* cell = nullptr;

                if (pixel != (unsigned short)-1) cell = gameState->getLevel()->getLevelData()->board->getCells()[pixel];

                gameState->hoverSymbol(cell);

                //placementData->symbol->hover(gameState->getLevel()->getBoardState(), cell);
            }
            return true;
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            unsigned short pixel = -1;

            glBindFramebuffer(GL_READ_FRAMEBUFFER, Rendering::GameStates::Main::intermediatePostFrameBuffer);
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            glReadPixels(event.mouseButton.x, window.getSize().y - event.mouseButton.y, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_SHORT, &pixel);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            
            const Logic::Level::Boards::Cell* cell = nullptr;

            if (pixel != (unsigned short)-1) cell = gameState->getLevel()->getLevelData()->board->getCells()[pixel];

            if (event.mouseButton.button == sf::Mouse::Button::Left) {

                if (gameState->getLevel()->getCurrentlyPickedUpSymbol() != nullptr) {
                    gameState->putDownSymbol(cell);
                } else {
                    float mouseXPos = 2 * (event.mouseButton.x / (float)window.getSize().x) - 1;
                    float mouseYPos = -2 * (event.mouseButton.y / (float)window.getSize().y) + 1;

                    float truePosX = (mouseXPos / 0.1f) / viewportRatio;
                    float truePosY = (mouseYPos / 0.1f);

                    Logic::Level::Symbols::Symbol* symbol = nullptr;
                    for (auto iter = gameState->getLevel()->getPlacemenetData().begin(); iter != gameState->getLevel()->getPlacemenetData().end(); iter++) {
                        float diffX = iter->xPos - truePosX;
                        float diffY = iter->yPos - truePosY;
                        if (diffX <= 1.0f && diffX >= -1.0f && diffY <= 1.0f && diffY >= -1.0f) {
                            symbol = iter->symbol;
                            break;
                        }
                    }
                    if (cell != nullptr && symbol == nullptr) {
                        auto potentialCell = gameState->getLevel()->getBoardState().getSymbolInstances().find(cell);
                        if (potentialCell != gameState->getLevel()->getBoardState().getSymbolInstances().end()) {
                            symbol = &(potentialCell->second.symbol);
                        }
                    }
                    if (symbol != nullptr) {
                        Logic::Level::PlacementData* placementData = gameState->pickUpSymbol(symbol);
                        if (placementData != nullptr) {
                            placementData->xPos = truePosX;
                            placementData->yPos = truePosY;
                        }
                    }
                }
                return true;
            } else if (event.mouseButton.button == sf::Mouse::Button::Right) {
                if (gameState->getLevel()->getCurrentlyPickedUpSymbol() != nullptr) {
                    gameState->adjustSymbol(cell);
                }
            }
        }

        return false;

    }

}