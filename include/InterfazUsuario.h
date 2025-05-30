#pragma once

#include <string>
using namespace std;

class InterfazUsuario {
public:
    void mostrarMensaje(const string& mensaje);
    void actualizarVista();
};
