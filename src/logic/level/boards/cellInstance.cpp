/******************************************************************************
* File: cellInstance.cpp
* Author: Tony Skeps
* Date: July 9, 2023
******************************************************************************/

#include "cellInstance.hpp"

namespace Game::Logic::Level::Boards {

    bool CellInstance::setColor(Boards::CellColor newColor) {
        if (newColor == color || locked) {
            return false;
        }
        color = newColor;
        for (auto iter = listeners.begin(); iter != listeners.end(); iter++) {
            (*iter)->onThisCellChange(/*board*/);
        }
        return true;
    }

    void CellInstance::reset() {
        locked = false;
        color = originalColor;
    }

}
