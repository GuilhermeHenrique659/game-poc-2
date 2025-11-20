# Makefile mais simples do mundo pra raylib + C++ (2025)

CXX      = g++
CXXFLAGS = -std=c++20 -Iinclude
LDFLAGS  = -Llib

# Windows (MinGW)
LDLIBS   = -lraylib -lopengl32 -lgdi32 -lwinmm

# Linux → descomente a linha abaixo e comente a de cima
# LDLIBS   = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# macOS → descomente se estiver no Mac
# LDLIBS   = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC = $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp)
OBJ = $(SRC:.cpp=.o)
EXE = jogo

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)
	@echo "Compilou! Execute com: ./$(EXE)"

clean:
	rm -f $(OBJ) $(EXE) *.exe

run: all
	./$(EXE)

.PHONY: all clean run