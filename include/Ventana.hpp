#pragma once
#include <SFML/Graphics.hpp>

class Ventana
{
public:
    Ventana(int ancho, int alto, const std::string &titulo);
    Ventana(sf::RenderWindow &ventanaExistente); // Constructor para aceptar una referencia a una ventana existente
    bool VerificarSiEstaAbierta() const;
    void Limpiar();
    void Mostrar();
    bool ProcesarEvento(sf::Event &evento);
    sf::RenderWindow &ObtenerVentana();

private:
    sf::RenderWindow *window; // Cambiado a puntero para evitar copia
};
