#include "Juego.hpp"
#include "GameOverScreen.hpp"
#include "Audio.hpp"
#include "ColorPieza.hpp" // Incluir el encabezado para usar ColorPieza
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream> // Incluir para std::cerr
#include <fstream>  // Incluir para std::ifstream

Juego::Juego(sf::RenderWindow& ventanaPrincipal)
    : ventana(ventanaPrincipal), gameOver(false), musica("assets/music/Tetris.ogg") {
    piezaActual = nullptr;
    proximaPieza = new Pieza(rand() % 7); // Inicializar proximaPieza con una nueva pieza
    puntaje = 0;

    // Cargar textura para las piezas
    if (!texturaPiezas.loadFromFile("assets/textures/piezas_pixelart.png")) {
        std::cerr << "Error: No se pudo cargar la textura de las piezas." << std::endl;
    }

    // Reproducir música de fondo
    musica.reproducir();
}

void Juego::nuevaPieza() {
    if (piezaActual) delete piezaActual;
    piezaActual = proximaPieza;

    if (piezaActual) { // Validar que piezaActual no sea nullptr
        piezaActual->x = 3; 
        piezaActual->y = 0;
        if (tablero.colision(*piezaActual)) gameOver = true;
    } else {
        std::cerr << "Error: piezaActual es nullptr después de asignar proximaPieza." << std::endl;
        gameOver = true;
        return;
    }

    proximaPieza = new Pieza(rand() % 7); // Genera una nueva próxima pieza
}

void Juego::dibujarProximaPieza() {
    // Dimensiones del recuadro
    float recuadroX = Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 20;
    float recuadroY = 200;
    float recuadroAncho = 100; // Ancho del recuadro
    float recuadroAlto = 100;  // Alto del recuadro

    // Dimensiones de la próxima pieza
    size_t piezaAncho = proximaPieza->forma[0].size() * BLOCK_SIZE;
    size_t piezaAlto = proximaPieza->forma.size() * BLOCK_SIZE;

    // Centrar la pieza dentro del recuadro con un ajuste adicional
    float piezaOffsetX = recuadroX + (recuadroAncho - piezaAncho) / 2 + 2; // Sin ajuste horizontal
    float piezaOffsetY = recuadroY + (recuadroAlto - piezaAlto) / 2 + 113; // Ajuste adicional hacia abajo

    // Dibujar cada bloque de la próxima pieza
    for (size_t i = 0; i < proximaPieza->forma.size(); ++i) {
        for (size_t j = 0; j < proximaPieza->forma[i].size(); ++j) {
            if (proximaPieza->forma[i][j] != 0) {
                sf::RectangleShape bloque(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4));
                bloque.setFillColor(ColorPieza::obtener(proximaPieza->tipo + 1));
                bloque.setOutlineThickness(2);
                bloque.setOutlineColor(sf::Color::Black);
                bloque.setPosition(piezaOffsetX + j * BLOCK_SIZE, piezaOffsetY + i * BLOCK_SIZE);
                ventana.getWindow().draw(bloque);
            }
        }
    }
}

