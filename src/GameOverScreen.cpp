#include "GameOverScreen.hpp"
#include <SFML/Graphics.hpp>

GameOverScreen::GameOverScreen(sf::RenderWindow &ventana)
    : ventana(ventana) {}

void GameOverScreen::Mostrar()
{
    ventana.clear(sf::Color::Black);

    const int pixelSize = 5;
    const sf::Color colorTexto = sf::Color::Red;

    const std::vector<std::vector<sf::Vector2i>> letras = {
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 2}},
        {{0, 1}, {1, 0}, {2, 0}, {3, 1}, {0, 2}, {3, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3}},
        {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 1}, {2, 1}, {3, 0}, {3, 1}, {3, 2}, {3, 3}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3}, {1, 2}},
        {},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3}, {3, 2}, {3, 1}},
        {{0, 0}, {0, 1}, {0, 2}, {1, 3}, {2, 3}, {3, 0}, {3, 1}, {3, 2}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 3}, {1, 2}},
        {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 3}, {2, 3}, {3, 1}, {2, 2}, {3, 2}}};

    int startX = (ventana.getSize().x - (letras.size() * 5 * pixelSize + (letras.size() - 1) * pixelSize)) / 2;
    int startY = ventana.getSize().y / 2 - 2 * pixelSize;

    for (size_t i = 0; i < letras.size(); ++i)
    {
        int offsetX = startX + i * (5 * pixelSize + pixelSize);
        for (const auto &pixel : letras[i])
        {
            sf::RectangleShape rect(sf::Vector2f(pixelSize, pixelSize));
            rect.setFillColor(colorTexto);
            rect.setPosition(offsetX + pixel.x * pixelSize, startY + pixel.y * pixelSize);
            ventana.draw(rect);
        }
    }

    ventana.display();
    sf::sleep(sf::seconds(3));
}
