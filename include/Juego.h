#pragma once

#include "Jugador.h"
#include "Tablero.h"
#include "Nivel.h"
#include "InterfazUsuario.h"

class Juego {
private:
    Jugador* jugador;
    Tablero* tablero;
    Nivel* nivel;
public:
    void iniciar();
    void reiniciar();
};
