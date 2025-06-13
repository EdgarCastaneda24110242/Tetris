#include "Musica.hpp"
#include <iostream>

Musica::Musica(const std::string& rutaArchivo) {
    if (!musica.openFromFile(rutaArchivo)) {
        std::cerr << "Error: No se pudo cargar la música desde " << rutaArchivo << std::endl;
    } else {
        musica.setLoop(true); // Repetir en bucle
    }
}

void Musica::reproducir() {
    musica.play();
}

void Musica::detener() {
    musica.stop();
}

bool Musica::estaReproduciendo() const {
    return musica.getStatus() == sf::Music::Playing;
}

void Musica::setVolume(float volumen) {
    musica.setVolume(volumen);
}
