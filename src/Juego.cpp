#include "Juego.hpp"
#include "GameOverScreen.hpp"
#include "Audio.hpp"
#include "ColorPieza.hpp" // Incluir el encabezado para usar ColorPieza
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream> // Incluir para std::cerr
#include <fstream>  // Incluir para std::ifstream
#include <vector>
#include <ctime>

// Estructura para bloques de fondo animados en el menú principal
struct BloqueFondo {
    int tipo; // 0-6
    float x, y;
    float velocidad;
    float rotacion;
    float rotacionVelocidad;
    float opacidad;
};

Juego::Juego(sf::RenderWindow& ventanaPrincipal)
    : ventana(ventanaPrincipal), gameOver(false), musica("assets/music/Tetris.ogg") {
    piezaActual = nullptr;
    proximaPieza = new Pieza(rand() % 7); // Inicializar proximaPieza con una nueva pieza
    puntaje = 0;

    // Cargar textura para las piezas
    if (!texturaPiezas.loadFromFile("assets/textures/piezas_pixelart.png")) {
        // Eliminado mensaje de error para evitar spam en la terminal
    }

    // Cargar la fuente Pixel.ttf una sola vez para toda la UI
    if (!fuente.loadFromFile("assets/fonts/Pixel.ttf")) {
        std::cerr << "Error: No se pudo cargar la fuente Pixel.ttf" << std::endl;
    }

    // No reproducir música aquí, solo cuando empiece el juego
}

