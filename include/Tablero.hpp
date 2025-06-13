#pragma once
#include <vector>
#include "Pieza.hpp"
#include <SFML/Graphics.hpp> // Asegúrate de incluir esto para usar sf::RenderWindow

class Tablero {
public:
    static const int ANCHO = 10;
    static const int ALTO = 20;
    std::vector<std::vector<int>> matriz;

    Tablero();
    bool colision(const Pieza& pieza) const;
    void fijarPieza(const Pieza& pieza);
    // Limpia las líneas completas y realiza una animación llamativa
    int limpiarLineas(sf::RenderWindow& ventana);
    void dibujar() const;
};
