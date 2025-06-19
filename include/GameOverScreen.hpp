#ifndef GAMEOVERSCREEN_HPP
#define GAMEOVERSCREEN_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class GameOverScreen
{
public:
    explicit GameOverScreen(sf::RenderWindow &ventana);
    void Mostrar();

private:
    sf::RenderWindow &ventana;
};

#endif
