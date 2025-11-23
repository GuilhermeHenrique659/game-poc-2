CXX      = g++
CXXFLAGS = -std=c++20 -Iinclude
LDFLAGS  = -Llib

# Windows (MinGW)
LDLIBS   = -lraylib -lopengl32 -lgdi32 -lwinmm -lenet -lws2_32

# Linux → descomente a linha abaixo e comente a de cima
# LDLIBS   = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# macOS → descomente se estiver no Mac
# LDLIBS   = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

SRC      = $(wildcard src/*.cpp src/**/*.cpp src/**/**/*.cpp)
OBJ_DIR  = build
OBJ      = $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)
EXE      = jogo

all: $(EXE)

$(EXE): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)
	@echo "Compilou! Execute com: ./$(EXE)"

# Regra genérica para compilar .cpp → .o dentro de build/
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)               # cria subpastas dentro de build/ automaticamente
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXE) *.exe

run: all
	./$(EXE)

.PHONY: all clean run