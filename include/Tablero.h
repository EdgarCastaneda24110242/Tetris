#pragma once

#include <vector>
using namespace std;

#include "Celda.h"

class Tablero {
private:
    vector<vector<Celda>> celdas;
public:
    void limpiarLineas();
    void agregarPieza();
};
