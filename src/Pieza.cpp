#include "Pieza.hpp"
#include <cstddef>
#include <vector>

// Definición de las 7 formas de Tetris
static const std::vector<std::vector<std::vector<int>>> FORMAS = {
    // I
    {
        {1,1,1,1}
    },
    // O
    {
        {1,1},
        {1,1}
    },
    // T
    {
        {0,1,0},
        {1,1,1}
    },
    // S
    {
        {0,1,1},
        {1,1,0}
    },
    // Z
    {
        {1,1,0},
        {0,1,1}
    },
    // J
    {
        {1,0,0},
        {1,1,1}
    },
    // L
    {
        {0,0,1},
        {1,1,1}
    }
};

Pieza::Pieza(int t) : tipo(t % 7), rotacion(0), x(3), y(0) {
    forma = FORMAS[tipo];
}

void Pieza::rotar() {
    std::vector<std::vector<int>> nueva(forma[0].size(), std::vector<int>(forma.size()));
    for (std::size_t i = 0; i < forma.size(); ++i)
        for (std::size_t j = 0; j < forma[0].size(); ++j)
            nueva[j][forma.size()-1-i] = forma[i][j];
    forma = nueva;
    rotacion = (rotacion + 1) % 4;
}