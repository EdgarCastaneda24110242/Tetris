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
struct BloqueFondo
{
    int tipo; // 0-6
    float x, y;
    float velocidad;
    float rotacion;
    float rotacionVelocidad;
    float opacidad;
};

Juego::Juego(sf::RenderWindow &ventanaPrincipal)
    : ventana(ventanaPrincipal), gameOver(false), musica("assets/music/Tetris.ogg")
{
    piezaActual = nullptr;
    proximaPieza = new Pieza(rand() % 7); // Inicializar proximaPieza con una nueva pieza
    puntaje = 0;

    // Cargar textura para las piezas
    if (!texturaPiezas.loadFromFile("assets/textures/piezas_pixelart.png"))
    {
        // Eliminado mensaje de error para evitar spam en la terminal
    }

    // Cargar la fuente Pixel.ttf una sola vez para toda la UI
    if (!fuente.loadFromFile("assets/fonts/Pixel.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente Pixel.ttf" << std::endl;
    }

    // No reproducir música aquí, solo cuando empiece el juego
}

void Juego::CrearNuevaPieza()
{
    // Asignar la próxima pieza como actual (sin liberar memoria aquí)
    piezaActual = proximaPieza;
    // Crear una nueva próxima pieza
    proximaPieza = new Pieza(rand() % 7);
    // Inicializar posición de la pieza actual
    if (piezaActual)
    {
        piezaActual->x = 3;
        piezaActual->y = 0;
        if (tablero.VerificarColision(*piezaActual))
            gameOver = true;
    }
    else
    {
        gameOver = true;
        return;
    }
}

void Juego::DibujarProximaPieza()
{
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
    for (size_t i = 0; i < proximaPieza->forma.size(); ++i)
    {
        for (size_t j = 0; j < proximaPieza->forma[i].size(); ++j)
        {
            if (proximaPieza->forma[i][j] != 0)
            {
                sf::RectangleShape bloque(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4));
                bloque.setFillColor(ColorPieza::Obtener(proximaPieza->tipo + 1));
                bloque.setOutlineThickness(2);
                bloque.setOutlineColor(sf::Color::Black);
                bloque.setPosition(piezaOffsetX + j * BLOCK_SIZE, piezaOffsetY + i * BLOCK_SIZE);
                ventana.ObtenerVentana().draw(bloque);
            }
        }
    }
}

// Método para dibujar el menú de pausa de forma clara y reutilizable
void Juego::DibujarMenuPausa(sf::RenderWindow &window, const sf::Font &fuente)
{
    sf::Vector2u tamanoVentana = window.getSize();
    int centroX = tamanoVentana.x / 2 - 80; // Ajuste a la derecha
    // Título "PAUSA" con fuente especial
    sf::Font fuenteTetrominoes;
    if (fuenteTetrominoes.loadFromFile("assets/fonts/tetrominoes.ttf"))
    {
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
    }
    else
    {
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

void Juego::DibujarUILateral(int puntaje, int nivel, sf::Font &fuente)
{
    // Recuadro derecho
    sf::RectangleShape recuadroDerecho(sf::Vector2f(6 * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE));
    recuadroDerecho.setFillColor(sf::Color(0, 0, 0, 210));
    recuadroDerecho.setOutlineThickness(2);
    recuadroDerecho.setOutlineColor(sf::Color::White);
    recuadroDerecho.setPosition(Tablero::ANCHO * BLOCK_SIZE, 0);
    ventana.ObtenerVentana().draw(recuadroDerecho);
    // Texto PUNTOS
    sf::Text textoPuntos;
    textoPuntos.setFont(fuente);
    textoPuntos.setString("PUNTOS");
    textoPuntos.setCharacterSize(32);
    textoPuntos.setFillColor(sf::Color::White);
    textoPuntos.setStyle(sf::Text::Bold);
    textoPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 3, BLOCK_SIZE + 63);
    ventana.ObtenerVentana().draw(textoPuntos);
    // Recuadro pequeño debajo de PUNTOS
    sf::RectangleShape recuadro(sf::Vector2f(120, 120));
    recuadro.setFillColor(sf::Color(30, 30, 30));
    recuadro.setOutlineThickness(2);
    recuadro.setOutlineColor(sf::Color::White);
    recuadro.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, 300);
    ventana.ObtenerVentana().draw(recuadro);
    // Recuadro de puntos acumulados
    sf::RectangleShape recuadroPuntos(sf::Vector2f(120, 50));
    recuadroPuntos.setFillColor(sf::Color(30, 30, 30));
    recuadroPuntos.setOutlineThickness(2);
    recuadroPuntos.setOutlineColor(sf::Color::White);
    recuadroPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, BLOCK_SIZE + 120);
    ventana.ObtenerVentana().draw(recuadroPuntos);
    // Texto de puntos acumulados
    sf::Text textoAcumulado;
    textoAcumulado.setFont(fuente);
    textoAcumulado.setString(std::to_string(puntaje));
    textoAcumulado.setCharacterSize(24);
    textoAcumulado.setFillColor(sf::Color::White);
    textoAcumulado.setStyle(sf::Text::Bold);
    textoAcumulado.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, BLOCK_SIZE + 130);
    ventana.ObtenerVentana().draw(textoAcumulado);
    // Texto SIG PIEZA
    sf::Text textoSigPieza;
    textoSigPieza.setFont(fuente);
    textoSigPieza.setString("SIG PIEZA");
    textoSigPieza.setCharacterSize(24);
    textoSigPieza.setFillColor(sf::Color::White);
    textoSigPieza.setStyle(sf::Text::Bold);
    textoSigPieza.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 5, 250);
    ventana.ObtenerVentana().draw(textoSigPieza);
    // Texto NIVEL
    sf::Text textoNivel;
    textoNivel.setFont(fuente);
    textoNivel.setString("NIVEL");
    textoNivel.setCharacterSize(24);
    textoNivel.setFillColor(sf::Color::White);
    textoNivel.setStyle(sf::Text::Bold);
    textoNivel.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, 463);
    ventana.ObtenerVentana().draw(textoNivel);
    // Texto nivel actual
    sf::Text textoNivelActual;
    textoNivelActual.setFont(fuente);
    textoNivelActual.setString(std::to_string(nivel));
    textoNivelActual.setCharacterSize(24);
    textoNivelActual.setFillColor(sf::Color::White);
    textoNivelActual.setStyle(sf::Text::Bold);
    textoNivelActual.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 63.5, 512);
    ventana.ObtenerVentana().draw(textoNivelActual);
    // Próxima pieza
    DibujarProximaPieza();
}