void Juego::nuevaPieza() {
    // Asignar la próxima pieza como actual (sin liberar memoria aquí)
    piezaActual = proximaPieza;
    // Crear una nueva próxima pieza
    proximaPieza = new Pieza(rand() % 7);
    // Inicializar posición de la pieza actual
    if (piezaActual) {
        piezaActual->x = 3;
        piezaActual->y = 0;
        if (tablero.colision(*piezaActual)) gameOver = true;
    } else {
        gameOver = true;
        return;
    }
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

// Método para dibujar el menú de pausa de forma clara y reutilizable
void Juego::dibujarMenuPausa(sf::RenderWindow& window, const sf::Font& fuente) {
    sf::Vector2u tamanoVentana = window.getSize();
    int centroX = tamanoVentana.x / 2 - 80; // Ajuste a la derecha
    // Título "PAUSA" con fuente especial
    sf::Font fuenteTetrominoes;
    if (fuenteTetrominoes.loadFromFile("assets/fonts/tetrominoes.ttf")) {
        sf::Text titulo;
        titulo.setFont(fuenteTetrominoes);
        titulo.setString("PAUSA");
        titulo.setCharacterSize(52);
        titulo.setFillColor(sf::Color::Red);
        titulo.setStyle(sf::Text::Bold);
        sf::FloatRect boundsTitulo = titulo.getLocalBounds();
        titulo.setOrigin(boundsTitulo.width / 2, boundsTitulo.height / 2);
        titulo.setPosition(centroX, tamanoVentana.y / 2 - 80);
        window.draw(titulo);
    } else {
        std::cerr << "No se pudo cargar tetrominoes.ttf, usando fuente por defecto para PAUSA" << std::endl;
        sf::Text titulo;
        titulo.setFont(fuente);
        titulo.setString("PAUSA");
        titulo.setCharacterSize(52);
        titulo.setFillColor(sf::Color::Red);
        titulo.setStyle(sf::Text::Bold);
        sf::FloatRect boundsTitulo = titulo.getLocalBounds();
        titulo.setOrigin(boundsTitulo.width / 2, boundsTitulo.height / 2);
        titulo.setPosition(centroX, tamanoVentana.y / 2 - 80);
        window.draw(titulo);
    }
    // Opción Reanudar
    sf::Text reanudar;
    reanudar.setFont(fuente);
    reanudar.setString("[Q] Reanudar");
    reanudar.setCharacterSize(36);
    reanudar.setFillColor(sf::Color::White);
    reanudar.setStyle(sf::Text::Bold);
    sf::FloatRect boundsReanudar = reanudar.getLocalBounds();
    reanudar.setOrigin(boundsReanudar.width / 2, boundsReanudar.height / 2);
    reanudar.setPosition(centroX, tamanoVentana.y / 2 + 10);
    window.draw(reanudar);
    // Opción Reiniciar
    sf::Text reiniciar;
    reiniciar.setFont(fuente);
    reiniciar.setString("[R] Reiniciar");
    reiniciar.setCharacterSize(36);
    reiniciar.setFillColor(sf::Color::White);
    reiniciar.setStyle(sf::Text::Bold);
    sf::FloatRect boundsReiniciar = reiniciar.getLocalBounds();
    reiniciar.setOrigin(boundsReiniciar.width / 2, boundsReiniciar.height / 2);
    reiniciar.setPosition(centroX, tamanoVentana.y / 2 + 60);
    window.draw(reiniciar);
    // Opción Salir
    sf::Text salir;
    salir.setFont(fuente);
    salir.setString("[M] SALIR");
    salir.setCharacterSize(36);
    salir.setFillColor(sf::Color::White);
    salir.setStyle(sf::Text::Bold);
    sf::FloatRect boundsSalir = salir.getLocalBounds();
    salir.setOrigin(boundsSalir.width / 2, boundsSalir.height / 2);
    salir.setPosition(centroX, tamanoVentana.y / 2 + 110);
    window.draw(salir);
}

void Juego::dibujarUILateral(int puntaje, int nivel, sf::Font& fuente) {
    // Recuadro derecho
    sf::RectangleShape recuadroDerecho(sf::Vector2f(6 * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE));
    recuadroDerecho.setFillColor(sf::Color(0, 0, 0, 210));
    recuadroDerecho.setOutlineThickness(2);
    recuadroDerecho.setOutlineColor(sf::Color::White);
    recuadroDerecho.setPosition(Tablero::ANCHO * BLOCK_SIZE, 0);
    ventana.getWindow().draw(recuadroDerecho);
    // Texto PUNTOS
    sf::Text textoPuntos;
    textoPuntos.setFont(fuente);
    textoPuntos.setString("PUNTOS");
    textoPuntos.setCharacterSize(32);
    textoPuntos.setFillColor(sf::Color::White);
    textoPuntos.setStyle(sf::Text::Bold);
    textoPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 3, BLOCK_SIZE + 63);
    ventana.getWindow().draw(textoPuntos);
    // Recuadro pequeño debajo de PUNTOS
    sf::RectangleShape recuadro(sf::Vector2f(120, 120));
    recuadro.setFillColor(sf::Color(30, 30, 30));
    recuadro.setOutlineThickness(2);
    recuadro.setOutlineColor(sf::Color::White);
    recuadro.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, 300);
    ventana.getWindow().draw(recuadro);
    // Recuadro de puntos acumulados
    sf::RectangleShape recuadroPuntos(sf::Vector2f(120, 50));
    recuadroPuntos.setFillColor(sf::Color(30, 30, 30));
    recuadroPuntos.setOutlineThickness(2);
    recuadroPuntos.setOutlineColor(sf::Color::White);
    recuadroPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, BLOCK_SIZE + 120);
    ventana.getWindow().draw(recuadroPuntos);
    // Texto de puntos acumulados
    sf::Text textoAcumulado;
    textoAcumulado.setFont(fuente);
    textoAcumulado.setString(std::to_string(puntaje));
    textoAcumulado.setCharacterSize(24);
    textoAcumulado.setFillColor(sf::Color::White);
    textoAcumulado.setStyle(sf::Text::Bold);
    textoAcumulado.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, BLOCK_SIZE + 130);
    ventana.getWindow().draw(textoAcumulado);
    // Texto SIG PIEZA
    sf::Text textoSigPieza;
    textoSigPieza.setFont(fuente);
    textoSigPieza.setString("SIG PIEZA");
    textoSigPieza.setCharacterSize(24);
    textoSigPieza.setFillColor(sf::Color::White);
    textoSigPieza.setStyle(sf::Text::Bold);
    textoSigPieza.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 5, 250);
    ventana.getWindow().draw(textoSigPieza);
    // Texto NIVEL
    sf::Text textoNivel;
    textoNivel.setFont(fuente);
    textoNivel.setString("NIVEL");
    textoNivel.setCharacterSize(24);
    textoNivel.setFillColor(sf::Color::White);
    textoNivel.setStyle(sf::Text::Bold);
    textoNivel.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, 463);
    ventana.getWindow().draw(textoNivel);
    // Texto nivel actual
    sf::Text textoNivelActual;
    textoNivelActual.setFont(fuente);
    textoNivelActual.setString(std::to_string(nivel));
    textoNivelActual.setCharacterSize(24);
    textoNivelActual.setFillColor(sf::Color::White);
    textoNivelActual.setStyle(sf::Text::Bold);
    textoNivelActual.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 63.5, 512);
    ventana.getWindow().draw(textoNivelActual);
    // Próxima pieza
    dibujarProximaPieza();
}

