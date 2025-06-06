# Directorios de origen y destino
SRC_DIR := sources
BIN_DIR := bin

# Detectar el sistema operativo
ifeq ($(OS),Windows_NT)
    EXE_EXT := .exe
    RM := del /Q
    RUN := $(BIN_DIR)/%$(EXE_EXT)
else
    EXE_EXT :=
    RM := rm -f
    RUN := ./$(BIN_DIR)/%
    # Si usas Homebrew para SFML, usa estos flags:
    SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d
endif

# Obtener todos los archivos .cpp en el directorio de origen
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# Generar los nombres de los archivos ejecutables en el directorio de destino
EXE_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%$(EXE_EXT),$(CPP_FILES))

# Regla para compilar cada archivo .cpp y generar el archivo ejecutable correspondiente
$(BIN_DIR)/%$(EXE_EXT): $(SRC_DIR)/%.cpp
	g++ -std=c++17 $< -o $@ $(SFML) -Iinclude

# Regla por defecto para compilar todos los archivos .cpp
all: $(EXE_FILES)

# Regla para ejecutar cada archivo ejecutable
run%: $(BIN_DIR)/%$(EXE_EXT)
	$(BIN_DIR)/$*$(EXE_EXT)

# Regla para limpiar los archivos generados
clean:
	$(RM) $(EXE_FILES)

.PHONY: all clean
.PHONY: run-%
