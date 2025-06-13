#pragma once
#include <SFML/Graphics/Color.hpp>

class ColorPieza {
public:
    static sf::Color obtener(int tipo) {
        switch (tipo) {
            case 1: return sf::Color(102,255,255);
            case 2: return sf::Color(255,255,102);
            case 3: return sf::Color(255,102,255);
            case 4: return sf::Color(102,255,102);
            case 5: return sf::Color(255,102,102);
            case 6: return sf::Color(102,102,255);
            case 7: return sf::Color(255,178,102);
            default: return sf::Color::White;
        }
    }
};
