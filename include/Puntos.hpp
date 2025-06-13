#pragma once
#include <iostream>

class Puntos {
private:
    int puntaje;

public:
    Puntos(); // Constructor para inicializar el puntaje
    void agregarPuntos(int lineas); // Agrega puntos según las líneas completadas
    int obtenerPuntaje() const; // Devuelve el puntaje actual
    void reiniciar(); // Reinicia el puntaje a cero
};
