#include "Ventana.hpp"

Ventana::Ventana(int ancho, int alto, const std::string& titulo)
    : window(new sf::RenderWindow(sf::VideoMode(ancho, alto), titulo)) {} // Inicializa el puntero con una nueva instancia

Ventana::Ventana(sf::RenderWindow& ventanaExistente)
    : window(&ventanaExistente) {} // Almacena un puntero a la ventana existente

bool Ventana::estaAbierta() const {
    return window->isOpen(); // Usar -> para acceder al método
}

void Ventana::limpiar() {
    window->clear(sf::Color::Black); // Usar -> para acceder al método
}

void Ventana::mostrar() {
    window->display(); // Usar -> para acceder al método
}

bool Ventana::obtenerEvento(sf::Event& evento) {
    return window->pollEvent(evento); // Usar -> para acceder al método
}

sf::RenderWindow& Ventana::getWindow() {
    return *window; // Retornar la referencia al objeto apuntado
}