void Juego::MostrarMenuPrincipal()
{
    bool enMenu = true;
    int opcionSeleccionada = 0;
    std::vector<std::string> opciones = {"Jugar", "Scores", "Salir"};
    sf::Font fuenteTetrominoes;
    fuenteTetrominoes.loadFromFile("assets/fonts/tetrominoes.ttf");
    Musica musicaMenu("assets/music/TetrisMenu.ogg");
    musicaMenu.EstablecerVolumen(10); // Volumen bajo para el menú principal
    musicaMenu.Reproducir();

    // Inicializar generador aleatorio
    srand(static_cast<unsigned int>(time(nullptr)));
    // Bloques de fondo animados
    std::vector<BloqueFondo> bloquesFondo;
    const int piezaMaxAncho = 4 * BLOCK_SIZE; // Máximo ancho de una pieza (pieza I)
    int numColumnas = ventana.ObtenerVentana().getSize().x / piezaMaxAncho;
    std::vector<bool> columnasOcupadas(numColumnas, false);
    const int numBloquesFondo = std::min(numColumnas, 18); // No más bloques que columnas
    for (int i = 0; i < numBloquesFondo; ++i)
    {
        BloqueFondo b;
        b.tipo = rand() % 7;
        Pieza piezaTmp(b.tipo);
        int piezaAncho = piezaTmp.forma[0].size() * BLOCK_SIZE;
        // Buscar columna libre
        int col;
        do
        {
            col = rand() % numColumnas;
        } while (columnasOcupadas[col]);
        columnasOcupadas[col] = true;
        b.x = col * piezaMaxAncho + rand() % (piezaMaxAncho - piezaAncho + 1);
        b.y = static_cast<float>(rand() % ventana.ObtenerVentana().getSize().y);
        b.velocidad = 60 + rand() % 80; // píxeles por segundo
        b.rotacion = 0;                 // Sin rotación
        b.rotacionVelocidad = 0;
        b.opacidad = 80 + rand() % 60; // 80-140
        b.rotacion = 0;
        bloquesFondo.push_back(b);
    }
    sf::Clock clockFondo;

    while (enMenu && ventana.VerificarSiEstaAbierta())
    {
        float dt = clockFondo.restart().asSeconds();
        // Actualizar bloques de fondo
        for (int i = 0; i < bloquesFondo.size(); ++i)
        {
            auto &b = bloquesFondo[i];
            b.y += b.velocidad * dt;
            if (b.y > ventana.ObtenerVentana().getSize().y + 40)
            {
                // Liberar columna anterior
                int colAnterior = (int)((b.x) / piezaMaxAncho);
                columnasOcupadas[colAnterior] = false;
                // Resetear bloque en nueva columna libre
                b.tipo = rand() % 7;
                Pieza piezaTmp(b.tipo);
                int piezaAncho = piezaTmp.forma[0].size() * BLOCK_SIZE;
                int col;
                do
                {
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
        ventana.ObtenerVentana().clear();
        // Dibujar bloques de fondo
        for (const auto &b : bloquesFondo)
        {
            Pieza pieza(b.tipo);
            for (size_t i = 0; i < pieza.forma.size(); ++i)
            {
                for (size_t j = 0; j < pieza.forma[i].size(); ++j)
                {
                    if (pieza.forma[i][j])
                    {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4));
                        block.setOrigin((BLOCK_SIZE - 4) / 2, (BLOCK_SIZE - 4) / 2);
                        block.setPosition(b.x + j * BLOCK_SIZE, b.y + i * BLOCK_SIZE);
                        sf::Color color = ColorPieza::Obtener(b.tipo + 1);
                        color.a = static_cast<sf::Uint8>(b.opacidad);
                        block.setFillColor(color);
                        block.setOutlineThickness(1);
                        block.setOutlineColor(sf::Color(0, 0, 0, 80));
                        ventana.ObtenerVentana().draw(block);
                    }
                }
            }
        }
        // Título con fuente tetrominoes
        sf::Text titulo;
        if (fuenteTetrominoes.getInfo().family != "")
        {
            titulo.setFont(fuenteTetrominoes);
        }
        else
        {
            titulo.setFont(fuente);
        }
        titulo.setString("TETRIS");
        titulo.setCharacterSize(60);
        titulo.setFillColor(sf::Color::Red);
        titulo.setStyle(sf::Text::Bold);
        sf::FloatRect boundsTitulo = titulo.getLocalBounds();
        titulo.setOrigin(boundsTitulo.width / 2, boundsTitulo.height / 2);
        titulo.setPosition(ventana.ObtenerVentana().getSize().x / 2, 120);
        ventana.ObtenerVentana().draw(titulo);
        // Subtítulo "by Fer and Edgar" con fuente Pixel.ttf
        sf::Text subtitulo;
        subtitulo.setFont(fuente);
        subtitulo.setString("by Fer and Edgar");
        subtitulo.setCharacterSize(24);
        subtitulo.setFillColor(sf::Color::White);
        subtitulo.setStyle(sf::Text::Bold);
        sf::FloatRect boundsSub = subtitulo.getLocalBounds();
        subtitulo.setOrigin(boundsSub.width / 2, boundsSub.height / 2);
        subtitulo.setPosition(ventana.ObtenerVentana().getSize().x / 2, 170);
        ventana.ObtenerVentana().draw(subtitulo);
        // Opciones
        for (size_t i = 0; i < opciones.size(); ++i)
        {
            sf::Text opcion;
            opcion.setFont(fuente);
            opcion.setString(opciones[i]);
            opcion.setCharacterSize(36);
            opcion.setFillColor(i == opcionSeleccionada ? sf::Color::Yellow : sf::Color::White);
            opcion.setStyle(sf::Text::Bold);
            sf::FloatRect boundsOpcion = opcion.getLocalBounds();
            opcion.setOrigin(boundsOpcion.width / 2, boundsOpcion.height / 2);
            opcion.setPosition(ventana.ObtenerVentana().getSize().x / 2, 250 + i * 60);
            ventana.ObtenerVentana().draw(opcion);
        }

        ventana.ObtenerVentana().display();
        // Manejo de eventos
        sf::Event event;
        while (ventana.ProcesarEvento(event))
        {
            if (event.type == sf::Event::Closed)
            {
                ventana.ObtenerVentana().close();
                return;
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
                {
                    opcionSeleccionada = (opcionSeleccionada + opciones.size() - 1) % opciones.size();
                }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                {
                    opcionSeleccionada = (opcionSeleccionada + 1) % opciones.size();
                }
                else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Return)
                {
                    if (opcionSeleccionada == 0)
                    {
                        // Iniciar el juego
                        gameOver = false;
                        puntaje = 0;
                        nivel = 1;
                        if (piezaActual)
                            delete piezaActual;
                        if (proximaPieza)
                            delete proximaPieza;
                        piezaActual = new Pieza(rand() % 7);
                        proximaPieza = new Pieza(rand() % 7);
                        tablero = Tablero();
                        enMenu = false;
                    }
                    else if (opcionSeleccionada == 1)
                    {
                        // Mostrar récords en la misma pantalla with actualización de ventana
                        std::vector<std::pair<int, std::string>> records;
                        std::ifstream archivoRecord("record.txt");
                        if (archivoRecord.is_open())
                        {
                            int puntos;
                            std::string nombre;
                            while (archivoRecord >> puntos && std::getline(archivoRecord, nombre))
                            {
                                if (!nombre.empty() && nombre[0] == ' ')
                                {
                                    nombre = nombre.substr(1);
                                }
                                records.emplace_back(puntos, nombre);
                            }
                            archivoRecord.close();
                        }
                        std::sort(records.begin(), records.end(), [](const std::pair<int, std::string> &a, const std::pair<int, std::string> &b)
                                  { return a.first > b.first; });
                        bool mostrandoRecords = true;
                        while (mostrandoRecords)
                        {
                            ventana.ObtenerVentana().clear();
                            sf::Text titulo;
                            titulo.setFont(fuente);
                            titulo.setString("MEJORES PUNTAJES\n\n"); // Agregar dos saltos de línea para separación
                            titulo.setCharacterSize(36);
                            titulo.setFillColor(sf::Color::White);
                            titulo.setStyle(sf::Text::Bold);
                            titulo.setPosition(ventana.ObtenerVentana().getSize().x / 2 - titulo.getLocalBounds().width / 2, ventana.ObtenerVentana().getSize().y / 2 - titulo.getLocalBounds().height / 2 - 100); // Centrar horizontal y verticalmente con un desplazamiento hacia arriba
                            ventana.ObtenerVentana().draw(titulo);
                            for (size_t i = 0; i < records.size() && i < 10; ++i)
                            {
                                sf::Text textoRecord;
                                textoRecord.setFont(fuente);
                                textoRecord.setString(std::to_string(i + 1) + ". " + records[i].second + " - " + std::to_string(records[i].first));
                                textoRecord.setCharacterSize(24);
                                textoRecord.setFillColor(sf::Color::Yellow);
                                float yOffset = ventana.ObtenerVentana().getSize().y / 2 - (records.size() * 30) / 2 + i * 30 + 20; // Reducir aún más el espacio entre el título y los puntajes
                                textoRecord.setPosition(ventana.ObtenerVentana().getSize().x / 2 - textoRecord.getLocalBounds().width / 2, yOffset);
                                ventana.ObtenerVentana().draw(textoRecord);
                            }
                            ventana.ObtenerVentana().display();
                            // Manejar eventos para salir de la pantalla de récords
                            sf::Event event;
                            while (ventana.ProcesarEvento(event))
                            {
                                if (event.type == sf::Event::KeyPressed)
                                {
                                    mostrandoRecords = false;
                                    break;
                                }
                            }
                        }
                    }
                    else if (opcionSeleccionada == 2)
                    {
                        ventana.ObtenerVentana().close();
                    }
                }
            }
        }
    }
    musicaMenu.Detener();
    musica.EstablecerVolumen(20); // Restaurar volumen normal al salir del menú
}

