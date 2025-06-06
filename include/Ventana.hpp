#include <SFML/Graphics.hpp>
#include "Reproductor.hpp"
#pragma once

using namespace sf;
using namespace std;

enum EstadoPantalla
{
    MENU,
    JUEGO,
    OPCIONES
};
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
    void dibujarPantallaJuego(RenderWindow &window);
    void dibujarOpciones(RenderWindow &window);
    EventoMenu leerEventoMenu(RenderWindow &window);
};

void Ventana::dibujarMenu(RenderWindow &window, const vector<string> &opciones, int seleccionada)
{
    Font font;
    if (!font.loadFromFile("./assets/fonts/tetrominoes.ttf"))
        return;
    for (size_t i = 0; i < opciones.size(); ++i)
    {
        Text texto;
        texto.setFont(font);
        texto.setString(opciones[i]);
        texto.setCharacterSize(36);
        texto.setPosition(100, 100 + 50 * i);
        if (i == seleccionada)
            texto.setFillColor(Color::Blue);
        else
            texto.setFillColor(Color::White);
        window.draw(texto);
    }
}

void Ventana::dibujarPantallaJuego(RenderWindow &window)
{
    Font font;
    if (!font.loadFromFile("./assets/fonts/tetrominoes.ttf"))
        return;
    Text texto;
    texto.setFont(font);
    texto.setString("¡Comienza el juego!");
    texto.setCharacterSize(36);
    texto.setFillColor(Color::Green);
    texto.setPosition(100, 200);
    window.draw(texto);
}

void Ventana::dibujarOpciones(RenderWindow &window)
{
    Font font;
    if (!font.loadFromFile("./assets/fonts/tetrominoes.ttf"))
        return;
    Text texto;
    texto.setFont(font);
    texto.setString("Opciones del juego");
    texto.setCharacterSize(36);
    texto.setFillColor(Color::Yellow);
    texto.setPosition(100, 200);
    window.draw(texto);
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
                return Arriba;
            if (event.key.code == Keyboard::Down)
                return Abajo;
            if (event.key.code == Keyboard::Enter)
                return Enter;
        }
    }
    return Ninguno;
}