void Juego::jugar() {
    // Crear un objeto de sonido para MovePieza
    Audio sonidoMover("assets/music/MovePieza.ogg");
    sonidoMover.setVolume(18); // Reducir el volumen tres veces más

    // Crear un objeto de sonido para HitPieza
    Audio sonidoHit("assets/music/HitPieza.ogg");
    sonidoHit.setVolume(75); // Reducir el volumen tres veces más

    // Reducir el volumen de la música de fondo Tetris
    musica.setVolume(33); // Reducir el volumen a un tercio

    sf::Clock clock;
    float timer = 0, delay = 0.5;

    nuevaPieza();

    // Reproducir el sonido de inicio del juego
    Audio sonidoInicio("assets/music/EmpezarJuego.ogg");
    sonidoInicio.reproducir();

    // Inicializar nivel y puntaje
    int nivel = 1;
    int puntaje = 0;

    // Ajustar la velocidad de caída según el nivel
    const float velocidadBase = 0.5f;
    delay = velocidadBase;

    // Reproducir el sonido al eliminar líneas
    Audio sonidoLinea("assets/music/LineaPunto.ogg");
    sonidoLinea.setVolume(50); // Ajustar el volumen

    bool enPausa = false; // Variable para rastrear el estado de pausa

    while (ventana.estaAbierta() && !gameOver) {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event e;
        while (ventana.obtenerEvento(e)) {
            if (e.type == sf::Event::Closed)
                ventana.getWindow().close();

            // Detectar la tecla 'Q' para alternar entre pausa y reanudar
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Q) {
                enPausa = !enPausa; // Alternar el estado de pausa
                continue; // Asegurar que el estado de pausa se procese correctamente
            }

            if (!enPausa) {
                Controles::Accion accion = Controles::procesarEvento(e);
                switch (accion) {
                    case Controles::MoverIzquierda:
                        piezaActual->x--; if (tablero.colision(*piezaActual)) piezaActual->x++;
                        break;
                    case Controles::MoverDerecha:
                        piezaActual->x++; if (tablero.colision(*piezaActual)) piezaActual->x--;
                        break;
                    case Controles::Bajar:
                        piezaActual->y++;
                        if (tablero.colision(*piezaActual)) {
                            piezaActual->y--;
                            tablero.fijarPieza(*piezaActual);

                            // Reproducir el sonido al fijar la pieza
                            sonidoHit.reproducir();

                            // Incrementar puntaje y nivel
                            int lineasEliminadas = tablero.limpiarLineas(ventana.getWindow()); // Pasar la ventana como argumento
                            if (lineasEliminadas > 0) {
                                sonidoLinea.reproducir(); // Reproducir sonido si se eliminan líneas
                            }
                            puntaje += lineasEliminadas * 100;
                            nivel = 1 + puntaje / 1000; // Subir nivel cada 1000 puntos
                            delay = velocidadBase / nivel; // Aumentar velocidad

                            nuevaPieza();
                        }
                        break;
                    case Controles::Rotar:
                        piezaActual->rotar();
                        if (tablero.colision(*piezaActual)) {
                            piezaActual->rotar();
                        } else {
                            // Reproducir el sonido al rotar la pieza
                            sonidoMover.reproducir();
                        }
                        break;
                    case Controles::Salir:
                        // Cambiar la funcionalidad de 'Salir' para que no cierre la ventana
                        enPausa = !enPausa; // Alternar el estado de pausa
                        break;
                    default:
                        break;
                }
            }
        }

        if (!enPausa) {
            // Actualizar la lógica del juego solo si no está en pausa
            if (timer > delay) {
                piezaActual->y++;
                if (tablero.colision(*piezaActual)) {
                    piezaActual->y--;
                    tablero.fijarPieza(*piezaActual);

                    // Reproducir el sonido al fijar la pieza
                    sonidoHit.reproducir();

                    // Incrementar puntaje y nivel
                    int lineasEliminadas = tablero.limpiarLineas(ventana.getWindow()); // Pasar la ventana como argumento
                    if (lineasEliminadas > 0) {
                        sonidoLinea.reproducir(); // Reproducir sonido si se eliminan líneas
                    }
                    puntaje += lineasEliminadas * 100;
                    nivel = 1 + puntaje / 1000; // Subir nivel cada 1000 puntos
                    delay = velocidadBase / nivel; // Aumentar velocidad

                    nuevaPieza();
                }
                timer = 0;
            }

            ventana.limpiar();

            // Dibuja el tablero with un color uniforme para las piezas fijadas
            for (int i = 0; i < Tablero::ALTO; ++i) {
                for (int j = 0; j < Tablero::ANCHO; ++j) {
                    if (tablero.matriz[i][j]) {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE-4, BLOCK_SIZE-4)); // Bloques más pequeños para simular píxeles

                        // Asignar un color uniforme para las piezas fijadas
                        block.setFillColor(sf::Color(128, 128, 128)); // Gris metálico

                        // Agregar un borde grueso negro para un efecto retro
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);

                        block.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(block);

                        // Agregar brillo pixelado en la esquina superior izquierda
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE-4) / 4, (BLOCK_SIZE-4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150)); // Color blanco translúcido para el brillo
                        brillo.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(brillo);
                    }
                }
            }

            // Dibuja la pieza actual con un estilo retro arcade y brillo pixelado
            for (size_t i = 0; i < piezaActual->forma.size(); ++i)
                for (size_t j = 0; j < piezaActual->forma[0].size(); ++j)
                    if (piezaActual->forma[i][j]) {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE-4, BLOCK_SIZE-4)); // Bloques más pequeños para simular píxeles

                        // Asignar colores sólidos brillantes para un estilo retro arcade
                        switch (piezaActual->tipo) {
                            case 0: block.setFillColor(sf::Color::Cyan); break;    // I - Cyan
                            case 1: block.setFillColor(sf::Color::Yellow); break;  // O - Amarillo
                            case 2: block.setFillColor(sf::Color::Magenta); break; // T - Magenta
                            case 3: block.setFillColor(sf::Color::Green); break;   // S - Verde
                            case 4: block.setFillColor(sf::Color::Red); break;     // Z - Rojo
                            case 5: block.setFillColor(sf::Color::Blue); break;    // J - Azul
                            case 6: block.setFillColor(sf::Color(255,140,0)); break; // L - Naranja
                            default: block.setFillColor(sf::Color::White); break;  // Blanco
                        }

                        // Agregar un borde grueso negro para un efecto retro
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);

                        block.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(block);

                        // Agregar brillo pixelado en la esquina superior izquierda
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE-4) / 4, (BLOCK_SIZE-4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150)); // Color blanco translúcido para el brillo
                        brillo.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(brillo);
                    }

            // Agregar un recuadro a la derecha del tablero
            sf::RectangleShape recuadroDerecho(sf::Vector2f(6 * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE));
            recuadroDerecho.setFillColor(sf::Color(0, 0, 0, 150)); // Fondo negro translúcido
            recuadroDerecho.setOutlineThickness(2);
            recuadroDerecho.setOutlineColor(sf::Color::White);
            recuadroDerecho.setPosition(Tablero::ANCHO * BLOCK_SIZE, 0);
            ventana.getWindow().draw(recuadroDerecho);

            // Cargar la fuente Arial desde la ruta correcta
            sf::Font fuente;
            if (!fuente.loadFromFile("assets/fonts/Pixel.ttf")) {
                std::cerr << "Error: No se pudo cargar la fuente Pixel desde 'assets/fonts/Pixel.ttf'. Verifica que el archivo exista y sea válido." << std::endl;
                return; // Salir si no se puede cargar la fuente
            }

            // Configurar y dibujar el texto 'PUNTOS'
            sf::Text textoPuntos;
            textoPuntos.setFont(fuente);
            textoPuntos.setString("PUNTOS");
            textoPuntos.setCharacterSize(32); // Incrementar el tamaño de letra
            textoPuntos.setFillColor(sf::Color::White); // Color blanco
            textoPuntos.setStyle(sf::Text::Bold); // Negrita
            textoPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 3, BLOCK_SIZE + 63); // Ajuste de posición para mayor visibilidad

            // Dibujar el texto 'PUNTOS'
            ventana.getWindow().draw(textoPuntos);

            // Dibujar un recuadro pequeño más abajo del letrero de "PUNTOS"
            sf::RectangleShape recuadro(sf::Vector2f(120, 120)); // Tamaño del recuadro
            recuadro.setFillColor(sf::Color(30, 30, 30)); // Color de fondo
            recuadro.setOutlineThickness(2);
            recuadro.setOutlineColor(sf::Color::White); // Borde blanco
            recuadro.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, 300); // Posición más abajo sin cambiar horizontalmente
            ventana.getWindow().draw(recuadro);

            // Dibujar un recuadro para mostrar los puntos acumulados
            sf::RectangleShape recuadroPuntos(sf::Vector2f(120, 50)); // Tamaño del recuadro
            recuadroPuntos.setFillColor(sf::Color(30, 30, 30)); // Color de fondo
            recuadroPuntos.setOutlineThickness(2);
            recuadroPuntos.setOutlineColor(sf::Color::White); // Borde blanco
            recuadroPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, BLOCK_SIZE + 120); // Posición debajo del texto 'PUNTOS'
            ventana.getWindow().draw(recuadroPuntos);

            // Configurar y dibujar el texto de los puntos acumulados
            sf::Text textoAcumulado;
            textoAcumulado.setFont(fuente);
            textoAcumulado.setString(std::to_string(puntaje)); // Mostrar el puntaje actual
            textoAcumulado.setCharacterSize(24); // Tamaño de letra
            textoAcumulado.setFillColor(sf::Color::White); // Color blanco
            textoAcumulado.setStyle(sf::Text::Bold); // Negrita
            textoAcumulado.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, BLOCK_SIZE + 130); // Posición dentro del recuadro

            // Dibujar el texto de los puntos acumulados
            ventana.getWindow().draw(textoAcumulado);

            // Configurar y dibujar el texto 'SIG PIEZA'
            sf::Text textoSigPieza;
            textoSigPieza.setFont(fuente);
            textoSigPieza.setString("SIG PIEZA");
            textoSigPieza.setCharacterSize(24); // Tamaño de letra
            textoSigPieza.setFillColor(sf::Color::White); // Color blanco
            textoSigPieza.setStyle(sf::Text::Bold); // Negrita
            textoSigPieza.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 5, 250); // Posición arriba del recuadro

            // Dibujar el texto 'SIG PIEZA'
            ventana.getWindow().draw(textoSigPieza);

            // Configurar y dibujar el texto 'NIVEL'
            sf::Text textoNivel;
            textoNivel.setFont(fuente);
            textoNivel.setString("NIVEL");
            textoNivel.setCharacterSize(24); // Tamaño de letra
            textoNivel.setFillColor(sf::Color::White); // Color blanco
            textoNivel.setStyle(sf::Text::Bold); // Negrita
            textoNivel.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, 463); // Posición adecuada

            // Dibujar el texto 'NIVEL'
            ventana.getWindow().draw(textoNivel);

            // Configurar y dibujar el texto del nivel actual
            sf::Text textoNivelActual;
            textoNivelActual.setFont(fuente);
            textoNivelActual.setString(std::to_string(nivel)); // Mostrar el nivel actual
            textoNivelActual.setCharacterSize(24); // Tamaño de letra
            textoNivelActual.setFillColor(sf::Color::White); // Color blanco
            textoNivelActual.setStyle(sf::Text::Bold); // Negrita
            textoNivelActual.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 63.5, 512); // Posición debajo del texto 'NIVEL'

            // Dibujar el texto del nivel actual
            ventana.getWindow().draw(textoNivelActual);

            // Dibujar la próxima pieza
            dibujarProximaPieza();

            ventana.mostrar();
        } else {
            // Cargar la fuente si no está cargada
            sf::Font fuente;
            if (!fuente.loadFromFile("assets/fonts/Pixel.ttf")) {
                std::cerr << "Error: No se pudo cargar la fuente Pixel desde 'assets/fonts/Pixel.ttf'. Verifica que el archivo exista y sea válido." << std::endl;
                return;
            }

            if (enPausa) {
                // Mostrar un mensaje de pausa con opciones
                sf::Text textoPausa;
                textoPausa.setFont(fuente); // Asegúrate de que la fuente esté cargada
                textoPausa.setString("   PAUSE\n\n\nCONTINUAR\n\n Reiniciar");
                textoPausa.setCharacterSize(30);
                textoPausa.setFillColor(sf::Color::White);
                textoPausa.setStyle(sf::Text::Bold);
                textoPausa.setPosition(ventana.getWindow().getSize().x / 2 - 100, ventana.getWindow().getSize().y / 2 - 75);

                // Centrar el texto de pausa dentro de la ventana
                sf::Vector2u tamanoVentana = ventana.getWindow().getSize();
                sf::FloatRect boundsTexto = textoPausa.getLocalBounds();
                textoPausa.setOrigin(boundsTexto.width / 2, boundsTexto.height / 2);
                textoPausa.setPosition(tamanoVentana.x / 2, tamanoVentana.y / 2);

                // Dibujar el texto de pausa
                ventana.getWindow().clear(); // Limpiar la ventana antes de dibujar
                ventana.getWindow().draw(textoPausa);
                ventana.getWindow().display(); // Mostrar el contenido dibujado

                // Pausar el bucle hasta que se reanude
                while (enPausa) {
                    sf::Event eventoPausa;
                    while (ventana.obtenerEvento(eventoPausa)) {
                        if (eventoPausa.type == sf::Event::KeyPressed && eventoPausa.key.code == sf::Keyboard::Q) {
                            enPausa = false; // Reanudar el juego
                        }
                    }
                }
            }
        }
    }
    if (piezaActual) delete piezaActual;

    if (gameOver) {
        // Detener la música de fondo
        musica.detener();

        // Reproducir el sonido de GameOver
        Audio sonidoGameOver("assets/music/GameOver.ogg");
        sonidoGameOver.reproducir();

        // Mostrar la pantalla de Game Over
        GameOverScreen gameOverScreen(ventana.getWindow());
        gameOverScreen.mostrar();

        // Esperar mientras se reproduce la música de GameOver
        while (sonidoGameOver.estaReproduciendo()) {
            sf::sleep(sf::milliseconds(100));
        }

    }
}