void Juego::Jugar()
{
    MostrarMenuPrincipal();
    musica.Detener();    // Detener cualquier música previa (incluida la del menú)
    musica.Reproducir(); // Iniciar la música de fondo del juego
    // Crear un objeto de sonido para MovePieza
    Audio sonidoMover("assets/music/MovePieza.ogg");
    sonidoMover.EstablecerVolumen(18); // Reducir el volumen tres veces más

    // Crear un objeto de sonido para HitPieza
    Audio sonidoHit("assets/music/HitPieza.ogg");
    sonidoHit.EstablecerVolumen(75); // Reducir el volumen tres veces más

    // Reducir el volumen de la música de fondo Tetris
    musica.EstablecerVolumen(20); // Volumen más bajo

    sf::Clock clock;
    float timer = 0, delay = 0.5;

    CrearNuevaPieza();

    // Eliminar la reproducción del sonido de inicio aquí para evitar duplicados
    // Audio sonidoInicio("assets/music/EmpezarJuego.ogg");
    // sonidoInicio.Reproducir();

    // Inicializar nivel y puntaje
    int nivel = 1;
    int puntaje = 0;

    // Ajustar la velocidad de caída según el nivel
    const float velocidadBase = 0.5f;
    delay = velocidadBase;

    // Reproducir el sonido al eliminar líneas
    Audio sonidoLinea("assets/music/LineaPunto.ogg");
    sonidoLinea.EstablecerVolumen(50); // Ajustar el volumen

    bool enPausa = false; // Variable para rastrear el estado de pausa

    while (ventana.VerificarSiEstaAbierta() && !gameOver)
    {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event e;
        while (ventana.ProcesarEvento(e))
        {
            if (e.type == sf::Event::Closed)
                ventana.ObtenerVentana().close();

            // Detectar la tecla 'Q' para alternar entre pausa y reanudar
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Q)
            {
                enPausa = !enPausa; // Alternar el estado de pausa
                if (enPausa)
                {
                    musica.Pausar(); // Pausar música
                }
                else
                {
                    musica.Reanudar(); // Reanudar música
                }
                continue; // Asegurar que el estado de pausa se procese correctamente
            }

            if (!enPausa)
            {
                Controles::Accion accion = Controles::ProcesarEvento(e);
                switch (accion)
                {
                case Controles::MoverIzquierda:
                    piezaActual->x--;
                    if (tablero.VerificarColision(*piezaActual))
                        piezaActual->x++;
                    break;
                case Controles::MoverDerecha:
                    piezaActual->x++;
                    if (tablero.VerificarColision(*piezaActual))
                        piezaActual->x--;
                    break;
                case Controles::Bajar:
                    piezaActual->y++;
                    if (tablero.VerificarColision(*piezaActual))
                    {
                        piezaActual->y--;
                        tablero.FijarPieza(*piezaActual);
                        sonidoHit.Reproducir();
                        // Eliminar todas las líneas completas correctamente
                        int lineasEliminadas = 0;
                        for (int i = Tablero::ALTO - 1; i >= 0; --i)
                        {
                            bool llena = true;
                            for (int j = 0; j < Tablero::ANCHO; ++j)
                            {
                                if (!tablero.matriz[i][j])
                                    llena = false;
                            }
                            if (llena)
                            {
                                tablero.matriz.erase(tablero.matriz.begin() + i);
                                tablero.matriz.insert(tablero.matriz.begin(), std::vector<int>(Tablero::ANCHO, 0));
                                ++lineasEliminadas;
                                ++i; // Revisar de nuevo la misma fila
                            }
                        }
                        if (lineasEliminadas > 0)
                        {
                            sonidoLinea.Reproducir();
                        }
                        puntaje += lineasEliminadas * 100;
                        nivel = 1 + puntaje / 1000;
                        delay = velocidadBase / nivel;
                        CrearNuevaPieza();
                        if (gameOver)
                            break; // Salir del bucle si se pierde
                    }
                    break;
                case Controles::Rotar:
                    piezaActual->Rotar();
                    if (tablero.VerificarColision(*piezaActual))
                    {
                        piezaActual->Rotar();
                    }
                    else
                    {
                        // Reproducir el sonido al rotar la pieza
                        sonidoMover.Reproducir();
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
            else
            {
                musica.Pausar(); // Pausar música al pausar el juego
            }
        }

        if (enPausa)
        {
            musica.Pausar();
            // Dibuja el estado actual del juego y overlay de pausa
            ventana.Limpiar();
            // Dibuja el tablero
            for (int i = 0; i < Tablero::ALTO; ++i)
            {
                for (int j = 0; j < Tablero::ANCHO; ++j)
                {
                    if (tablero.matriz[i][j])
                    {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4));
                        block.setFillColor(sf::Color(128, 128, 128));
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);
                        block.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(block);
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE - 4) / 4, (BLOCK_SIZE - 4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                        brillo.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(brillo);
                    }
                }
            }
            // Dibuja la pieza actual
            for (size_t i = 0; i < piezaActual->forma.size(); ++i)
                for (size_t j = 0; j < piezaActual->forma[0].size(); ++j)
                    if (piezaActual->forma[i][j])
                    {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4));
                        switch (piezaActual->tipo)
                        {
                        case 0:
                            block.setFillColor(sf::Color::Cyan);
                            break;
                        case 1:
                            block.setFillColor(sf::Color::Yellow);
                            break;
                        case 2:
                            block.setFillColor(sf::Color::Magenta);
                            break;
                        case 3:
                            block.setFillColor(sf::Color::Green);
                            break;
                        case 4:
                            block.setFillColor(sf::Color::Red);
                            break;
                        case 5:
                            block.setFillColor(sf::Color::Blue);
                            break;
                        case 6:
                            block.setFillColor(sf::Color(255, 140, 0));
                            break;
                        default:
                            block.setFillColor(sf::Color::White);
                            break;
                        }
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);
                        block.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(block);
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE - 4) / 4, (BLOCK_SIZE - 4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                        brillo.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(brillo);
                    }
            // Dibuja la UI lateral completa (recuadro, textos, puntuación, próxima pieza)
            DibujarUILateral(puntaje, nivel, fuente);

            // 2. Dibuja overlay negro translúcido SOLO sobre el área del tablero (no sobre la UI lateral)
            sf::RectangleShape overlay(sf::Vector2f(Tablero::ANCHO * BLOCK_SIZE, Tablero::ALTO * BLOCK_SIZE));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            overlay.setPosition(0, 0);
            ventana.ObtenerVentana().draw(overlay);

            // 3. Dibuja el menú de pausa centrado
            DibujarMenuPausa(ventana.ObtenerVentana(), fuente);
            ventana.ObtenerVentana().display();
            // Pausar el bucle hasta que se reanude, reinicie o regrese al menú
            bool salirPausa = false;
            bool regresarMenu = false;
            while (enPausa && !salirPausa && !regresarMenu)
            {
                sf::Event eventoPausa;
                while (ventana.ProcesarEvento(eventoPausa))
                {
                    if (eventoPausa.type == sf::Event::KeyPressed)
                    {
                        if (eventoPausa.key.code == sf::Keyboard::Q)
                        {
                            enPausa = false; // Reanudar
                        }
                        if (eventoPausa.key.code == sf::Keyboard::R)
                        {
                            // Transición: pantalla negra y sonido de inicio
                            ventana.ObtenerVentana().clear(sf::Color::Black);
                            ventana.ObtenerVentana().display();
                            Audio sonidoInicio("assets/music/EmpezarJuego.ogg");
                            sonidoInicio.EstablecerVolumen(20);
                            sonidoInicio.Reproducir();
                            sf::sleep(sf::seconds(0.7f)); // Aumenta el tiempo de pantalla negra
                            // Reiniciar la música de fondo
                            musica.Detener();
                            musica.Reproducir();
                            // Reiniciar el juego y quitar pausa automáticamente
                            puntaje = 0;
                            nivel = 1;
                            delay = velocidadBase;
                            tablero = Tablero();
                            CrearNuevaPieza();
                            enPausa = false;
                            salirPausa = true;
                        }
                        if (eventoPausa.key.code == sf::Keyboard::M)
                        {
                            ventana.ObtenerVentana().close(); // Ahora sí cerrar el juego
                            salirPausa = true;
                        }
                    }
                }
            }
            if (regresarMenu)
                return; // Salir de Jugar() y volver al menú principal
            if (salirPausa)
                continue; // Salta el resto del ciclo para reiniciar
        }
        else
        {
            musica.Reanudar();
        }

        if (!enPausa)
        {
            // Actualizar la lógica del juego solo si no está en pausa
            if (timer > delay)
            {
                piezaActual->y++;
                if (tablero.VerificarColision(*piezaActual))
                {
                    piezaActual->y--;
                    tablero.FijarPieza(*piezaActual);
                    sonidoHit.Reproducir();
                    // Detectar filas completas
                    std::vector<int> filasCompletas;
                    for (int i = Tablero::ALTO - 1; i >= 0; --i)
                    {
                        bool llena = true;
                        for (int j = 0; j < Tablero::ANCHO; ++j)
                        {
                            if (!tablero.matriz[i][j])
                                llena = false;
                        }
                        if (llena)
                        {
                            filasCompletas.push_back(i);
                        }
                    }
                    // Animación de parpadeo blanco para filas completas
                    if (!filasCompletas.empty())
                    {
                        const int parpadeos = 6;              // Número de parpadeos
                        const float duracionParpadeo = 0.06f; // Duración de cada parpadeo (segundos)
                        for (int p = 0; p < parpadeos; ++p)
                        {
                            ventana.Limpiar();
                            // Dibuja el tablero con filas parpadeando
                            for (int i = 0; i < Tablero::ALTO; ++i)
                            {
                                for (int j = 0; j < Tablero::ANCHO; ++j)
                                {
                                    if (tablero.matriz[i][j])
                                    {
                                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4));
                                        bool esFilaParpadeo = std::find(filasCompletas.begin(), filasCompletas.end(), i) != filasCompletas.end();
                                        if (esFilaParpadeo)
                                        {
                                            if (p % 2 == 0)
                                                block.setFillColor(sf::Color::White); // Parpadeo blanco
                                            else
                                                block.setFillColor(sf::Color(200, 200, 200)); // Gris claro
                                        }
                                        else
                                        {
                                            block.setFillColor(sf::Color(128, 128, 128));
                                        }
                                        block.setOutlineThickness(2);
                                        block.setOutlineColor(sf::Color::Black);
                                        block.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                                        ventana.ObtenerVentana().draw(block);
                                        // Brillo pixelado
                                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE - 4) / 4, (BLOCK_SIZE - 4) / 4));
                                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                                        brillo.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                                        ventana.ObtenerVentana().draw(brillo);
                                    }
                                }
                            }
                            // Dibuja la pieza actual y UI
                            for (size_t i = 0; i < piezaActual->forma.size(); ++i)
                                for (size_t j = 0; j < piezaActual->forma[0].size(); ++j)
                                    if (piezaActual->forma[i][j])
                                    {
                                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4));
                                        switch (piezaActual->tipo)
                                        {
                                        case 0:
                                            block.setFillColor(sf::Color::Cyan);
                                            break;
                                        case 1:
                                            block.setFillColor(sf::Color::Yellow);
                                            break;
                                        case 2:
                                            block.setFillColor(sf::Color::Magenta);
                                            break;
                                        case 3:
                                            block.setFillColor(sf::Color::Green);
                                            break;
                                        case 4:
                                            block.setFillColor(sf::Color::Red);
                                            break;
                                        case 5:
                                            block.setFillColor(sf::Color::Blue);
                                            break;
                                        case 6:
                                            block.setFillColor(sf::Color(255, 140, 0));
                                            break;
                                        default:
                                            block.setFillColor(sf::Color::White);
                                            break;
                                        }
                                        block.setOutlineThickness(2);
                                        block.setOutlineColor(sf::Color::Black);
                                        block.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                                        ventana.ObtenerVentana().draw(block);
                                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE - 4) / 4, (BLOCK_SIZE - 4) / 4));
                                        brillo.setFillColor(sf::Color(255, 255, 255, 150));
                                        brillo.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                                        ventana.ObtenerVentana().draw(brillo);
                                    }
                            ventana.Mostrar();
                            sf::sleep(sf::seconds(duracionParpadeo));
                        }
                    }
                    // Eliminar filas completas
                    int lineasEliminadas = 0;
                    for (int idx = filasCompletas.size() - 1; idx >= 0; --idx)
                    {
                        int fila = filasCompletas[idx];
                        tablero.matriz.erase(tablero.matriz.begin() + fila);
                        tablero.matriz.insert(tablero.matriz.begin(), std::vector<int>(Tablero::ANCHO, 0));
                        ++lineasEliminadas;
                    }
                    if (lineasEliminadas > 0)
                    {
                        sonidoLinea.Reproducir();
                    }
                    puntaje += lineasEliminadas * 100;
                    nivel = 1 + puntaje / 1000;
                    delay = velocidadBase / nivel;
                    CrearNuevaPieza();
                }
                timer = 0;
            }

            ventana.Limpiar();

            // Dibuja el tablero with un color uniforme para las piezas fijadas
            for (int i = 0; i < Tablero::ALTO; ++i)
            {
                for (int j = 0; j < Tablero::ANCHO; ++j)
                {
                    if (tablero.matriz[i][j])
                    {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4)); // Bloques más pequeños para simular píxeles

                        // Asignar un color uniforme para las piezas fijadas
                        block.setFillColor(sf::Color(128, 128, 128)); // Gris metálico

                        // Agregar un borde grueso negro para un efecto retro
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);

                        block.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(block);

                        // Agregar brillo pixelado en la esquina superior izquierda
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE - 4) / 4, (BLOCK_SIZE - 4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150)); // Color blanco translúcido para el brillo
                        brillo.setPosition(j * BLOCK_SIZE + 2, i * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(brillo);
                    }
                }
            }

            // Dibuja la pieza actual con un estilo retro arcade y brillo pixelado
            for (size_t i = 0; i < piezaActual->forma.size(); ++i)
                for (size_t j = 0; j < piezaActual->forma[0].size(); ++j)
                    if (piezaActual->forma[i][j])
                    {
                        sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 4, BLOCK_SIZE - 4)); // Bloques más pequeños para simular píxeles

                        // Asignar colores sólidos brillantes para un estilo retro arcade
                        switch (piezaActual->tipo)
                        {
                        case 0:
                            block.setFillColor(sf::Color::Cyan);
                            break; // I - Cyan
                        case 1:
                            block.setFillColor(sf::Color::Yellow);
                            break; // O - Amarillo
                        case 2:
                            block.setFillColor(sf::Color::Magenta);
                            break; // T - Magenta
                        case 3:
                            block.setFillColor(sf::Color::Green);
                            break; // S - Verde
                        case 4:
                            block.setFillColor(sf::Color::Red);
                            break; // Z - Rojo
                        case 5:
                            block.setFillColor(sf::Color::Blue);
                            break; // J - Azul
                        case 6:
                            block.setFillColor(sf::Color(255, 140, 0));
                            break; // L - Naranja
                        default:
                            block.setFillColor(sf::Color::White);
                            break; // Blanco
                        }

                        // Agregar un borde grueso negro para un efecto retro
                        block.setOutlineThickness(2);
                        block.setOutlineColor(sf::Color::Black);

                        block.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(block);

                        // Agregar brillo pixelado en la esquina superior izquierda
                        sf::RectangleShape brillo(sf::Vector2f((BLOCK_SIZE - 4) / 4, (BLOCK_SIZE - 4) / 4));
                        brillo.setFillColor(sf::Color(255, 255, 255, 150)); // Color blanco translúcido para el brillo
                        brillo.setPosition((piezaActual->x + j) * BLOCK_SIZE + 2, (piezaActual->y + i) * BLOCK_SIZE + 2);
                        ventana.ObtenerVentana().draw(brillo);
                    }

            // Agregar un recuadro a la derecha del tablero y la UI lateral
            DibujarUILateral(puntaje, nivel, fuente);

            // Cargar la fuente Arial desde la ruta correcta
            sf::Font fuente;
            fuente.loadFromFile("assets/fonts/Pixel.ttf"); // No mostrar mensaje de error

            // Configurar y dibujar el texto 'PUNTOS'
            sf::Text textoPuntos;
            textoPuntos.setFont(fuente);
            textoPuntos.setString("PUNTOS");
            textoPuntos.setCharacterSize(32);                                                       // Incrementar el tamaño de letra
            textoPuntos.setFillColor(sf::Color::White);                                             // Color blanco
            textoPuntos.setStyle(sf::Text::Bold);                                                   // Negrita
            textoPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 3, BLOCK_SIZE + 63); // Ajuste de posición para mayor visibilidad

            // Dibujar el texto 'PUNTOS'
            ventana.ObtenerVentana().draw(textoPuntos);

            // Dibujar un recuadro pequeño más abajo del letrero de "PUNTOS"
            sf::RectangleShape recuadro(sf::Vector2f(120, 120)); // Tamaño del recuadro
            recuadro.setFillColor(sf::Color(30, 30, 30));        // Color de fondo
            recuadro.setOutlineThickness(2);
            recuadro.setOutlineColor(sf::Color::White);                               // Borde blanco
            recuadro.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, 300); // Posición más abajo sin cambiar horizontalmente
            ventana.ObtenerVentana().draw(recuadro);

            // Dibujar un recuadro para mostrar los puntos acumulados
            sf::RectangleShape recuadroPuntos(sf::Vector2f(120, 50)); // Tamaño del recuadro
            recuadroPuntos.setFillColor(sf::Color(30, 30, 30));       // Color de fondo
            recuadroPuntos.setOutlineThickness(2);
            recuadroPuntos.setOutlineColor(sf::Color::White);                                            // Borde blanco
            recuadroPuntos.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 10, BLOCK_SIZE + 120); // Posición debajo del texto 'PUNTOS'
            ventana.ObtenerVentana().draw(recuadroPuntos);

            // Configurar y dibujar el texto de los puntos acumulados
            sf::Text textoAcumulado;
            textoAcumulado.setFont(fuente);
            textoAcumulado.setString(std::to_string(puntaje));                                           // Mostrar el puntaje actual
            textoAcumulado.setCharacterSize(24);                                                         // Tamaño de letra
            textoAcumulado.setFillColor(sf::Color::White);                                               // Color blanco
            textoAcumulado.setStyle(sf::Text::Bold);                                                     // Negrita
            textoAcumulado.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, BLOCK_SIZE + 130); // Posición dentro del recuadro

            // Dibujar el texto de los puntos acumulados
            ventana.ObtenerVentana().draw(textoAcumulado);

            // Configurar y dibujar el texto 'SIG PIEZA'
            sf::Text textoSigPieza;
            textoSigPieza.setFont(fuente);
            textoSigPieza.setString("SIG PIEZA");
            textoSigPieza.setCharacterSize(24);                                           // Tamaño de letra
            textoSigPieza.setFillColor(sf::Color::White);                                 // Color blanco
            textoSigPieza.setStyle(sf::Text::Bold);                                       // Negrita
            textoSigPieza.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 5, 250); // Posición arriba del recuadro

            // Dibujar el texto 'SIG PIEZA'
            ventana.ObtenerVentana().draw(textoSigPieza);

            // Configurar y dibujar el texto 'NIVEL'
            sf::Text textoNivel;
            textoNivel.setFont(fuente);
            textoNivel.setString("NIVEL");
            textoNivel.setCharacterSize(24);                                            // Tamaño de letra
            textoNivel.setFillColor(sf::Color::White);                                  // Color blanco
            textoNivel.setStyle(sf::Text::Bold);                                        // Negrita
            textoNivel.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 40, 463); // Posición adecuada

            // Dibujar el texto 'NIVEL'
            ventana.ObtenerVentana().draw(textoNivel);

            // Configurar y dibujar el texto del nivel actual
            sf::Text textoNivelActual;
            textoNivelActual.setFont(fuente);
            textoNivelActual.setString(std::to_string(nivel));                                  // Mostrar el nivel actual
            textoNivelActual.setCharacterSize(24);                                              // Tamaño de letra
            textoNivelActual.setFillColor(sf::Color::White);                                    // Color blanco
            textoNivelActual.setStyle(sf::Text::Bold);                                          // Negrita
            textoNivelActual.setPosition(Tablero::ANCHO * BLOCK_SIZE + BLOCK_SIZE + 63.5, 512); // Posición debajo del texto 'NIVEL'

            // Dibujar el texto del nivel actual
            ventana.ObtenerVentana().draw(textoNivelActual);

            // Dibujar la próxima pieza
            DibujarProximaPieza();

            ventana.Mostrar();
        }
        else
        {
            if (enPausa)
            {
                // Limpiar la ventana antes de dibujar
                ventana.ObtenerVentana().clear();

                // --- Dibuja la UI lateral durante el menú de pausa ---
                DibujarUILateral(puntaje, nivel, fuente);

                // Dibujar el menú de pausa centrado usando la fuente miembro
                DibujarMenuPausa(ventana.ObtenerVentana(), fuente);

                ventana.ObtenerVentana().display();

                // Pausar el bucle hasta que se reanude
                while (enPausa)
                {
                    sf::Event eventoPausa;
                    while (ventana.ProcesarEvento(eventoPausa))
                    {
                        if (eventoPausa.type == sf::Event::KeyPressed && eventoPausa.key.code == sf::Keyboard::Q)
                        {
                            enPausa = false; // Reanudar el juego
                        }
                    }
                }
            }
        }
    }
    // Liberar memoria de piezaActual y proximaPieza SOLO al final del juego
    if (piezaActual)
    {
        delete piezaActual;
        piezaActual = nullptr;
    }
    if (proximaPieza)
    {
        delete proximaPieza;
        proximaPieza = nullptr;
    }
    if (gameOver)
    {
        // Detener la música de fondo
        musica.Detener();

        // Reproducir el sonido de GameOver
        Audio sonidoGameOver("assets/music/GameOver.ogg");
        sonidoGameOver.Reproducir();

        // Mostrar la pantalla de Game Over
        GameOverScreen gameOverScreen(ventana.ObtenerVentana());
        gameOverScreen.Mostrar();

        // Esperar mientras se reproduce la música de GameOver
        while (sonidoGameOver.VerificarSiEstaReproduciendo())
        {
            sf::sleep(sf::milliseconds(100));
        }

        // Verificar si se rompió el récord
        int recordActual = 0;
        std::ifstream archivoRecord("record.txt");
        if (archivoRecord.is_open())
        {
            archivoRecord >> recordActual;
            archivoRecord.close();
        }

        if (puntaje > recordActual)
        {
            // Mostrar mensaje de nuevo récord con mejor estética
            sf::Text textoRecord;
            textoRecord.setFont(fuente);
            textoRecord.setString("¡NUEVO RECORD!");
            textoRecord.setCharacterSize(36);
            textoRecord.setFillColor(sf::Color::Red);
            textoRecord.setStyle(sf::Text::Bold);
            textoRecord.setPosition(ventana.ObtenerVentana().getSize().x / 2 - textoRecord.getLocalBounds().width / 2, ventana.ObtenerVentana().getSize().y / 2 - 120);

            sf::Text textoInstruccion;
            textoInstruccion.setFont(fuente);
            textoInstruccion.setString("Ingresa tu nombre:");
            textoInstruccion.setCharacterSize(24);
            textoInstruccion.setFillColor(sf::Color::White);
            textoInstruccion.setPosition(ventana.ObtenerVentana().getSize().x / 2 - textoInstruccion.getLocalBounds().width / 2, ventana.ObtenerVentana().getSize().y / 2 - 70);

            // Recuadro para la entrada del nombre
            sf::RectangleShape recuadroNombre(sf::Vector2f(300, 50));
            recuadroNombre.setFillColor(sf::Color(0, 0, 0, 150)); // Fondo semitransparente
            recuadroNombre.setOutlineThickness(2);
            recuadroNombre.setOutlineColor(sf::Color::White);
            recuadroNombre.setPosition(ventana.ObtenerVentana().getSize().x / 2 - recuadroNombre.getSize().x / 2, ventana.ObtenerVentana().getSize().y / 2);

            // Capturar el nombre del jugador
            std::string nombre;
            bool capturandoNombre = true;
            while (capturandoNombre)
            {
                sf::Event event;
                while (ventana.ProcesarEvento(event))
                {
                    if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode == '\n' || event.text.unicode == '\r')
                        {
                            capturandoNombre = false; // Salir al presionar Enter
                        }
                        else if (event.text.unicode == 8 && !nombre.empty())
                        {
                            // Manejar retroceso (Backspace)
                            nombre.pop_back();
                        }
                        else if (event.text.unicode < 128 && event.text.unicode != 8)
                        {
                            nombre += static_cast<char>(event.text.unicode);
                        }
                    }
                }

                // Actualizar la pantalla con el nombre ingresado
                ventana.ObtenerVentana().clear();
                ventana.ObtenerVentana().draw(textoRecord);
                ventana.ObtenerVentana().draw(textoInstruccion);
                ventana.ObtenerVentana().draw(recuadroNombre);

                sf::Text textoNombre;
                textoNombre.setFont(fuente);
                textoNombre.setString(nombre);
                textoNombre.setCharacterSize(28);
                textoNombre.setFillColor(sf::Color::Yellow);
                textoNombre.setStyle(sf::Text::Italic);
                textoNombre.setPosition(ventana.ObtenerVentana().getSize().x / 2 - textoNombre.getLocalBounds().width / 2, ventana.ObtenerVentana().getSize().y / 2 + 10);
                ventana.ObtenerVentana().draw(textoNombre);
                ventana.ObtenerVentana().display();
            }

            // Guardar el nuevo récord y mantener los 5 mejores
            std::vector<std::pair<int, std::string>> records;
            std::ifstream archivoRecordIn("record.txt");
            if (archivoRecordIn.is_open())
            {
                int puntos;
                std::string nombreExistente;
                while (archivoRecordIn >> puntos && std::getline(archivoRecordIn, nombreExistente))
                {
                    if (!nombreExistente.empty() && nombreExistente[0] == ' ')
                    {
                        nombreExistente = nombreExistente.substr(1);
                    }
                    records.emplace_back(puntos, nombreExistente);
                }
                archivoRecordIn.close();
            }
            records.emplace_back(puntaje, nombre);
            std::sort(records.begin(), records.end(), [](const std::pair<int, std::string> &a, const std::pair<int, std::string> &b)
                      { return a.first > b.first; });
            if (records.size() > 5)
            {
                records.resize(5); // Mantener solo los 5 mejores
            }
            std::ofstream archivoRecordOut("record.txt");
            if (archivoRecordOut.is_open())
            {
                for (const auto &record : records)
                {
                    archivoRecordOut << record.first << " " << record.second << "\n";
                }
                archivoRecordOut.close();
            }
        }

        // Reiniciar el estado del juego
        gameOver = false;
        puntaje = 0;
        nivel = 1;

        // Liberar memoria de piezas anteriores
        if (piezaActual)
        {
            delete piezaActual;
            piezaActual = nullptr;
        }
        if (proximaPieza)
        {
            delete proximaPieza;
            proximaPieza = nullptr;
        }

        // Crear nuevas piezas
        piezaActual = new Pieza(rand() % 7);
        proximaPieza = new Pieza(rand() % 7);

        // Reiniciar el tablero
        tablero = Tablero();

        // Reiniciar el ciclo del juego
        Jugar();
    }
}

