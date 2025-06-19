#include "TableroRender.hpp"
#include "Bloque.hpp"
#include "FondoTablero.hpp"

void TableroRender::DibujarFondo(sf::RenderWindow &w, int winWidth, int winHeight, int boardOffsetX, int boardOffsetY, int boardW, int boardH, int scale)
{
    FondoTablero::Dibujar(w, winWidth, winHeight, boardOffsetX, boardOffsetY, boardW, boardH, scale);
}

void TableroRender::DibujarTablero(sf::RenderWindow &w, const Tablero &tablero, int boardOffsetX, int boardOffsetY, int blockSize, int scale, const std::vector<int> &lineasAnimadas, int animFrames, int maxFrames)
{
    for (int i = 0; i < Tablero::ALTO; ++i)
        for (int j = 0; j < Tablero::ANCHO; ++j)
            if (tablero.matriz[i][j])
            {
                bool anim = std::find(lineasAnimadas.begin(), lineasAnimadas.end(), i) != lineasAnimadas.end();
                sf::Color baseColor = ColorPieza::Obtener(tablero.matriz[i][j]);
                sf::Color c;
                if (anim)
                {
                    if (animFrames > maxFrames / 2)
                    {
                        c = sf::Color::White;
                    }
                    else
                    {
                        int alpha = 255 * animFrames / (maxFrames / 2);
                        c = sf::Color(baseColor.r, baseColor.g, baseColor.b, alpha);
                    }
                }
                else
                {
                    c = baseColor;
                }
                Bloque::Dibujar(w, boardOffsetX + j * blockSize * scale, boardOffsetY + i * blockSize * scale, blockSize, scale, c, c.a);
            }
}

void TableroRender::DibujarPieza(sf::RenderWindow &w, const Pieza &pieza, int boardOffsetX, int boardOffsetY, int blockSize, int scale)
{
    for (size_t i = 0; i < pieza.forma.size(); ++i)
        for (size_t j = 0; j < pieza.forma[0].size(); ++j)
            if (pieza.forma[i][j])
            {
                sf::Color c = ColorPieza::Obtener(pieza.tipo + 1);
                Bloque::Dibujar(w, boardOffsetX + (pieza.x + j) * blockSize * scale, boardOffsetY + (pieza.y + i) * blockSize * scale, blockSize, scale, c);
            }
}
