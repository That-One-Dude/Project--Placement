/******************************************************************************
* File: fileManager.hpp
* Author: Tony Skeps
* Date: September 17, 2022
******************************************************************************/

#include <unordered_map>
#include <vector>
#include "namespacedId.hpp"

#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

namespace Game {

    std::vector<std::string> gamePacks;

    void reload();

    std::string grabFile(NamespacedId);

    std::string iterateFiles(std::string);

}

#endif