void Juego::mostrarMenuPrincipal() {
    bool enMenu = true;
    int opcionSeleccionada = 0;
    std::vector<std::string> opciones = {"Jugar", "Scores", "Salir"};
    sf::Font fuenteTetrominoes;
    fuenteTetrominoes.loadFromFile("assets/fonts/tetrominoes.ttf");
    Musica musicaMenu("assets/music/TetrisMenu.ogg");
    musicaMenu.setVolume(10); // Volumen bajo para el menú principal
    musicaMenu.reproducir();

    // Inicializar generador aleatorio
    srand(static_cast<unsigned int>(time(nullptr)));
    // Bloques de fondo animados
    std::vector<BloqueFondo> bloquesFondo;
    const int piezaMaxAncho = 4 * BLOCK_SIZE; // Máximo ancho de una pieza (pieza I)
    int numColumnas = ventana.getWindow().getSize().x / piezaMaxAncho;
    std::vector<bool> columnasOcupadas(numColumnas, false);
    const int numBloquesFondo = std::min(numColumnas, 18); // No más bloques que columnas
    for (int i = 0; i < numBloquesFondo; ++i) {
        BloqueFondo b;
        b.tipo = rand() % 7;
        Pieza piezaTmp(b.tipo);
        int piezaAncho = piezaTmp.forma[0].size() * BLOCK_SIZE;
        // Buscar columna libre
        int col;
        do {
            col = rand() % numColumnas;
        } while (columnasOcupadas[col]);
        columnasOcupadas[col] = true;
        b.x = col * piezaMaxAncho + rand() % (piezaMaxAncho - piezaAncho + 1);
        b.y = static_cast<float>(rand() % ventana.getWindow().getSize().y);
        b.velocidad = 60 + rand() % 80; // píxeles por segundo
        b.rotacion = 0; // Sin rotación
        b.rotacionVelocidad = 0;
        b.opacidad = 80 + rand() % 60; // 80-140
        b.rotacion = 0;
        bloquesFondo.push_back(b);
    }
    sf::Clock clockFondo;

    while (enMenu && ventana.estaAbierta()) {
        float dt = clockFondo.restart().asSeconds();
        // Actualizar bloques de fondo
        for (int i = 0; i < bloquesFondo.size(); ++i) {
            auto& b = bloquesFondo[i];
            b.y += b.velocidad * dt;
            if (b.y > ventana.getWindow().getSize().y + 40) {
                // Liberar columna anterior
                int colAnterior = (int)((b.x) / piezaMaxAncho);
                columnasOcupadas[colAnterior] = false;
                // Resetear bloque en nueva columna libre
                b.tipo = rand() % 7;
                Pieza piezaTmp(b.tipo);
                int piezaAncho = piezaTmp.forma[0].size() * BLOCK_SIZE;
                int col;
                do {
                    col = rand() % numColumnas;
                } while (columnasOcupadas[col]);
                columnasOcupadas[col] = true;
                b.x = col * piezaMaxAncho + rand() % (piezaMaxAncho - piezaAncho + 1);
                b.y = -40.0f;
                b.rotacion = 0;
                b.rotacionVelocidad = 0;
                b.opacidad = 80 + rand() % 60;
            }
        }
        ventana.getWindow().clear();
        // Dibujar bloques de fondo
        for (const auto& b : bloquesFondo) {
            Pieza pieza(b.tipo);
            for (size_t i = 0; i < pieza.forma.size(); ++i) {
                for (size_t j = 0; j < pieza.forma[i].size(); ++j) {
                    if (pieza.forma[i][j]) {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE-4, BLOCK_SIZE-4));
                        block.setOrigin((BLOCK_SIZE-4)/2, (BLOCK_SIZE-4)/2);
                        block.setPosition(b.x + j*BLOCK_SIZE, b.y + i*BLOCK_SIZE);
                        sf::Color color = ColorPieza::obtener(b.tipo+1);
                        color.a = static_cast<sf::Uint8>(b.opacidad);
                        block.setFillColor(color);
                        block.setOutlineThickness(1);
                        block.setOutlineColor(sf::Color(0,0,0,80));
                        ventana.getWindow().draw(block);
                    }
                }
            }
        }
        // Título con fuente tetrominoes
        sf::Text titulo;
        if (fuenteTetrominoes.getInfo().family != "") {
            titulo.setFont(fuenteTetrominoes);
        } else {
            titulo.setFont(fuente);
        }
        titulo.setString("TETRIS");
        titulo.setCharacterSize(60);
        titulo.setFillColor(sf::Color::Red);
        titulo.setStyle(sf::Text::Bold);
        sf::FloatRect boundsTitulo = titulo.getLocalBounds();
        titulo.setOrigin(boundsTitulo.width / 2, boundsTitulo.height / 2);
        titulo.setPosition(ventana.getWindow().getSize().x / 2, 120);
        ventana.getWindow().draw(titulo);
        // Subtítulo "by Fer and Edgar" con fuente Pixel.ttf
        sf::Text subtitulo;
        subtitulo.setFont(fuente);
        subtitulo.setString("by Fer and Edgar");
        subtitulo.setCharacterSize(24);
        subtitulo.setFillColor(sf::Color::White);
        subtitulo.setStyle(sf::Text::Bold);
        sf::FloatRect boundsSub = subtitulo.getLocalBounds();
        subtitulo.setOrigin(boundsSub.width / 2, boundsSub.height / 2);
        subtitulo.setPosition(ventana.getWindow().getSize().x / 2, 170);
        ventana.getWindow().draw(subtitulo);
        // Opciones
        for (size_t i = 0; i < opciones.size(); ++i) {
            sf::Text opcion;
            opcion.setFont(fuente);
            opcion.setString(opciones[i]);
            opcion.setCharacterSize(36);
            opcion.setFillColor(i == opcionSeleccionada ? sf::Color::Yellow : sf::Color::White);
            opcion.setStyle(sf::Text::Bold);
            sf::FloatRect boundsOpcion = opcion.getLocalBounds();
            opcion.setOrigin(boundsOpcion.width / 2, boundsOpcion.height / 2);
            opcion.setPosition(ventana.getWindow().getSize().x / 2, 250 + i * 60);
            ventana.getWindow().draw(opcion);
        }
        ventana.getWindow().display();
        // Manejo de eventos
        sf::Event event;
        while (ventana.obtenerEvento(event)) {
            if (event.type == sf::Event::Closed) {
                ventana.getWindow().close();
                return;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                    opcionSeleccionada = (opcionSeleccionada + opciones.size() - 1) % opciones.size();
                } else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                    opcionSeleccionada = (opcionSeleccionada + 1) % opciones.size();
                } else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return) {
                    if (opcionSeleccionada == 0) {
                        // Reproducir sonido de empezar juego SOLO aquí
                        Audio sonidoInicio("assets/music/EmpezarJuego.ogg");
                        sonidoInicio.setVolume(20);
                        sonidoInicio.reproducir();
                        sf::sleep(sf::seconds(1.0f));
                        enMenu = false; // Jugar
                    } else if (opcionSeleccionada == 1) {
                        sf::Text scores;
                        scores.setFont(fuente);
                        scores.setString("No implementado");
                        scores.setCharacterSize(32);
                        scores.setFillColor(sf::Color::White);
                        scores.setPosition(ventana.getWindow().getSize().x / 2 - 100, 450);
                        ventana.getWindow().draw(scores);
                        ventana.getWindow().display();
                        sf::sleep(sf::seconds(1.5f));
                    } else if (opcionSeleccionada == 2) {
                        ventana.getWindow().close();
                        return;
                    }
                }
            }
        }
    }
    musicaMenu.detener();
    musica.setVolume(20); // Restaurar volumen normal al salir del menú
}

