/******************************************************************************
* File: levelLoader.hpp
* Author: Tony Skeps
* Date: April 22, 2023
******************************************************************************/

#include <string>
#include "levelData.hpp"

#ifndef LEVEL_LOADER_HPP
#define LEVEL_LOADER_HPP

namespace Game::Logic::Level {

    // Format:

    // 4 byte magic
    // PLCv where v is the version
    
    // 4 byte offset to board id
    // 4 byte offset to board data
    // unsigned short noting how many preplaced symbols there are
    // unsigned short noting how many placeable symbols there are
    // {unsigned short cell; unsigned char color; 4 byte offset to cell id; 4 byte offset to cellData} []
    // {unsigned char color; 4 byte offset to cell id; 4 byte offset to cellData}
    // random assorted information for ids and data

    LevelData* loadLevel(std::string fileName);

    LevelData* loadLevel(void* data);

    //void saveLevel(std::string levelName, Level* level);

    void saveLevel(std::string fileName, LevelData*);
}

#endif