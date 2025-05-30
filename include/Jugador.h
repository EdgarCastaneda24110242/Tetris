#pragma once

#include <string>
using namespace std;

#include "Puntaje.h"
#include "Configuracion.h"
#include "EntradaJugador.h"


class Jugador {
private:
    string nombre;
    Puntaje* puntaje;
    Configuracion* configuracion;
public:
    void actualizarPuntaje();
};
