/******************************************************************************
* File: mainGameState.cpp
* Author: Tony Skeps
* Date: June 12, 2022
******************************************************************************/

#include <cmath>
#include "../../../universalData.hpp"
#include "../../level/level.hpp"
#include "mainGameState.hpp"

#include "../../level/boards/squareGridBoard.hpp"
#include "../../level/boards/hexagonalGridBoard.hpp"
#include "../../level/boards/truncatedSquareGridBoard.hpp"
#include "../../level/symbols/symbolFactory.hpp"
#include "../../level/symbols/colorConditionSymbol.hpp"
#include "../../level/symbols/painterSymbol.hpp"
#include "../../level/symbols/lineDrawerSymbol.hpp"
#include "../../level/symbols/groupConditionSymbol.hpp"
#include "../../level/symbols/debugRegionColorerSymbol.hpp"
#include "../../level/symbols/mineConditionSymbol.hpp"
#include "../../level/symbols/singleTilePainterSymbol.hpp"
#include "../../level/levelLoader.hpp"
#include <iostream>

namespace Game::Logic::GameStates::Main {

    void init() {
        boardFactory.addFactory(NamespacedId("square_grid"), Level::Boards::squareGridBoardFactory);
        boardFactory.addFactory(NamespacedId("hexagonal_grid"), Level::Boards::hexagonalGridBoardFactory);
        boardFactory.addFactory(NamespacedId("truncated_square_grid"), Level::Boards::truncatedSquareGridBoardFactory);

        symbolFactory.addFactory(NamespacedId("painter"), Level::Symbols::painterFactory);
        symbolFactory.addFactory(NamespacedId("color_condition"), Level::Symbols::colorConditionFactory);
        symbolFactory.addFactory(NamespacedId("group_condition"), Level::Symbols::groupConditionFactory);
        symbolFactory.addFactory(NamespacedId("line_drawer"), Level::Symbols::lineDrawerFactory);
        symbolFactory.addFactory(NamespacedId("mine_condition"), Level::Symbols::mineConditionFactory);
        symbolFactory.addFactory(NamespacedId("single_tile_painter"), Level::Symbols::singleTilePainterFactory);
    }

