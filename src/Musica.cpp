#include "Musica.hpp"
#include <iostream>

Musica::Musica(const std::string &rutaArchivo)
{
    if (!musica.openFromFile(rutaArchivo))
    {
        // std::cerr << "Error: No se pudo cargar la música desde " << rutaArchivo << std::endl;
    }
    else
    {
        musica.setLoop(true); // Repetir en bucle
    }
}

void Musica::Reproducir()
{
    musica.play();
}

void Musica::Detener()
{
    musica.stop();
}

void Musica::Pausar()
{
    musica.pause();
}

void Musica::Reanudar()
{
    if (musica.getStatus() == sf::Music::Paused)
        musica.play();
}

bool Musica::VerificarSiEstaReproduciendo() const
{
    return musica.getStatus() == sf::Music::Playing;
}

void Musica::EstablecerVolumen(float volumen)
{
    musica.setVolume(volumen);
}
