#include "../include/Audio.hpp"
#include <stdexcept>
#include <iostream>

Audio::Audio(const std::string& rutaArchivo) {
    std::cout << "Intentando cargar el archivo de audio: " << rutaArchivo << std::endl;
    if (!musica.openFromFile(rutaArchivo)) {
        throw std::runtime_error("No se pudo cargar el archivo de audio: " + rutaArchivo);
    }
    std::cout << "Archivo de audio cargado correctamente." << std::endl;
    musica.setVolume(100); // Establece el volumen al 100%
}

void Audio::reproducir() {
    std::cout << "Reproduciendo audio..." << std::endl;
    musica.play();
    if (musica.getStatus() == sf::Music::Playing) {
        std::cout << "El audio se está reproduciendo correctamente." << std::endl;
    } else {
        std::cout << "El audio no se está reproduciendo." << std::endl;
    }
}

bool Audio::estaReproduciendo() const {
    return musica.getStatus() == sf::Music::Playing;
}

void Audio::setVolume(float volumen) {
    musica.setVolume(volumen);
}
