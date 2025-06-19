#pragma once
#include <SFML/Audio.hpp>
#include <string>

class Audio
{
private:
    sf::Music musica;

public:
    explicit Audio(const std::string &rutaArchivo);
    void Reproducir();
    bool VerificarSiEstaReproduciendo() const;
    void EstablecerVolumen(float volumen); // Método para ajustar el volumen
};
