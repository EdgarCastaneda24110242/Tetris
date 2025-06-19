#pragma once
#include <SFML/Graphics.hpp>

class FondoTablero
{
public:
    static void Dibujar(sf::RenderWindow &ventana, int winWidth, int winHeight, int offsetX, int offsetY, int ancho, int alto, float scale);
};
