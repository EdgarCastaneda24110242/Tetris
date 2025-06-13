#pragma once
#include "Tablero.hpp"
#include "Pieza.hpp"
#include "Ventana.hpp"
#include "Controles.hpp"
#include "Constantes.hpp"
#include "Musica.hpp"
#include "GameOverScreen.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Incluir Audio.hpp para la música

class Juego {
public:
    Juego(sf::RenderWindow& ventanaPrincipal); // Constructor que acepta una referencia a la ventana principal
    void jugar();
private:
    Ventana ventana;
    Tablero tablero;
    Pieza* piezaActual;
    Pieza* proximaPieza;
    bool gameOver;
    int puntaje;
    sf::Font fuente;
    sf::Text textoPuntaje;
    sf::Texture texturaPiezas; // Textura para las piezas estilo pixel art
    Musica musica; // Manejo de la música de fondo
    void nuevaPieza();
    void actualizarPuntaje();
    void dibujarProximaPieza(); // Declaración de la función para dibujar la próxima pieza
};
