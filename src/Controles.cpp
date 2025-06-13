#include "Controles.hpp"

Controles::Accion Controles::procesarEvento(const sf::Event& evento) {
    if (evento.type == sf::Event::KeyPressed) {
        switch (evento.key.code) {
            case sf::Keyboard::A: return MoverIzquierda;
            case sf::Keyboard::D: return MoverDerecha;
            case sf::Keyboard::S: return Bajar;
            case sf::Keyboard::W: return Rotar;
            case sf::Keyboard::Q: return Salir;
            default: return Ninguna;
        }
    }
    return Ninguna;
}
