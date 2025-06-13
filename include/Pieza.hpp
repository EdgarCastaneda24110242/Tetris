#pragma once
#include <vector>

class Pieza {
public:
    int tipo;
    int rotacion;
    int x, y;
    std::vector<std::vector<int>> forma;

    Pieza(int tipo);
    void rotar();
};
