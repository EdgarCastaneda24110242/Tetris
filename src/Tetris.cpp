#include "Tetris.hpp"
#include "Ventana.hpp"
#include "Controles.hpp"
#include "ColorPieza.hpp"
#include "TableroRender.hpp"
#include "../include/FondoTablero.hpp"
#include "../include/Audio.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <vector>
#include <algorithm>

const int BLOCK_SIZE = 30, SCALE = 2, PANEL_WIDTH = 180, PANEL_MARGIN = 16;
const int BOARD_OFFSET_X = PANEL_WIDTH + PANEL_MARGIN, BOARD_OFFSET_Y = 40;
const int WIN_WIDTH = BOARD_OFFSET_X + Tablero::ANCHO * BLOCK_SIZE * SCALE + PANEL_MARGIN;
const int WIN_HEIGHT = BOARD_OFFSET_Y + Tablero::ALTO * BLOCK_SIZE * SCALE + PANEL_MARGIN;

Tetris::Tetris() : tablero(), piezaActual(nullptr), gameOver(false) {}

void Tetris::nuevaPieza() {
    if (piezaActual) delete piezaActual;
    piezaActual = new Pieza(rand() % 7);
    piezaActual->x = 3; piezaActual->y = 0;
    if (tablero.colision(*piezaActual)) gameOver = true;
}

void Tetris::jugar() {
    // Crear una instancia de la clase Audio y reproducir la música
    Audio audio("assets/music/Tetris.ogg");
    audio.reproducir();

    Ventana ventana(WIN_WIDTH, WIN_HEIGHT, "Tetris NES Style");
    sf::Font font;
    font.loadFromFile("arial.ttf"); // Asegúrate de tener arial.ttf en el directorio de ejecución

    // Pantalla de inicio
    bool inicio = true;
    while (inicio && ventana.estaAbierta()) {
        ventana.limpiar();
        FondoTablero::dibujar(ventana.getWindow(), WIN_WIDTH, WIN_HEIGHT, BOARD_OFFSET_X, BOARD_OFFSET_Y, Tablero::ANCHO * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE, SCALE);

        sf::Text titulo("TETRIS", font, 80);
        titulo.setFillColor(sf::Color(255, 255, 255));
        titulo.setStyle(sf::Text::Bold);
        titulo.setPosition(WIN_WIDTH/2 - titulo.getGlobalBounds().width/2, 80);
        ventana.getWindow().draw(titulo);

        sf::Text press("Presiona ENTER para jugar", font, 32);
        press.setFillColor(sf::Color(200, 200, 200));
        press.setPosition(WIN_WIDTH/2 - press.getGlobalBounds().width/2, WIN_HEIGHT/2 + 60);
        ventana.getWindow().draw(press);

        ventana.mostrar();

        sf::Event e;
        while (ventana.obtenerEvento(e)) {
            if (e.type == sf::Event::Closed) {
                ventana.getWindow().close();
                return;
            }
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                inicio = false;
            }
        }
    }

    sf::Clock clock;
    float timer = 0, delay = 0.5;
    std::vector<int> lineasAnimadas; int animFrames = 0, FRAMES = 12;
    nuevaPieza();

    while (ventana.estaAbierta() && !gameOver) {
        float time = clock.restart().asSeconds(); timer += time;
        sf::Event e;
        while (ventana.obtenerEvento(e)) {
            if (e.type == sf::Event::Closed) ventana.getWindow().close();
            Controles::Accion a = Controles::procesarEvento(e);
            if (a == Controles::MoverIzquierda) { piezaActual->x--; if (tablero.colision(*piezaActual)) piezaActual->x++; }
            if (a == Controles::MoverDerecha) { piezaActual->x++; if (tablero.colision(*piezaActual)) piezaActual->x--; }
            if (a == Controles::Bajar) { piezaActual->y++; if (tablero.colision(*piezaActual)) { piezaActual->y--; tablero.fijarPieza(*piezaActual); tablero.limpiarLineas(ventana.getWindow()); nuevaPieza(); } }
            if (a == Controles::Rotar) { piezaActual->rotar(); if (tablero.colision(*piezaActual)) piezaActual->rotar(); }
            if (a == Controles::Salir) ventana.getWindow().close();
        }

        if (animFrames > 0) {
            ventana.limpiar();
            TableroRender::dibujarFondo(ventana.getWindow(), WIN_WIDTH, WIN_HEIGHT, BOARD_OFFSET_X, BOARD_OFFSET_Y, Tablero::ANCHO*BLOCK_SIZE, Tablero::ALTO*BLOCK_SIZE, SCALE);
            TableroRender::dibujarTablero(ventana.getWindow(), tablero, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE, lineasAnimadas, animFrames, FRAMES);
            TableroRender::dibujarPieza(ventana.getWindow(), *piezaActual, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE);
            ventana.mostrar();
            if (--animFrames == 0) {
                for (int idx : lineasAnimadas) {
                    tablero.matriz.erase(tablero.matriz.begin() + idx);
                    tablero.matriz.insert(tablero.matriz.begin(), std::vector<int>(Tablero::ANCHO, 0));
                }
                lineasAnimadas.clear();
            }
            continue;
        }

        if (timer > delay) {
            piezaActual->y++;
            if (tablero.colision(*piezaActual)) {
                piezaActual->y--;
                tablero.fijarPieza(*piezaActual);
                lineasAnimadas.clear();
                for (int i=0; i<Tablero::ALTO; ++i) {
                    bool llena = true;
                    for (int j=0; j<Tablero::ANCHO; ++j) if (!tablero.matriz[i][j]) { llena = false; break; }
                    if (llena) lineasAnimadas.push_back(i);
                }
                if (!lineasAnimadas.empty()) animFrames = FRAMES;
                else tablero.limpiarLineas(ventana.getWindow());
                nuevaPieza();
            }
            timer = 0;
        }

        if (animFrames == 0) {
            ventana.limpiar();
            TableroRender::dibujarFondo(ventana.getWindow(), WIN_WIDTH, WIN_HEIGHT, BOARD_OFFSET_X, BOARD_OFFSET_Y, Tablero::ANCHO*BLOCK_SIZE, Tablero::ALTO*BLOCK_SIZE, SCALE);
            TableroRender::dibujarTablero(ventana.getWindow(), tablero, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE);
            TableroRender::dibujarPieza(ventana.getWindow(), *piezaActual, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE);
            ventana.mostrar();
        }
    }
    delete piezaActual;
}
