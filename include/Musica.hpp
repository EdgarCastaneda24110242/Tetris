#ifndef MUSICA_HPP
#define MUSICA_HPP

#include <SFML/Audio.hpp>
#include <string>

class Musica
{
public:
    explicit Musica(const std::string &rutaArchivo);
    void Reproducir();
    void Detener();
    void Pausar();
    void Reanudar();
    bool VerificarSiEstaReproduciendo() const; // Verificar si el sonido está reproduciéndose
    void EstablecerVolumen(float volumen);     // Método para ajustar el volumen

private:
    sf::Music musica;
};

#endif
