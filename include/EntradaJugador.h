#pragma once

#include "IEntradaJugador.h"

class EntradaJugador : public IEntradaJugador {
public:
    void leerEntrada() override;
};
