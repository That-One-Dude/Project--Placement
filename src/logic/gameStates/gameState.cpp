/******************************************************************************
* File: gameState.cpp
* Author: Tony Skeps
* Date: June 11, 2022
******************************************************************************/

#include "gameState.hpp"

namespace Game::Logic::GameStates {

    GameState::GameState(GameState* previous, const std::string& rendererName): previous(previous), rendererName(rendererName) {
        if (previous != nullptr) {
            holder = previous->holder;
            *holder = this;
        } else {
            holder = nullptr;
        }
    };

    GameState::GameState(GameState** holder, const std::string& rendererName): previous(nullptr), holder(holder), rendererName(rendererName) {};

    GameState::~GameState() {}

    GameState* GameState::getPrevious() {
        return previous;
    }

    void GameState::exit() {
        *holder = previous;
    }

    BaseGameState::BaseGameState(): GameState(&holder, "noOp"), holder(this) {}

    BaseGameState::~BaseGameState() {}

    void BaseGameState::tick() {
        holder->tick();
    }

    void BaseGameState::recieveInput(IO::Input::InputData data) {
        if (holder != this) {
            holder->recieveInput(data);
        }
    }

    GameState* BaseGameState::getCurrentGameState() {
        return holder;
    }

}