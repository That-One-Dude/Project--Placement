/******************************************************************************
* File: mainGameState.hpp
* Author: Tony Skeps
* Date: June 12, 2022
******************************************************************************/

#include <string>
#include "../../level/level.hpp"
#include "../gameState.hpp"

#ifndef MAIN_GAME_STATE_HPP
#define MAIN_GAME_STATE_HPP

namespace Game::Logic::GameStates::Main {

    void init();

    class MainGameState: public GameState {
    public:
        MainGameState(GameState*);
        virtual void tick();
        virtual void recieveInput(IO::Input::InputData);
        const Level::Level* getLevel() const {return level;}

        Level::PlaceResult putDownSymbol(const Level::Boards::Cell* cell);
        Level::PlacementData* pickUpSymbol(const Level::Symbols::Symbol* symbol);
        void hoverSymbol(const Level::Boards::Cell* cell);
        void adjustSymbol(const Level::Boards::Cell* cell);
    private:
        Level::Level* level = nullptr;
        bool upHeld = false, downHeld = false, leftHeld = false, rightHeld = false;
    };

}

#endif