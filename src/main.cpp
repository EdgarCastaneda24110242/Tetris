#include <SFML/Graphics.hpp>
#include "Juego.hpp"

int main() {
    sf::RenderWindow ventana(sf::VideoMode(480, 600), "Tetris"); // Ajustar la altura a 600 píxeles y mantener el ancho en 480 píxeles
    Juego juego(ventana);
    juego.jugar();
    return 0;
}
