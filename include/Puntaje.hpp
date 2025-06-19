#pragma once
#include <iostream>

class Puntaje
{
private:
    int puntaje;

public:
    Puntaje();                       // Constructor para inicializar el puntaje
    void AgregarPuntos(int lineas); // Agrega puntos según las líneas completadas
    int CalcularPuntaje() const;    // Devuelve el puntaje actual
    void Reiniciar();               // Reinicia el puntaje a cero
};