// Declaración de la función mostrarRecords
void Juego::MostrarRecords()
{
    sf::RenderWindow ventanaRecords(sf::VideoMode(800, 600), "Records - Tetris");
    ventanaRecords.setFramerateLimit(60);

    // Cargar la fuente Pixel.ttf
    sf::Font fuente;
    if (!fuente.loadFromFile("assets/fonts/Pixel.ttf"))
    {
        std::cerr << "Error: No se pudo cargar la fuente Pixel.ttf" << std::endl;
        return;
    }

    // Variables para los récords
    std::vector<std::pair<int, std::string>> records;
    int recordActual = 0;

    // Leer los récords del archivo
    std::ifstream archivoRecord("record.txt");
    if (archivoRecord.is_open())
    {
        int puntaje;
        std::string nombre;
        while (archivoRecord >> puntaje >> nombre)
        {
            records.push_back(std::make_pair(puntaje, nombre));
        }
        archivoRecord.close();
    }

    // Ordenar los récords de mayor a menor
    std::sort(records.begin(), records.end(), std::greater<std::pair<int, std::string>>());

    // Obtener el récord actual si existe
    if (!records.empty())
    {
        recordActual = records[0].first;
    }

    // Bucle principal de la ventana de récords
    while (ventanaRecords.isOpen())
    {
        sf::Event event;
        while (ventanaRecords.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                ventanaRecords.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    ventanaRecords.close();
                }
            }
        }

        ventanaRecords.clear(sf::Color(0, 0, 0));

        // Dibujar el título "RECORDS"
        sf::Text textoTitulo;
        textoTitulo.setFont(fuente);
        textoTitulo.setString("RECORDS");
        textoTitulo.setCharacterSize(48);
        textoTitulo.setFillColor(sf::Color::White);
        textoTitulo.setStyle(sf::Text::Bold);
        sf::FloatRect boundsTitulo = textoTitulo.getLocalBounds();
        textoTitulo.setOrigin(boundsTitulo.width / 2, boundsTitulo.height / 2);
        textoTitulo.setPosition(ventanaRecords.getSize().x / 2, 50);
        ventanaRecords.draw(textoTitulo);

        // Dibujar el récord actual
        sf::Text textoRecordActual;
        textoRecordActual.setFont(fuente);
        textoRecordActual.setString("Tu récord: " + std::to_string(recordActual));
        textoRecordActual.setCharacterSize(24);
        textoRecordActual.setFillColor(sf::Color::Yellow);
        textoRecordActual.setPosition(50, 150);
        ventanaRecords.draw(textoRecordActual);

        // Dibujar la lista de récords
        for (size_t i = 0; i < records.size(); ++i)
        {
            const auto &[puntaje, nombre] = records[i];
            sf::Text textoRecord;
            textoRecord.setFont(fuente);
            textoRecord.setString(std::to_string(i + 1) + ". " + nombre + " - " + std::to_string(puntaje));
            textoRecord.setCharacterSize(24);
            textoRecord.setFillColor(sf::Color::White);
            textoRecord.setPosition(ventanaRecords.getSize().x / 2 - textoRecord.getLocalBounds().width / 2, ventanaRecords.getSize().y / 2 - textoRecord.getLocalBounds().height / 2 + 50 + i * 30); // Centrar horizontal y verticalmente los puntajes con desplazamiento
            ventanaRecords.draw(textoRecord);
        }

        ventanaRecords.display();
    }
}