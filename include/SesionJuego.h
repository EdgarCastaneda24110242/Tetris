#pragma once

#include <string>
using namespace std;

class SesionJuego {
private:
    string fechaInicio;
    int duracion;
public:
    void iniciarSesion();
    void finalizarSesion();
};
