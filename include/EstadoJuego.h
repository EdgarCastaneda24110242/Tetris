#pragma once

#include <string>
using namespace std;

class EstadoJuego {
private:
    string estadoActual;
public:
    void iniciarJuego();
    void pausarJuego();
    void finalizarJuego();
};
