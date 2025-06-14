#ifndef MUSICA_HPP
#define MUSICA_HPP

#include <SFML/Audio.hpp>
#include <string>

class Musica {
public:
    explicit Musica(const std::string& rutaArchivo);
    void reproducir();
    void detener();
    void pausar();
    void reanudar();
    bool estaReproduciendo() const; // Verificar si el sonido está reproduciéndose
    void setVolume(float volumen); // Método para ajustar el volumen

private:
    sf::Music musica;
};

#endif
