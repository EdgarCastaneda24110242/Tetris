#include <SFML/Graphics.hpp>
#include "Reproductor.hpp"
#pragma once

using namespace sf;
using namespace std;

enum EventoMenu
{
    Ninguno,
    Arriba,
    Abajo,
    Enter
};

class Ventana
{
public:
    Reproductor r;
    void dibujarMenu(RenderWindow &window, const vector<string> &opciones, int seleccionada);
    EventoMenu leerEventoMenu(RenderWindow &window);
    // Nueva función para mostrar el menú completo y devolver la opción elegida
    int mostrarMenu(RenderWindow &window, const vector<string> &opciones)
    {
        int opcionSeleccionada = 0;
        while (window.isOpen())
        {
            EventoMenu evento = leerEventoMenu(window);
            if (evento == Arriba && opcionSeleccionada > 0)
                opcionSeleccionada--;
            if (evento == Abajo && opcionSeleccionada < opciones.size() - 1)
                opcionSeleccionada++;
            if (evento == Enter)
            {
                return opcionSeleccionada;
            }
            window.clear();
            dibujarMenu(window, opciones, opcionSeleccionada);
            window.display();
        }
        return -1; // Si se cierra la ventana
    }
};

void Ventana::dibujarMenu(RenderWindow &window, const vector<string> &opciones, int seleccionada)
{
    Font font;
    if (!font.loadFromFile("./assets/fonts/Tetris.ttf"))
    {
        // Si no se carga la fuente, no dibuja nada
        return;
    }
    for (size_t i = 0; i < opciones.size(); ++i)
    {
        Text texto;
        texto.setFont(font);
        texto.setString(opciones[i]);
        texto.setCharacterSize(23);
        texto.setPosition(160, 120 + 60 * i);
        if (i == seleccionada)
        {
            texto.setFillColor(Color::Yellow);
        }
        else
        {
            texto.setFillColor(Color::White);
        }
        window.draw(texto);
    }
}

EventoMenu Ventana::leerEventoMenu(RenderWindow &window)
{
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
            return Ninguno;
        }
        if (event.type == Event::KeyPressed)
        {
            if (event.key.code == Keyboard::Up)
            {
                r.reproducirMusica("./assets/music/Desplazamiento.ogg");
                return Arriba;
            }
            if (event.key.code == Keyboard::Down)
            {
                r.reproducirMusica("./assets/music/Desplazamiento.ogg");
                return Abajo;

            }
            if (event.key.code == Keyboard::Enter)
                return Enter;
        }
    }
    return Ninguno;
}