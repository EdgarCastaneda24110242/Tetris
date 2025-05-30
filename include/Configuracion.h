#pragma once

#include <string>
using namespace std;

class Configuracion {
private:
    string dificultad;
public:
    void cambiarDificultad(const string& nuevaDificultad);
};
