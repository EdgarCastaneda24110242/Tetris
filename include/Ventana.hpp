#pragma once
#include <SFML/Graphics.hpp>

class Ventana {
public:
    Ventana(int ancho, int alto, const std::string& titulo);
    Ventana(sf::RenderWindow& ventanaExistente); // Constructor para aceptar una referencia a una ventana existente
    bool verificarSiEstaAbierta() const;
    void limpiar();
    void mostrar();
    bool procesarEvento(sf::Event& evento);
    sf::RenderWindow& obtenerVentana();

private:
    sf::RenderWindow* window; // Cambiado a puntero para evitar copia
};