    MainGameState::MainGameState(GameState* state): GameState(state, "mainGameState") {

        /*struct {
            unsigned int aSize = 4, bSize = 4, cSize = 4;
            Level::Boards::CellColor colors[37]{
                1, Level::Boards::nocell, Level::Boards::nocell, Level::Boards::nocell,
                Level::Boards::nocell, 1, 1, 2, Level::Boards::nocell,
                Level::Boards::nocell, 1, 1, 2, 1, Level::Boards::nocell,
                Level::Boards::nocell, 2, 2, 2, 1, 1, 1,
                Level::Boards::nocell, 1, 1, 2, 1, Level::Boards::nocell,
                Level::Boards::nocell, 1, 1, 2, Level::Boards::nocell,
                1, Level::Boards::nocell, Level::Boards::nocell, Level::Boards::nocell
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("hexagonal_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 3;

        unsigned char direction = 1;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->preplacedSymbols[0].cell = 0;
        levelData->preplacedSymbols[0].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), 2, true, (void*)0);
        levelData->preplacedSymbols[1].cell = 13;
        levelData->preplacedSymbols[1].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), 2, true, (void*)0);
        levelData->preplacedSymbols[2].cell = 21;
        levelData->preplacedSymbols[2].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), 2, true, (void*)0);


        levelData->numPreplacedSymbols = numSymbols;

        levelData->numPlaceableSymbols = 1;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];
        //levelData->placeableSymbols[0].count = 1;
        //levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("painter"), 3, (void*)0);
        levelData->placeableSymbols[0].count = 7;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("line_drawer"), Level::Boards::nocell, false, (void*)0);
        float distance = 15.0f;
        float initialAngle = -M_PI / 3 + (-M_PI / 3) / (levelData->placeableSymbols[0].count - 1);
        float angleChange = (-2 * (M_PI_2 + initialAngle)) / (levelData->placeableSymbols[0].count - 1);
        GL::vec2 offset = GL::vec2(0.0f, 8.0f);
        GL::vec2 vector = GL::vec2(distance, 0.0f);
        GL::mat2 initialTransform = GL::rotation2D(initialAngle);
        GL::mat2 changeTransform = GL::rotation2D(angleChange);
        levelData->placeableSymbols[0].initialOffset = (initialTransform * vector) + offset;
        levelData->placeableSymbols[0].initialDirection = (initialTransform * (changeTransform + GL::mat2(-1.0f, 0.0f, 0.0f, -1.0f))) * vector;
        levelData->placeableSymbols[0].recursiveTransformation = changeTransform;
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/arrow_mastery.plc", levelData);*/

        /*struct {
            unsigned int xSize = 2, ySize = 2;
            Level::Boards::CellColor colors[4]{
                6, 7,
                6, 7
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 0;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->numPlaceableSymbols = 2;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        levelData->placeableSymbols[0].count = 1;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("color_condition"), 6, true, (void*)0);
        GL::vec2 offset = GL::vec2(6.0f, -1.0f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(0.0f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 1;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("color_condition"), 7, true, (void*)0);
        offset = GL::vec2(6.0f, 1.0f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(0.0f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/introduction.plc", levelData);*/

        /*struct {
            unsigned int xSize = 2, ySize = 2;
            Level::Boards::CellColor colors[4]{
                2, 2,
                1, 1
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 0;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->numPlaceableSymbols = 2;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        levelData->placeableSymbols[0].count = 2;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("group_condition"), 4, true, (void*)0);
        GL::vec2 offset = GL::vec2(6.0f, -1.0f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(2.0f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 2;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("group_condition"), 3, true, (void*)0);
        offset = GL::vec2(6.0f, 1.0f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(2.0f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/match_region.plc", levelData);*/

        /*struct {
            unsigned int xSize = 4, ySize = 4;
            Level::Boards::CellColor colors[16]{
                1, 5, 1, 5,
                5, 1, 1, 1,
                1, 1, 1, 5,
                1, 5, 1, 1,
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 0;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->numPlaceableSymbols = 3;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        unsigned int mineCount1 = 1;
        unsigned int mineCount2 = 2;
        unsigned int mineCount3 = 3;

        levelData->placeableSymbols[0].count =1;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 5, true, &mineCount1);
        GL::vec2 offset = GL::vec2(7.0f, -2.2f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 1;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 5, true, &mineCount2);
        offset = GL::vec2(7.0f, 0.0f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[2].count = 1;
        levelData->placeableSymbols[2].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 5, true, &mineCount3);
        offset = GL::vec2(7.0f, 2.2f);
        levelData->placeableSymbols[2].initialOffset = offset;
        levelData->placeableSymbols[2].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[2].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/mine_intro_1.plc", levelData);*/

        /*struct {
            unsigned int xSize = 5, ySize = 5;
            Level::Boards::CellColor colors[25]{
                3, 3, 3, 3, 3,
                3, 7, 7, 7, 7,
                3, 7, 7, 7, 3,
                7, 7, 7, 7, 3,
                3, 3, 3, 3, 3,
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 16;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->preplacedSymbols[0].cell = 0;
        levelData->preplacedSymbols[0].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[1].cell = 1;
        levelData->preplacedSymbols[1].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[2].cell = 2;
        levelData->preplacedSymbols[2].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[3].cell = 3;
        levelData->preplacedSymbols[3].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[4].cell = 4;
        levelData->preplacedSymbols[4].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[5].cell = 5;
        levelData->preplacedSymbols[5].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[6].cell = 9;
        levelData->preplacedSymbols[6].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[7].cell = 10;
        levelData->preplacedSymbols[7].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[8].cell = 14;
        levelData->preplacedSymbols[8].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[9].cell = 15;
        levelData->preplacedSymbols[9].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[10].cell = 19;
        levelData->preplacedSymbols[10].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[11].cell = 20;
        levelData->preplacedSymbols[11].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[12].cell = 21;
        levelData->preplacedSymbols[12].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[13].cell = 22;
        levelData->preplacedSymbols[13].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[14].cell = 23;
        levelData->preplacedSymbols[14].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[15].cell = 24;
        levelData->preplacedSymbols[15].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);

        levelData->numPlaceableSymbols = 6;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        unsigned int mineCount1 = 2;
        unsigned int mineCount2 = 3;
        unsigned int mineCount3 = 4;
        unsigned int mineCount4 = 5;
        unsigned int mineCount5 = 6;

        levelData->placeableSymbols[0].count = 1;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 3, true, &mineCount1);
        GL::vec2 offset = GL::vec2(7.0f, -4.4f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 1;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 3, true, &mineCount2);
        offset = GL::vec2(7.0f, -2.2f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[2].count = 1;
        levelData->placeableSymbols[2].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 3, true, &mineCount3);
        offset = GL::vec2(7.0f, 0.0f);
        levelData->placeableSymbols[2].initialOffset = offset;
        levelData->placeableSymbols[2].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[2].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[3].count = 1;
        levelData->placeableSymbols[3].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 3, true, &mineCount4);
        offset = GL::vec2(7.0f, 2.2f);
        levelData->placeableSymbols[3].initialOffset = offset;
        levelData->placeableSymbols[3].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[3].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[4].count = 1;
        levelData->placeableSymbols[4].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 3, true, &mineCount5);
        offset = GL::vec2(7.0f, 4.4f);
        levelData->placeableSymbols[4].initialOffset = offset;
        levelData->placeableSymbols[4].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[4].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[5].count = 2;
        levelData->placeableSymbols[5].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("single_tile_painter"), 3, false, nullptr);
        offset = GL::vec2(9.2f, -0.7f);
        levelData->placeableSymbols[5].initialOffset = offset;
        levelData->placeableSymbols[5].initialDirection = GL::vec2(0.0f, 1.4f);
        levelData->placeableSymbols[5].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/mine_intro_3.plc", levelData);*/

        /*struct {
            unsigned int xSize = 5, ySize = 5;
            Level::Boards::CellColor colors[25]{
                2, 2, 4, 4, 4,
                2, 2, 4, 2, 2,
                2, 4, 2, 2, 4,
                Level::Boards::nocell, Level::Boards::nocell, 2, 2, 4,
                Level::Boards::nocell, Level::Boards::nocell, 2, 4, 4,
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 9;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->preplacedSymbols[0].cell = 2;
        levelData->preplacedSymbols[0].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[1].cell = 3;
        levelData->preplacedSymbols[1].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[2].cell = 4;
        levelData->preplacedSymbols[2].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[3].cell = 7;
        levelData->preplacedSymbols[3].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[4].cell = 11;
        levelData->preplacedSymbols[4].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[5].cell = 14;
        levelData->preplacedSymbols[5].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[6].cell = 17;
        levelData->preplacedSymbols[6].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[7].cell = 19;
        levelData->preplacedSymbols[7].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);
        levelData->preplacedSymbols[8].cell = 20;
        levelData->preplacedSymbols[8].symbol = symbolFactory.createSymbol(NamespacedId("color_condition"), Level::Boards::nocell, false, (void*)0);

        levelData->numPlaceableSymbols = 3;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        unsigned int mineCount1 = 1;
        unsigned int mineCount2 = 2;
        unsigned int mineCount3 = 3;

        levelData->placeableSymbols[0].count = 3;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 4, true, &mineCount1);
        GL::vec2 offset = GL::vec2(7.0f, -2.2f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 3;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 4, true, &mineCount2);
        offset = GL::vec2(7.0f, 0.0f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[2].count = 3;
        levelData->placeableSymbols[2].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 4, true, &mineCount3);
        offset = GL::vec2(7.0f, 2.2f);
        levelData->placeableSymbols[2].initialOffset = offset;
        levelData->placeableSymbols[2].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[2].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/mine_intro_2.plc", levelData);*/

        struct {
            unsigned int xSize = 3, ySize = 3;
            Level::Boards::CellColor colors[9]{
                2, 1, 2,
                1, 1, 1,
                2, 1, 2,
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 0;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->numPlaceableSymbols = 4;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        unsigned int mineCount1 = 1;
        unsigned int mineCount2 = 2;
        unsigned int mineCount3 = 3;

        levelData->placeableSymbols[0].count = 1;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 2, true, &mineCount1);
        GL::vec2 offset = GL::vec2(7.0f, -2.2f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 1;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 2, true, &mineCount2);
        offset = GL::vec2(7.0f, 0.0f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[2].count = 1;
        levelData->placeableSymbols[2].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 2, true, &mineCount3);
        offset = GL::vec2(7.0f, 2.2f);
        levelData->placeableSymbols[2].initialOffset = offset;
        levelData->placeableSymbols[2].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[2].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[3].count = 1;
        levelData->placeableSymbols[3].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("single_tile_painter"), 2, false, nullptr);
        offset = GL::vec2(9.2f, 0.0f);
        levelData->placeableSymbols[3].initialOffset = offset;
        levelData->placeableSymbols[3].initialDirection = GL::vec2(0.0f, 1.4f);
        levelData->placeableSymbols[3].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/mine_intro_4.plc", levelData);

        /*struct {
            unsigned int xSize = 4, ySize = 4;
            Level::Boards::CellColor colors[16]{
                1, 5, 1, 5,
                5, 1, 1, 1,
                1, 1, 1, 5,
                1, 5, 1, 1,
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 0;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->numPlaceableSymbols = 3;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        unsigned int mineCount1 = 1;
        unsigned int mineCount2 = 2;
        unsigned int mineCount3 = 3;

        levelData->placeableSymbols[0].count =1;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 5, true, &mineCount1);
        GL::vec2 offset = GL::vec2(7.0f, -2.2f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 1;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 5, true, &mineCount2);
        offset = GL::vec2(7.0f, 0.0f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[2].count = 1;
        levelData->placeableSymbols[2].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 5, true, &mineCount3);
        offset = GL::vec2(7.0f, 2.2f);
        levelData->placeableSymbols[2].initialOffset = offset;
        levelData->placeableSymbols[2].initialDirection = GL::vec2(2.2f, 0.0f);
        levelData->placeableSymbols[2].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/mine_intro_1.plc", levelData);*/

        /*struct {
            unsigned int xSize = 4, ySize = 4;
            Level::Boards::CellColor colors[41]{
                1, 2, 3, 4,
                2, 3, 4, 1,
                3, 4, 1, 2,
                4, 1, 2, 3,

                5, 6, 5, 6, 5,
                6, 5, 6, 5, 6,
                5, 6, 5, 6, 5,
                6, 5, 6, 5, 6,
                5, 6, 5, 6, 5,
            };
        } boardData;

        Level::LevelData* levelData = new Level::LevelData();
        Level::Boards::Board* board = boardFactory.createBoard(NamespacedId("truncated_square_grid"), &boardData);
        levelData->board = board;

        levelData->transform = GL::mat4(
            0.75f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.75f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );

        unsigned int numSymbols = 0;

        levelData->preplacedSymbols = new Level::PreplacedSymbol[numSymbols];

        levelData->numPreplacedSymbols = numSymbols;

        levelData->numPlaceableSymbols = 2;
        levelData->placeableSymbols = new Level::PlaceableSymbol[levelData->numPlaceableSymbols];

        unsigned int numMines = 2;

        levelData->placeableSymbols[0].count = 1;
        levelData->placeableSymbols[0].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("mine_condition"), 6, true, &numMines);
        GL::vec2 offset = GL::vec2(6.0f, -1.0f);
        levelData->placeableSymbols[0].initialOffset = offset;
        levelData->placeableSymbols[0].initialDirection = GL::vec2(0.0f, 0.0f);
        levelData->placeableSymbols[0].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);

        levelData->placeableSymbols[1].count = 1;
        levelData->placeableSymbols[1].symbol = symbolFactory.createPlaceableSymbol(NamespacedId("painter"), 7, true, (void*)0);
        offset = GL::vec2(6.0f, 1.0f);
        levelData->placeableSymbols[1].initialOffset = offset;
        levelData->placeableSymbols[1].initialDirection = GL::vec2(0.0f, 0.0f);
        levelData->placeableSymbols[1].recursiveTransformation = GL::mat2(1.0f, 0.0f, 0.0f, 1.0f);
        level = new Level::Level(levelData);

        Level::saveLevel("data/levels/prototype/chad_truncated_square_grid.plc", levelData);*/

        /*Level::LevelData* levelData = Level::loadLevel("data/levels/prototype/introduction.plc");
        level = new Level::Level(levelData);*/

    }

    void MainGameState::tick() {

        
        
    }

    void MainGameState::recieveInput(IO::Input::InputData data) {

        

    }

    Level::PlaceResult MainGameState::putDownSymbol(const Level::Boards::Cell* cell) {
        return level->placeSymbol(cell);
    }

    Level::PlacementData* MainGameState::pickUpSymbol(const Level::Symbols::Symbol* symbol) {
        return level->pickUpSymbol(symbol);
    }

    void MainGameState::hoverSymbol(const Level::Boards::Cell* cell) {
        level->hoverSymbol(cell);
    }

    void MainGameState::adjustSymbol(const Level::Boards::Cell* cell) {
        level->adjustSymbol(cell);
    }

}