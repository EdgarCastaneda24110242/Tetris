#pragma once

#include <vector>
using namespace std;

#include "Puntaje.h"

class RegistroPuntuacion {
private:
    vector<Puntaje*> historial;
public:
    void guardarPuntaje(Puntaje* puntaje);
};
