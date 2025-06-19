#pragma once
#include "Tablero.hpp"
#include "Pieza.hpp"

class Tetris
{
public:
    Tetris();
    void Jugar();

private:
    Tablero tablero;
    Pieza *piezaActual;
    bool gameOver;
    void CrearNuevaPieza();
};
