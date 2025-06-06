#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Ventana.hpp"
#include "Reproductor.hpp"

using namespace std;

class JuegoTetrix {
private:
    Reproductor reproductor;
    Ventana ventana;
    vector<string> opcionesMenu = {"Jugar", "Opciones", "Salir"};
    int opcionSeleccionada = 0;
    EstadoPantalla estado = MENU;
public:
    JuegoTetrix() = default;
    void iniciarJuego() {
        sf::RenderWindow window(sf::VideoMode(500, 500), "Tetris");
        reproductor.reproducirMusica("./assets/music/TetrisMenu.ogg");
        while (window.isOpen()) {
            if (estado == MENU) {
                EventoMenu evento = ventana.leerEventoMenu(window);
                if (evento == Arriba && opcionSeleccionada > 0) {
                    opcionSeleccionada--;
                    ventana.r.reproducirMusica("./assets/music/Desplazamiento.ogg");
                }
                if (evento == Abajo && opcionSeleccionada < opcionesMenu.size() - 1) {
                    opcionSeleccionada++;
                    ventana.r.reproducirMusica("./assets/music/Desplazamiento.ogg");
                }
                if (evento == Enter) {
                    if (opcionSeleccionada == 0) {
                        estado = JUEGO;
                        reproductor.reproducirMusica("./assets/music/EmpezarJuego.ogg");
                    } else if (opcionSeleccionada == 1) {
                        estado = OPCIONES;
                    } else if (opcionSeleccionada == 2) {
                        window.close();
                    }
                }
                window.clear();
                ventana.dibujarMenu(window, opcionesMenu, opcionSeleccionada);
                window.display();
            } else if (estado == JUEGO) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    // Aquí puedes agregar controles del juego
                }
                window.clear();
                ventana.dibujarPantallaJuego(window);
                window.display();
            } else if (estado == OPCIONES) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                        estado = MENU;
                }
                window.clear();
                ventana.dibujarOpciones(window);
                window.display();
            }
        }
    }
};