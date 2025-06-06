#include "../include/JuegoTetrix.hpp"

JuegoTetrix::JuegoTetrix()
    : opcionesMenu{"Jugar", "Opciones", "Salir"}, opcionSeleccionada(0) {}

void JuegoTetrix::iniciarJuego() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Tetris Menu");
    while (window.isOpen()) {
        EventoMenu evento = Ventana.leerEventoMenu(window);
        if (evento == Arriba && opcionSeleccionada > 0) opcionSeleccionada--;
        if (evento == Abajo && opcionSeleccionada < opcionesMenu.size() - 1) opcionSeleccionada++;
        if (evento == Enter) {
            // Aquí puedes ejecutar la acción de la opción seleccionada
            break;
        }
        window.clear();
        Ventana.dibujarMenu(window, opcionesMenu, opcionSeleccionada);
        window.display();
    }
}
