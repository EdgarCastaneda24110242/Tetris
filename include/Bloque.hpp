#pragma once
#include <SFML/Graphics.hpp>

class Bloque {
public:
    static void dibujar(sf::RenderWindow& w, int x, int y, int size, int scale, sf::Color c, int a = 255) {
        sf::RectangleShape b(sf::Vector2f(size*scale-2, size*scale-2));
        b.setFillColor(sf::Color(c.r, c.g, c.b, a));
        b.setOutlineThickness(2);
        b.setOutlineColor(sf::Color(200,200,200,a));
        b.setPosition(x, y); w.draw(b);

        sf::RectangleShape s(sf::Vector2f(size*scale-6, 6));
        s.setFillColor(sf::Color(80,80,80,a));
        s.setPosition(x+4, y+size*scale-10); w.draw(s);
        s.setSize(sf::Vector2f(6, size*scale-6));
        s.setPosition(x+size*scale-10, y+4); w.draw(s);

        sf::RectangleShape br(sf::Vector2f(size*scale-6, 4));
        br.setFillColor(sf::Color(255,255,255,60*a/255));
        br.setPosition(x+2, y+2); w.draw(br);
        br.setSize(sf::Vector2f(4, size*scale-6));
        br.setPosition(x+2, y+2); w.draw(br);
    }
};
