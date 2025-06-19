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

void Tetris::CrearNuevaPieza()
{
    if (piezaActual)
        delete piezaActual;
    piezaActual = new Pieza(rand() % 7);
    piezaActual->x = 3;
    piezaActual->y = 0;
    if (tablero.VerificarColision(*piezaActual))
        gameOver = true;
}

void Tetris::Jugar()
{
    // Crear una instancia de la clase Audio y reproducir la música
    Audio audio("assets/music/Tetris.ogg");
    audio.Reproducir();

    Ventana ventana(WIN_WIDTH, WIN_HEIGHT, "Tetris NES Style");
    sf::Font font;
    font.loadFromFile("arial.ttf"); // Asegúrate de tener arial.ttf en el directorio de ejecución

    // Pantalla de inicio
    bool inicio = true;
    while (inicio && ventana.VerificarSiEstaAbierta())
    {
        ventana.Limpiar();
        FondoTablero::Dibujar(ventana.ObtenerVentana(), WIN_WIDTH, WIN_HEIGHT, BOARD_OFFSET_X, BOARD_OFFSET_Y, Tablero::ANCHO * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE, SCALE);

        sf::Text titulo("TETRIS", font, 80);
        titulo.setFillColor(sf::Color(255, 255, 255));
        titulo.setStyle(sf::Text::Bold);
        titulo.setPosition(WIN_WIDTH / 2 - titulo.getGlobalBounds().width / 2, 80);
        ventana.ObtenerVentana().draw(titulo);

        sf::Text press("Presiona ENTER para jugar", font, 32);
        press.setFillColor(sf::Color(200, 200, 200));
        press.setPosition(WIN_WIDTH / 2 - press.getGlobalBounds().width / 2, WIN_HEIGHT / 2 + 60);
        ventana.ObtenerVentana().draw(press);

        ventana.Mostrar();

        sf::Event e;
        while (ventana.ProcesarEvento(e))
        {
            if (e.type == sf::Event::Closed)
            {
                ventana.ObtenerVentana().close();
                return;
            }
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter)
            {
                inicio = false;
            }
        }
    }

    sf::Clock clock;
    float timer = 0, delay = 0.5;
    std::vector<int> lineasAnimadas;
    int animFrames = 0, FRAMES = 12;
    CrearNuevaPieza();

    while (ventana.VerificarSiEstaAbierta() && !gameOver)
    {
        float time = clock.restart().asSeconds();
        timer += time;
        sf::Event e;
        while (ventana.ProcesarEvento(e))
        {
            if (e.type == sf::Event::Closed)
                ventana.ObtenerVentana().close();
            Controles::Accion a = Controles::ProcesarEvento(e);
            if (a == Controles::MoverIzquierda)
            {
                piezaActual->x--;
                if (tablero.VerificarColision(*piezaActual))
                    piezaActual->x++;
            }
            if (a == Controles::MoverDerecha)
            {
                piezaActual->x++;
                if (tablero.VerificarColision(*piezaActual))
                    piezaActual->x--;
            }
            if (a == Controles::Bajar)
            {
                piezaActual->y++;
                if (tablero.VerificarColision(*piezaActual))
                {
                    piezaActual->y--;
                    tablero.FijarPieza(*piezaActual);
                    tablero.LimpiarLineas(ventana.ObtenerVentana());
                    CrearNuevaPieza();
                }
            }
            if (a == Controles::Rotar)
            {
                piezaActual->Rotar();
                if (tablero.VerificarColision(*piezaActual))
                    piezaActual->Rotar();
            }
            if (a == Controles::Salir)
                ventana.ObtenerVentana().close();
        }

        if (animFrames > 0)
        {
            ventana.Limpiar();
            TableroRender::DibujarFondo(ventana.ObtenerVentana(), WIN_WIDTH, WIN_HEIGHT, BOARD_OFFSET_X, BOARD_OFFSET_Y, Tablero::ANCHO * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE, SCALE);
            TableroRender::DibujarTablero(ventana.ObtenerVentana(), tablero, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE, lineasAnimadas, animFrames, FRAMES);
            TableroRender::DibujarPieza(ventana.ObtenerVentana(), *piezaActual, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE);
            ventana.Mostrar();
            if (--animFrames == 0)
            {
                for (int idx : lineasAnimadas)
                {
                    tablero.matriz.erase(tablero.matriz.begin() + idx);
                    tablero.matriz.insert(tablero.matriz.begin(), std::vector<int>(Tablero::ANCHO, 0));
                }
                lineasAnimadas.clear();
            }
            continue;
        }

        if (timer > delay)
        {
            piezaActual->y++;
            if (tablero.VerificarColision(*piezaActual))
            {
                piezaActual->y--;
                tablero.FijarPieza(*piezaActual);
                lineasAnimadas.clear();
                for (int i = 0; i < Tablero::ALTO; ++i)
                {
                    bool llena = true;
                    for (int j = 0; j < Tablero::ANCHO; ++j)
                        if (!tablero.matriz[i][j])
                        {
                            llena = false;
                            break;
                        }
                    if (llena)
                        lineasAnimadas.push_back(i);
                }
                if (!lineasAnimadas.empty())
                    animFrames = FRAMES;
                else
                    tablero.LimpiarLineas(ventana.ObtenerVentana());
                CrearNuevaPieza();
            }
            timer = 0;
        }

        if (animFrames == 0)
        {
            ventana.Limpiar();
            TableroRender::DibujarFondo(ventana.ObtenerVentana(), WIN_WIDTH, WIN_HEIGHT, BOARD_OFFSET_X, BOARD_OFFSET_Y, Tablero::ANCHO * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE, SCALE);
            TableroRender::DibujarTablero(ventana.ObtenerVentana(), tablero, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE);
            TableroRender::DibujarPieza(ventana.ObtenerVentana(), *piezaActual, BOARD_OFFSET_X, BOARD_OFFSET_Y, BLOCK_SIZE, SCALE);
            ventana.Mostrar();
        }
    }
    delete piezaActual;
}
