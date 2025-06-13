#include "Tablero.hpp"
#include <iostream>
#include <SFML/System.hpp> // Asegúrate de incluir esto para sf::sleep
#include <SFML/Graphics.hpp> // Incluir SFML para gráficos
#include "Constantes.hpp"  // Incluir para BLOCK_SIZE

Tablero::Tablero() : matriz(ALTO, std::vector<int>(ANCHO, 0)) {}

bool Tablero::colision(const Pieza& pieza) const {
    for (size_t i = 0; i < pieza.forma.size(); ++i)
        for (size_t j = 0; j < pieza.forma[0].size(); ++j)
            if (pieza.forma[i][j]) {
                int nx = pieza.x + j;
                int ny = pieza.y + i;
                if (nx < 0 || nx >= ANCHO || ny < 0 || ny >= ALTO || matriz[ny][nx])
                    return true;
            }
    return false;
}

void Tablero::fijarPieza(const Pieza& pieza) {
    for (size_t i = 0; i < pieza.forma.size(); ++i)
        for (size_t j = 0; j < pieza.forma[0].size(); ++j)
            if (pieza.forma[i][j]) {
                int nx = pieza.x + j;
                int ny = pieza.y + i;
                if (ny >= 0 && ny < ALTO && nx >= 0 && nx < ANCHO)
                    matriz[ny][nx] = pieza.tipo + 1;
            }
}

int Tablero::limpiarLineas(sf::RenderWindow& ventana) {
    int lineasEliminadas = 0;
    for (int i = ALTO - 1; i >= 0; --i) {
        bool llena = true;
        for (int j = 0; j < ANCHO; ++j) {
            if (!matriz[i][j]) llena = false;
        }
        if (llena) {
            // Efecto de destello para la línea eliminada
            for (int frame = 0; frame < 10; ++frame) { // 10 frames de animación
                for (int j = 0; j < ANCHO; ++j) {
                    sf::RectangleShape destello(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
                    destello.setFillColor(sf::Color(255, 255, 255, 255 - frame * 25)); // Desvanecimiento
                    destello.setPosition(j * BLOCK_SIZE, i * BLOCK_SIZE);
                    ventana.draw(destello);
                }
                ventana.display();
                sf::sleep(sf::milliseconds(50)); // Pausa para la animación
            }

            matriz.erase(matriz.begin() + i);
            matriz.insert(matriz.begin(), std::vector<int>(ANCHO, 0));
            ++i;
            ++lineasEliminadas;
        }
    }
    return lineasEliminadas;
}

void Tablero::dibujar() const {
    system("cls");
    for (int i = 0; i < ALTO; ++i) {
        for (int j = 0; j < ANCHO; ++j)
            std::cout << (matriz[i][j] ? "#" : ".");
        std::cout << "\n";
    }
}