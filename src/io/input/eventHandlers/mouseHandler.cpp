/******************************************************************************
* File: mouseHandler.cpp
* Author: Tony Skeps
* Date: June 15, 2022
******************************************************************************/

#include "mouseHandler.hpp"
#include "../../../universalData.hpp"

#include <iostream>
#include "../../rendering/renderingManager.hpp"

namespace Game::IO::Input::EventHandlers {

    float mouseSensitivity = 0.007f;

    bool LeftHeld = false, RightHeld = false, ForwardHeld = false, BackHeld = false;

    bool handleMouseEvents(const sf::Window& window, const sf::Event& event, InputData& data) {

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::W) {
                if (!ForwardHeld) {
                    data.up = true;
                    ForwardHeld = true;
                    return true;
                } 
            } else if (event.key.code == sf::Keyboard::S) {
                if (!BackHeld) {
                    data.down = true;
                    BackHeld = true;
                    return true;
                }
            } else if (event.key.code == sf::Keyboard::A) {
                if (!LeftHeld) {
                    data.left = true;
                    LeftHeld = true;
                    return true;
                }
            } else if (event.key.code == sf::Keyboard::D) {
                if (!RightHeld) {
                    data.right = true;
                    RightHeld = true;
                    return true;
                }
            } else if (event.key.code == sf::Keyboard::Escape) {
                Rendering::releaseMouseControl();
                return true;
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::W) {
                if (ForwardHeld) {
                    data.up = false;
                    ForwardHeld = false;
                    return true;
                }
            } else if (event.key.code == sf::Keyboard::S) {
                if (BackHeld) {
                    data.down = false;
                    BackHeld = false;
                    return true;
                }
            } else if (event.key.code == sf::Keyboard::A) {
                if (LeftHeld) {
                    data.left = false;
                    LeftHeld = false;
                    return true;
                }
            } else if (event.key.code == sf::Keyboard::D) {
                if (RightHeld) {
                    data.right = false;
                    RightHeld = false;
                    return true;
                }
            }
        }

        if (event.type == sf::Event::LostFocus) {
            Rendering::releaseMouseControl();
            return true;
        }

        return false;

    }

}
