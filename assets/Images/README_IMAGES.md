# Imágenes para el Manual de Usuario

## opcion-jugar.png
Esta imagen debe mostrar la opción "Jugar" resaltada en el menú principal del juego.

**Características de la imagen esperada:**
- Captura de pantalla del menú principal
- La opción "Jugar" debe estar resaltada en **color amarillo**
- Debe mostrar el cursor o indicador visual señalando la opción "Jugar"
- Fondo negro con piezas de Tetris animadas
- Título "TETRIS" visible en la parte superior
- Créditos "by Fer and Edgar" visibles

**Ubicación en el README:**
Esta imagen se usa en la sección "Manual de Usuario > Iniciando una Partida" para mostrar visualmente cómo seleccionar la opción para comenzar a jugar.

## pantalla-juego.png
Esta imagen debe mostrar la interfaz principal del juego durante una partida activa.

**Características de la imagen esperada:**
- Captura de pantalla durante el gameplay
- **Tablero principal** en el centro con piezas colocadas
- **Panel lateral derecho** visible con:
  - Puntuación actual
  - Nivel actual
  - Vista previa de la pieza siguiente
- Algunas líneas ya formadas o en proceso
- Una pieza activa cayendo o ya posicionada

**Ubicación en el README:**
Esta imagen se usa en la sección "Manual de Usuario > Pantalla de Juego" para explicar la interfaz durante el gameplay.

## opcion-records.png
Esta imagen debe mostrar la pantalla de récords con los 5 mejores puntajes alcanzados.

**Características de la imagen esperada:**
- Captura de pantalla de la pantalla de récords/scores completa
- **Los 5 mejores puntajes** claramente visibles y ordenados de mayor a menor
- **Puntuaciones reflejadas** que muestran los logros históricos del juego
- **Interfaz limpia** con números grandes y fáciles de leer
- **Título/encabezado** que indique "Records", "Scores" o "Mejores Puntuaciones"
- **Navegación visible** para regresar al menú principal
- **Fondo consistente** con el estilo visual del juego

**Propósito de la imagen:**
Esta captura muestra donde se **reflejan las mejores puntuaciones** obtenidas en todas las partidas jugadas, permitiendo a los usuarios:
- Ver sus logros históricos
- Establecer metas para futuras partidas
- Comparar su progreso a lo largo del tiempo
- Motivarse para superar récords anteriores

**Ubicación en el README:**
Esta imagen se usa en la sección "Manual de Usuario > Pantalla de Récords" para demostrar visualmente cómo y dónde se muestran las mejores puntuaciones del juego.

## pantalla-pausa.png
Esta imagen debe mostrar el menú de pausa que aparece cuando el usuario presiona la tecla Q durante una partida.

**Características de la imagen esperada:**
- Captura de pantalla del **menú de pausa** superpuesto al juego
- **Tablero de fondo** visible pero opacado/difuminado para indicar que el juego está pausado
- **Opciones claramente visibles**:
  - [Q] Reanudar
  - [R] Reiniciar  
  - [M] SALIR
- **Controles indicados** con las teclas correspondientes entre corchetes
- **Overlay/ventana modal** que destaque las opciones sobre el fondo del juego
- **Estilo visual consistente** con el resto del juego

**Propósito de la imagen:**
Esta captura muestra la **funcionalidad de pausa** que permite a los usuarios:
- Tomar un descanso sin perder el progreso
- Acceder a opciones de control del juego (reanudar, reiniciar, salir)
- Planificar estrategia sin presión de tiempo
- Manejar interrupciones durante el gameplay

**Ubicación en el README:**
Esta imagen se usa en la sección "Manual de Usuario > Pantalla de Pausa" para explicar cómo funciona el sistema de pausa y qué opciones están disponibles.

**Referencias en README.md:**
```markdown
![Opción para empezar a jugar](assets/images/opcion-jugar.png)
![Pantalla de juego](assets/images/pantalla-juego.png)
![Opción Records](assets/images/opcion-records.png)
![Pantalla de Pausa](assets/images/pantalla-pausa.png)
```

## Instrucciones para agregar las imágenes:
1. **opcion-jugar.png**: Captura del menú principal con "Jugar" seleccionado (resaltado en amarillo)
2. **pantalla-juego.png**: Captura durante el gameplay mostrando tablero y panel lateral
3. **opcion-records.png**: Captura de la pantalla de récords mostrando los 5 mejores puntajes
4. **pantalla-pausa.png**: Captura del menú de pausa mostrando las opciones disponibles
5. Guarda todas las imágenes en esta carpeta (`assets/images/`)
6. Las referencias en el README.md ya están configuradas y mostrarán las imágenes automáticamente
