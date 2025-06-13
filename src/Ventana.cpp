#include "Ventana.hpp"

Ventana::Ventana(int ancho, int alto, const std::string& titulo)
    : window(sf::VideoMode(ancho, alto), titulo) {}

bool Ventana::estaAbierta() const {
    return window.isOpen();
}

void Ventana::limpiar() {
    window.clear(sf::Color::Black);
}

void Ventana::mostrar() {
    window.display();
}

bool Ventana::obtenerEvento(sf::Event& evento) {
    return window.pollEvent(evento);
}

sf::RenderWindow& Ventana::getWindow() {
    return window;
}
