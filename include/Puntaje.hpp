#pragma once

#include "IPuntaje.h"

class Puntaje : public IPuntaje {
private:
    int puntajeActual;
public:
    void sumarPuntos(int puntos) override;
    void reiniciarPuntaje();
};