void Juego::jugar() {
    mostrarMenuPrincipal();
    musica.detener(); // Detener cualquier música previa (incluida la del menú)
    musica.reproducir(); // Iniciar la música de fondo del juego
    // Crear un objeto de sonido para MovePieza
    Audio sonidoMover("assets/music/MovePieza.ogg");
    sonidoMover.setVolume(18); // Reducir el volumen tres veces más

    // Crear un objeto de sonido para HitPieza
    Audio sonidoHit("assets/music/HitPieza.ogg");
    sonidoHit.setVolume(75); // Reducir el volumen tres veces más

    // Reducir el volumen de la música de fondo Tetris
    musica.setVolume(20); // Volumen más bajo

    sf::Clock clock;
    float timer = 0, delay = 0.5;

    nuevaPieza();

    // Eliminar la reproducción del sonido de inicio aquí para evitar duplicados
    // Audio sonidoInicio("assets/music/EmpezarJuego.ogg");
    // sonidoInicio.reproducir();

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
                if (enPausa) {
                    musica.pausar(); // Pausar música
                } else {
                    musica.reanudar(); // Reanudar música
                }
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
                            sonidoHit.reproducir();
                            // Eliminar todas las líneas completas correctamente
                            int lineasEliminadas = 0;
                            for (int i = Tablero::ALTO - 1; i >= 0; --i) {
                                bool llena = true;
                                for (int j = 0; j < Tablero::ANCHO; ++j) {
                                    if (!tablero.matriz[i][j]) llena = false;
                                }
                                if (llena) {
                                    tablero.matriz.erase(tablero.matriz.begin() + i);
                                    tablero.matriz.insert(tablero.matriz.begin(), std::vector<int>(Tablero::ANCHO, 0));
                                    ++lineasEliminadas;
                                    ++i; // Revisar de nuevo la misma fila
                                }
                            }
                            if (lineasEliminadas > 0) {
                                sonidoLinea.reproducir();
                            }
                            puntaje += lineasEliminadas * 100;
                            nivel = 1 + puntaje / 1000;
                            delay = velocidadBase / nivel;
                            nuevaPieza();
                            if (gameOver) break; // Salir del bucle si se pierde
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
            } else {
                musica.pausar(); // Pausar música al pausar el juego
            }
        }

        if (enPausa) {
            musica.pausar();
            // Dibuja el estado actual del juego y overlay de pausa
            ventana.limpiar();
            // Dibuja el tablero
            for (int i = 0; i < Tablero::ALTO; ++i) {
                for (int j = 0; j < Tablero::ANCHO; ++j) {
                    if (tablero.matriz[i][j]) {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE-4, BLOCK_SIZE-4));
                        block.setFillColor(sf::Color(128, 128, 128));
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);
                        block.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(block);
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE-4) / 4, (BLOCK_SIZE-4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                        brillo.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(brillo);
                    }
                }
            }
            // Dibuja la pieza actual
            for (size_t i = 0; i < piezaActual->forma.size(); ++i)
                for (size_t j = 0; j < piezaActual->forma[0].size(); ++j)
                    if (piezaActual->forma[i][j]) {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE-4, BLOCK_SIZE-4));
                        switch (piezaActual->tipo) {
                            case 0: block.setFillColor(sf::Color::Cyan); break;
                            case 1: block.setFillColor(sf::Color::Yellow); break;
                            case 2: block.setFillColor(sf::Color::Magenta); break;
                            case 3: block.setFillColor(sf::Color::Green); break;
                            case 4: block.setFillColor(sf::Color::Red); break;
                            case 5: block.setFillColor(sf::Color::Blue); break;
                            case 6: block.setFillColor(sf::Color(255,140,0)); break;
                            default: block.setFillColor(sf::Color::White); break;
                        }
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);
                        block.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(block);
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE-4) / 4, (BLOCK_SIZE-4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                        brillo.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.getWindow().draw(brillo);
                    }
            // Dibuja la UI lateral completa (recuadro, textos, puntuación, próxima pieza)
            dibujarUILateral(puntaje, nivel, fuente);

            // 2. Dibuja overlay negro translúcido SOLO sobre el área del tablero (no sobre la UI lateral)
            sf::RectangleShape overlay(sf::Vector2f(Tablero::ANCHO * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            overlay.setPosition(0, 0);
            ventana.getWindow().draw(overlay);

            // 3. Dibuja el menú de pausa centrado
            dibujarMenuPausa(ventana.getWindow(), fuente);
            ventana.getWindow().display();
            // Pausar el bucle hasta que se reanude, reinicie o regrese al menú
            bool salirPausa = false;
            bool regresarMenu = false;
            while (enPausa && !salirPausa && !regresarMenu) {
                sf::Event eventoPausa;
                while (ventana.obtenerEvento(eventoPausa)) {
                    if (eventoPausa.type == sf::Event::KeyPressed) {
                        if (eventoPausa.key.code == sf::Keyboard::Q) {
                            enPausa = false; // Reanudar
                        }
                        if (eventoPausa.key.code == sf::Keyboard::R) {
                            // Transición: pantalla negra y sonido de inicio
                            ventana.getWindow().clear(sf::Color::Black);
                            ventana.getWindow().display();
                            Audio sonidoInicio("assets/music/EmpezarJuego.ogg");
                            sonidoInicio.setVolume(20);
                            sonidoInicio.reproducir();
                            sf::sleep(sf::seconds(0.7f)); // Aumenta el tiempo de pantalla negra
                            // Reiniciar la música de fondo
                            musica.detener();
                            musica.reproducir();
                            // Reiniciar el juego y quitar pausa automáticamente
                            puntaje = 0;
                            nivel = 1;
                            delay = velocidadBase;
                            tablero = Tablero();
                            nuevaPieza();
                            enPausa = false;
                            salirPausa = true;
                        }
                        if (eventoPausa.key.code == sf::Keyboard::M) {
                            ventana.getWindow().close(); // Ahora sí cerrar el juego
                            salirPausa = true;
                        }
                    }
                }
            }
            if (regresarMenu) return; // Salir de jugar() y volver al menú principal
            if (salirPausa) continue; // Salta el resto del ciclo para reiniciar
        } else {
            musica.reanudar();
        }

        if (!enPausa) {
            // Actualizar la lógica del juego solo si no está en pausa
            if (timer > delay) {
                piezaActual->y++;
                if (tablero.colision(*piezaActual)) {
                    piezaActual->y--;
                    tablero.fijarPieza(*piezaActual);
                    sonidoHit.reproducir();
                    // Detectar filas completas
                    std::vector<int> filasCompletas;
                    for (int i = Tablero::ALTO - 1; i >= 0; --i) {
                        bool llena = true;
                        for (int j = 0; j < Tablero::ANCHO; ++j) {
                            if (!tablero.matriz[i][j]) llena = false;
                        }
                        if (llena) {
                            filasCompletas.push_back(i);
                        }
                    }
                    // Animación de parpadeo blanco para filas completas
                    if (!filasCompletas.empty()) {
                        const int parpadeos = 6; // Número de parpadeos
                        const float duracionParpadeo = 0.06f; // Duración de cada parpadeo (segundos)
                        for (int p = 0; p < parpadeos; ++p) {
                            ventana.limpiar();
                            // Dibuja el tablero con filas parpadeando
                            for (int i = 0; i < Tablero::ALTO; ++i) {
                                for (int j = 0; j < Tablero::ANCHO; ++j) {
                                    if (tablero.matriz[i][j]) {
                                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE-4, BLOCK_SIZE-4));
                                        bool esFilaParpadeo = std::find(filasCompletas.begin(), filasCompletas.end(), i) != filasCompletas.end();
                                        if (esFilaParpadeo) {
                                            if (p % 2 == 0)
                                                block.setFillColor(sf::Color::White); // Parpadeo blanco
                                            else
                                                block.setFillColor(sf::Color(200, 200, 200)); // Gris claro
                                        } else {
                                            block.setFillColor(sf::Color(128, 128, 128));
                                        }
                                        block.setOutlineThickness(2);
                                        block.setOutlineColor(sf::Color::Black);
                                        block.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                                        ventana.getWindow().draw(block);
                                        // Brillo pixelado
                                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE-4) / 4, (BLOCK_SIZE-4) / 4));
                                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                                        brillo.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                                        ventana.getWindow().draw(brillo);
                                    }
                                }
                            }
                            // Dibuja la pieza actual y UI
                            for (size_t i = 0; i < piezaActual->forma.size(); ++i)
                                for (size_t j = 0; j < piezaActual->forma[0].size(); ++j)
                                    if (piezaActual->forma[i][j]) {
                                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE-4, BLOCK_SIZE-4));
                                        switch (piezaActual->tipo) {
                                            case 0: block.setFillColor(sf::Color::Cyan); break;
                                            case 1: block.setFillColor(sf::Color::Yellow); break;
                                            case 2: block.setFillColor(sf::Color::Magenta); break;
                                            case 3: block.setFillColor(sf::Color::Green); break;
                                            case 4: block.setFillColor(sf::Color::Red); break;
                                            case 5: block.setFillColor(sf::Color::Blue); break;
                                            case 6: block.setFillColor(sf::Color(255,140,0)); break;
                                            default: block.setFillColor(sf::Color::White); break;
                                        }
                                        block.setOutlineThickness(2);
                                        block.setOutlineColor(sf::Color::Black);
                                        block.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                                        ventana.getWindow().draw(block);
                                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE-4) / 4, (BLOCK_SIZE-4) / 4));
                                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                                        brillo.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                                        ventana.getWindow().draw(brillo);
                                    }
                            // Dibuja la UI lateral y la próxima pieza
                            dibujarUILateral(puntaje, nivel, fuente);
                            dibujarProximaPieza();
                            ventana.mostrar();
                            sf::sleep(sf::seconds(duracionParpadeo));
                        }
                    }
                    // Eliminar filas completas
                    int lineasEliminadas = 0;
                    for (int idx = filasCompletas.size() - 1; idx >= 0; --idx) {
                        int fila = filasCompletas[idx];
                        tablero.matriz.erase(tablero.matriz.begin() + fila);
                        tablero.matriz.insert(tablero.matriz.begin(), std::vector<int>(Tablero::ANCHO, 0));
                        ++lineasEliminadas;
                    }
                    if (lineasEliminadas > 0) {
                        sonidoLinea.reproducir();
                    }
                    puntaje += lineasEliminadas * 100;
                    nivel = 1 + puntaje / 1000;
                    delay = velocidadBase / nivel;
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

            // Agregar un recuadro a la derecha del tablero y la UI lateral
            dibujarUILateral(puntaje, nivel, fuente);

            // Cargar la fuente Arial desde la ruta correcta
            sf::Font fuente;
            fuente.loadFromFile("assets/fonts/Pixel.ttf"); // No mostrar mensaje de error

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
            if (enPausa) {
                // Limpiar la ventana antes de dibujar
                ventana.getWindow().clear();

                // --- Dibuja la UI lateral durante el menú de pausa ---
                dibujarUILateral(puntaje, nivel, fuente);

                // Dibujar el menú de pausa centrado usando la fuente miembro
                dibujarMenuPausa(ventana.getWindow(), fuente);

                ventana.getWindow().display();

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
    // Liberar memoria de piezaActual y proximaPieza SOLO al final del juego
    if (piezaActual) {
        delete piezaActual;
        piezaActual = nullptr;
    }
    if (proximaPieza) {
        delete proximaPieza;
        proximaPieza = nullptr;
    }
}