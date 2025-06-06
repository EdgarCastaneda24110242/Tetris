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
    Reproductor r;
    Ventana Ventana;
    vector<string> opcionesMenu = {"Jugar", "Opciones", "Salir"};
    int opcionSeleccionada = 0;
public:
    JuegoTetrix() = default;
    
    void iniciarJuego() {
         r.reproducirMusica("./assets/music/TetrisMenu.ogg");
        sf::RenderWindow window(sf::VideoMode(500, 700), "Tetris");
        int seleccion = Ventana.mostrarMenu(window, opcionesMenu);
        // Aquí puedes actuar según la opción elegida
        if (seleccion == 0) {
            // Lógica para "Jugar"
            cout << "Iniciar juego..." << endl;
            r.reproducirMusica("./assets/music/EmpezarJuego.ogg");
            // ...
        } else if (seleccion == 1) {
            // Lógica para "Opciones"
            cout << "Mostrar opciones..." << endl;
            // ...
        } else if (seleccion == 2) {
            // Lógica para "Salir"
            cout << "Salir del juego..." << endl;
            // ...
        }
        // Aquí puede seguir el resto de la lógica del juego
    }
};