#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

class Reproductor {
private:
    Music music;
    int volumenGeneral = 5; // Volumen general del reproductor, puede ser ajustado
public:
    int reproducirMusica(const string& archivo);
    void setVolumen(int nuevoVolumen);
    void finMusica();
    bool estaSonando() const {
        return music.getStatus() == sf::Music::Playing;
    }
    void esperarFinMusica() {
        while (music.getStatus() == sf::Music::Playing) {
            sf::sleep(sf::milliseconds(100));
        }
    }
};

// Implementaciones fuera de la clase
void Reproductor::finMusica() {
    if (music.getStatus() == Music::Playing) {
        music.stop();
        std::cout << "Música detenida correctamente\n";
    } else {
        std::cout << "No hay música reproduciéndose\n";
    }
}

void Reproductor::setVolumen(int nuevoVolumen) {
    volumenGeneral = nuevoVolumen;
    music.setVolume(volumenGeneral);
}

int Reproductor::reproducirMusica(const string& archivo) {
    if (!music.openFromFile(archivo)) {
        std::cerr << "Error al cargar el archivo de música\n";
        return -1;
    }
    std::cout << "Archivo de música cargado correctamente\n";
    music.setVolume(volumenGeneral);
    music.play();
    return 0;
}

