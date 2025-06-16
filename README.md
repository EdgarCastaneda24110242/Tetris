# Tetris

Este es un juego de Tetris desarrollado en C++ utilizando la biblioteca SFML. El objetivo del juego es alinear bloques para completar líneas y obtener la mayor cantidad de puntos posible.

## Características

- **Jugabilidad clásica de Tetris**: Mueve, rota y coloca las piezas para completar líneas.
- **Sistema de récords**: Guarda y muestra los 5 mejores puntajes.
- **Menú principal**: Navega entre las opciones de jugar, ver récords y salir.
- **Menú de pausa**: Permite reanudar, reiniciar o salir del juego.
- **Interfaz visual mejorada**: Título y puntajes centrados, colores destacados para opciones seleccionadas.

## Controles

- **Flechas izquierda/derecha**: Mover la pieza.
- **Flecha arriba**: Rotar la pieza.
- **Flecha abajo**: Acelerar la caída de la pieza.
- **Espacio**: Colocar la pieza instantáneamente.
- **P**: Pausar el juego.
- **Enter**: Seleccionar opción en los menús.

## Requisitos

- **Sistema operativo**: Windows, Linux o macOS.
- **Compilador**: Compatible con C++11 o superior.
- **Biblioteca SFML**: Versión 2.5 o superior.

## Instalación y ejecución

1. Clona este repositorio:
   ```bash
   git clone <URL_DEL_REPOSITORIO>
   ```
2. Navega al directorio del proyecto:
   ```bash
   cd Tetris
   ```
3. Compila el proyecto utilizando el `makefile`:
   ```bash
   make
   ```
4. Ejecuta el juego:
   ```bash
   ./bin/tetris.exe
   ```

## Archivos importantes

- **src/**: Contiene el código fuente del juego.
- **include/**: Contiene los archivos de cabecera.
- **assets/**: Contiene las fuentes y música del juego.
- **record.txt**: Archivo donde se almacenan los récords.

## Créditos

Desarrollado por Fer y Edgar.

## Licencia

Este proyecto se distribuye bajo la licencia MIT. Consulta el archivo `LICENSE` para más detalles.
