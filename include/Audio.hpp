#pragma once
#include <SFML/Audio.hpp>
#include <string>

class Audio {
private:
    sf::Music musica;

public:
    explicit Audio(const std::string& rutaArchivo);
    void reproducir();
    bool estaReproduciendo() const;
    void setVolume(float volumen); // Método para ajustar el volumen
};
