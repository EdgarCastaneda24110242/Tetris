#pragma once
#include <iostream>

class Puntos
{
private:
    int puntaje;

public:
    Puntos();                       // Constructor para inicializar el puntaje
    void AgregarPuntos(int lineas); // Agrega puntos según las líneas completadas
    int CalcularPuntaje() const;    // Devuelve el puntaje actual
    void Reiniciar();               // Reinicia el puntaje a cero
};
