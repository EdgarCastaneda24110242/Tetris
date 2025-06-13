#include "../include/FondoTablero.hpp"
#include <SFML/Graphics.hpp>

void FondoTablero::dibujar(sf::RenderWindow& ventana, int winWidth, int winHeight, int offsetX, int offsetY, int ancho, int alto, float scale) {
    sf::RectangleShape fondo(sf::Vector2f(ancho * scale, alto * scale));
    fondo.setPosition(offsetX, offsetY);
    fondo.setFillColor(sf::Color::Black);
    ventana.draw(fondo);
}
