#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tablero.hpp"
#include "Pieza.hpp"
#include "ColorPieza.hpp"

class TableroRender {
public:
    static void dibujarFondo(sf::RenderWindow& w, int winWidth, int winHeight, int boardOffsetX, int boardOffsetY, int boardW, int boardH, int scale);
    static void dibujarTablero(sf::RenderWindow& w, const Tablero& tablero, int boardOffsetX, int boardOffsetY, int blockSize, int scale, const std::vector<int>& lineasAnimadas = {}, int animFrames = 0, int maxFrames = 12);
    static void dibujarPieza(sf::RenderWindow& w, const Pieza& pieza, int boardOffsetX, int boardOffsetY, int blockSize, int scale);
};
