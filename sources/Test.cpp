#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>

int main() {
    //crear ventana
    sf::RenderWindow window(sf::VideoMode(400, 600), "SFML Window");
    
    // Cargar la fuente de texto TTF
    sf::Font fuente;
    if (!fuente.loadFromFile("./assets/fonts/Minecraft.ttf"))
    {
        // Manejar el error si no se puede cargar la fuente
        return -1;
    }

    // Crear un texto
    sf::Text titleTetris;
    sf::Text creadores;
    //asignamos la fuente previa a nestro texto tetris
    titleTetris.setFont(fuente);
    creadores.setFont(fuente);
    //asignamos el texto a nuestro texto tetris
    titleTetris.setString("Tetris");
    creadores.setString("By: Edgar and Fer");   
//asignamos el tamao de la letra
    titleTetris.setCharacterSize(50);
    creadores.setCharacterSize(20);
    //asignamos el color de la letra
    titleTetris.setFillColor(sf::Color::Blue);
    creadores.setFillColor(sf::Color::White);
    //asignamos la posicion de nuestro texto
    titleTetris.setPosition(125, 10);
    creadores.setPosition(125, 100);

    // Bucle principal de la ventana
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                window.close();
            }
        }

        // Limpiar la ventana
        window.clear(sf::Color::Black);

        // Dibujar el texto
        window.draw(titleTetris);
        window.draw(creadores);

        // Mostrar el contenido de la ventana
        window.display();

    }
    return 0;
}
