#include "../include/Puntos.hpp"

Puntos::Puntos() : puntaje(0) {}

void Puntos::agregarPuntos(int lineas) {
    // Asigna puntos según el número de líneas completadas
    switch (lineas) {
        case 1: puntaje += 100; break; // 1 línea
        case 2: puntaje += 300; break; // 2 líneas
        case 3: puntaje += 500; break; // 3 líneas
        case 4: puntaje += 800; break; // 4 líneas (Tetris)
        default: break;
    }
}

int Puntos::obtenerPuntaje() const {
    return puntaje;
}

void Puntos::reiniciar() {
    puntaje = 0;
}
