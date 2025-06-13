#pragma once
#include <SFML/Window/Event.hpp>

// Controles del usuario para Tetris:
//   - A: Mover izquierda
//   - D: Mover derecha
//   - S: Bajar rápido
//   - W: Rotar pieza
//   - Q: Salir del juego

class Controles {
public:
    enum Accion {
        Ninguna,
        MoverIzquierda,
        MoverDerecha,
        Bajar,
        Rotar,
        Salir
    };

    static Accion procesarEvento(const sf::Event& evento);
};
