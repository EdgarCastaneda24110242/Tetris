# Tetris

Un juego de **Tetris** clásico desarrollado en **C++** utilizando la biblioteca **SFML**. Disfruta de la experiencia original de Tetris con gráficos modernos, efectos de sonido inmersivos y un sistema de récords para competir contigo mismo.

## Tabla de Contenidos

- [Descripción del Juego](#descripción-del-juego)
- [Características](#características)
- [Controles](#controles)
- [Menús del Juego](#menús-del-juego)
- [Requisitos del Sistema](#requisitos-del-sistema)
- [Instalación y Configuración](#instalación-y-configuración)
- [Compilación y Ejecución](#compilación-y-ejecución)
- [Instrucciones Específicas para macOS](#instrucciones-específicas-para-macos)
- [Desarrollo](#desarrollo)
- [Solución de Problemas](#solución-de-problemas)

## Descripción del Juego

Tetris es un juego de rompecabezas donde el objetivo es **alinear bloques** de diferentes formas para completar líneas horizontales y obtener la mayor cantidad de **puntos** posible. Las piezas caen desde la parte superior del tablero y debes rotar y posicionar estratégicamente cada pieza para evitar que el tablero se llene.

### Reglas del Juego

- Las **tetrominós** (piezas de 4 bloques) caen desde arriba
- Completa líneas horizontales para eliminarlas y ganar puntos
- El juego termina cuando las piezas alcanzan la parte superior
- Mayor velocidad = mayor puntuación

## Características

- **Jugabilidad clásica de Tetris**: Mecánicas auténticas y fluidas
- **Sistema de récords**: Almacena y muestra los 5 mejores puntajes localmente
- **Banda sonora inmersiva**: Música de fondo y efectos de sonido
- **Interfaz visual moderna**: Gráficos limpios con fuentes personalizadas
- **Sistema de pausa**: Pausa y reanuda en cualquier momento
- **Menú principal interactivo**: Navegación intuitiva entre opciones
- **Pantalla de Game Over**: Visualización de estadísticas finales
- **7 tipos de tetrominós**: Todas las piezas clásicas (I, O, T, S, Z, J, L)
- **Efectos visuales**: Animaciones de líneas completadas con parpadeo
- **Fondo animado**: Piezas de fondo en movimiento en el menú principal

## Controles

### Durante el Juego
| Tecla | Acción |
|-------|--------|
| **A** | Mover pieza hacia la izquierda |
| **D** | Mover pieza hacia la derecha |
| **S** | Acelerar caída de la pieza |
| **W** | Rotar pieza en sentido horario |
| **Q** | Pausar/Reanudar juego |

### En el Menú Principal
| Tecla | Acción |
|-------|--------|
| **W / ↑** | Navegar hacia arriba |
| **S / ↓** | Navegar hacia abajo |
| **Enter** | Seleccionar opción |

### En el Menú de Pausa
| Tecla | Acción |
|-------|--------|
| **Q** | Reanudar juego |
| **R** | Reiniciar partida |
| **M** | Salir del juego |

## Menús del Juego

### Menú Principal
El menú principal incluye las siguientes opciones:
- **Jugar**: Inicia una nueva partida
- **Scores**: Muestra los 5 mejores puntajes guardados
- **Salir**: Cierra el juego

El menú cuenta con:
- Fondo animado con tetrominós cayendo
- Música ambiente específica para el menú
- Navegación con teclas W/S o flechas arriba/abajo
- Efectos visuales para la opción seleccionada

### Menú de Pausa
Durante el juego, presiona **Q** para acceder al menú de pausa con las opciones:
- **[Q] Reanudar**: Continúa la partida actual
- **[R] Reiniciar**: Comienza una nueva partida desde cero
- **[M] SALIR**: Termina el juego y cierra la aplicación

Características del menú de pausa:
- La música se pausa automáticamente
- El estado del juego se mantiene intacto al reanudar
- Reiniciar reproduce un sonido especial de inicio

### Pantalla de Récords
Muestra los 5 mejores puntajes alcanzados, guardados automáticamente en el archivo `record.txt`.

## Requisitos del Sistema

### Sistemas Operativos Soportados
- **Windows** 10/11
- **Linux** (Ubuntu 18.04+, Fedora 30+, etc.)
- **macOS** 10.15+ (Catalina o superior)

### Herramientas de Desarrollo
- **Compilador C++**: Compatible con C++11 o superior
  - Linux: `g++` 7.0+
  - macOS: `clang++` (Xcode Command Line Tools)
  - Windows: `g++` (MinGW) o Visual Studio 2017+
- **Make**: Para ejecutar el sistema de compilación

### Librerías Necesarias
- **SFML 2.5+** (Simple and Fast Multimedia Library)
  - `sfml-graphics`
  - `sfml-window`  
  - `sfml-system`
  - `sfml-audio`

## Instalación y Configuración

### 1. Clonar el Repositorio
```bash
git clone <URL_DEL_REPOSITORIO>
cd Tetris
```

### 2. Instalar Dependencias

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential libsfml-dev
```

#### Linux (Fedora/CentOS)
```bash
sudo dnf install gcc-c++ make SFML-devel
# o en CentOS/RHEL:
sudo yum install gcc-c++ make SFML-devel
```

#### macOS
```bash
# Usando Homebrew (recomendado)
brew install sfml

# O usando MacPorts
sudo port install sfml +universal
```

#### Windows
1. Descarga SFML desde [sfml-dev.org](https://www.sfml-dev.org/download.php)
2. Extrae en `C:\SFML` o similar
3. Instala MinGW o Visual Studio

## Compilación y Ejecución

### Compilación Rápida
```bash
make
```

### Ejecutar el Juego
```bash
# En Linux/macOS
./bin/tetris

# En Windows
./bin/tetris.exe
```

### Comandos del Makefile
```bash
make            # Compilar todo el proyecto
make clean      # Limpiar archivos objeto y ejecutable
make runtetrix  # Compilar y ejecutar automáticamente
```

## Instrucciones Específicas para macOS

### Instalación de Dependencias en macOS

#### Opción 1: Homebrew (Recomendado)
```bash
# Instalar Homebrew si no lo tienes
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instalar SFML
brew install sfml

# Verificar instalación
brew list sfml
```

#### Opción 2: Compilar SFML desde el código fuente
```bash
# Descargar SFML
curl -O https://www.sfml-dev.org/files/SFML-2.6.1-sources.zip
unzip SFML-2.6.1-sources.zip
cd SFML-2.6.1

# Compilar e instalar
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make
sudo make install
```

### Verificación de la Instalación
```bash
# Verificar que g++ esté disponible
g++ --version

# Verificar que SFML esté instalada correctamente
pkg-config --libs sfml-all
```

### Solución de Problemas en macOS

#### Error: "library not found for -lsfml-*"
```bash
# Configurar variables de entorno
export CPPFLAGS="-I/usr/local/include"
export LDFLAGS="-L/usr/local/lib"

# O si usas Homebrew
export CPPFLAGS="-I$(brew --prefix)/include"
export LDFLAGS="-L$(brew --prefix)/lib"
```

#### Permisos de Ejecución
```bash
chmod +x bin/tetris
```

## Desarrollo

### Estructura del Proyecto
```
Tetris/
├── src/                 # Código fuente (.cpp)
├── include/            # Archivos de cabecera (.hpp)
├── assets/             # Recursos del juego
│   ├── fonts/         # Fuentes tipográficas
│   └── music/         # Archivos de audio
├── bin/               # Ejecutables y archivos objeto
├── docs/              # Documentación y diagramas UML
├── makefile           # Sistema de compilación
└── record.txt         # Archivo de récords
```

### Clases Principales
- **`Tetris`**: Clase principal del juego y control de estados
- **`Juego`**: Lógica principal del gameplay
- **`Tablero`**: Gestión del área de juego y detección de líneas
- **`Pieza`**: Representación y manipulación de tetrominós
- **`Audio`** y **`Musica`**: Sistema de sonido
- **`Ventana`**: Gestión de la ventana y renderizado
- **`Puntos`**: Sistema de puntuación y récords

### Compilación de Desarrollo
```bash
# Compilación con información de debug
g++ -std=c++11 -g -Iinclude src/*.cpp -o bin/tetris_debug \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Verificar dependencias dinámicas (macOS)
otool -L bin/tetris

# Verificar dependencias dinámicas (Linux)
ldd bin/tetris
```

## Solución de Problemas

### Errores Comunes

#### 1. "SFML not found" o "library not found"
**Solución**:
```bash
# macOS con Homebrew
export PKG_CONFIG_PATH="$(brew --prefix)/lib/pkgconfig"

# Linux
sudo ldconfig
```

#### 2. "Permission denied" al ejecutar
**Solución**:
```bash
chmod +x bin/tetris
```

#### 3. "No such file or directory: assets/"
**Solución**: Asegúrate de ejecutar desde el directorio raíz del proyecto:
```bash
cd /ruta/al/proyecto/Tetris
./bin/tetris
```

#### 4. Audio no funciona en Linux
**Solución**: Instalar dependencias de audio:
```bash
sudo apt install libopenal1 libopenal-dev libvorbis-dev libflac-dev
```

#### 5. Problemas de renderizado en macOS
**Solución**: Actualizar drivers gráficos y verificar compatibilidad con OpenGL:
```bash
# Verificar versión de OpenGL
system_profiler SPDisplaysDataType | grep OpenGL
```

### Logs y Debugging
Para obtener información de debug, puedes modificar temporalmente el código para mostrar información adicional o usar herramientas como `gdb`:

```bash
# Ejecutar con gdb
gdb ./bin/tetris
(gdb) run
(gdb) bt  # backtrace en caso de crash
```

### Rendimiento
- El juego está optimizado para 60 FPS
- Uso de memoria típico: ~50MB
- CPU: Mínimo requerido para gráficos 2D

## Archivos Importantes

| Archivo/Directorio | Descripción |
|-------------------|-------------|
| **`src/`** | Código fuente principal del juego (.cpp) |
| **`include/`** | Archivos de cabecera (.hpp) |
| **`assets/fonts/`** | Fuentes tipográficas (Pixel.ttf, tetrominoes.ttf) |
| **`assets/music/`** | Archivos de audio (.ogg) |
| **`bin/`** | Ejecutables y archivos objeto compilados |
| **`docs/`** | Documentación y diagramas UML |
| **`record.txt`** | Archivo donde se almacenan los récords |
| **`makefile`** | Sistema de compilación automatizada |
| **`LICENSE`** | Términos de licencia del proyecto |

## Recursos de Audio

El juego incluye los siguientes efectos de sonido y música:

- **`Tetris.ogg`**: Música principal durante el juego
- **`TetrisMenu.ogg`**: Música del menú principal  
- **`EmpezarJuego.ogg`**: Sonido al iniciar una partida
- **`MovePieza.ogg`**: Efecto al mover piezas
- **`HitPieza.ogg`**: Sonido al colocar una pieza
- **`LineaPunto.ogg`**: Efecto al completar una línea
- **`GameOver.ogg`**: Sonido de fin de partida

## Fuentes Personalizadas

- **`Pixel.ttf`**: Fuente principal con estilo pixel art
- **`tetrominoes.ttf`**: Fuente especial para elementos del juego

## Sistema de Récords

El archivo `record.txt` almacena los 5 mejores puntajes en formato:
```
Puntuación1
Puntuación2
Puntuación3
Puntuación4
Puntuación5
```

## Contribuciones

¡Las contribuciones son bienvenidas! Si deseas mejorar el juego:

1. Fork el repositorio
2. Crea una rama para tu feature (`git checkout -b feature/nueva-caracteristica`)
3. Commit tus cambios (`git commit -am 'Agrega nueva característica'`)
4. Push a la rama (`git push origin feature/nueva-caracteristica`)
5. Crea un Pull Request

## Créditos

**Desarrolladores**: Fer y Edgar

**Librerías utilizadas**:
- [SFML](https://www.sfml-dev.org/) - Simple and Fast Multimedia Library

## Licencia

Este proyecto se distribuye bajo la **Licencia MIT**. Consulta el archivo [`LICENSE`](LICENSE) para más detalles.

---

### ¡Que disfrutes jugando Tetris!

Si encuentras algún problema o tienes sugerencias, no dudes en abrir un [issue](../../issues) en el repositorio.

**¡A por esos récords!**
