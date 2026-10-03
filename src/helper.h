#pragma once
#include <SFML/System/Vector2.hpp>

// Helper functions will be contained in this file. 

// wrap around position to keep it within the range [-1, 1] for both x and y coordinates
inline void wrapAround (sf::Vector2f& pos) {
    if (pos.x > 1.0f) 
        pos.x = pos.x - 2.0f;
    if (pos.x < -1.0f) 
        pos.x = pos.x + 2.0f;
    if (pos.y > 1.0f) 
        pos.y = pos.y - 2.0f;
    if (pos.y < -1.0f) 
        pos.y = pos.y + 2.0f;
}