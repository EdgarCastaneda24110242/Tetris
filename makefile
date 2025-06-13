CXX = g++
CXXFLAGS = -std=c++11 -Iinclude

SRCDIR = src
INCDIR = include
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))
TARGET = $(BINDIR)/tetris

all: $(BINDIR) $(TARGET)

$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

runtetrix: all
	$(TARGET)

clean:
	rm -rf $(BINDIR)/*.o $(TARGET)
