CXX      = g++
CXXFLAGS = -std=c++20 -g -O0 -Wall -Iinclude
LDFLAGS  = -Llib

# Windows (MinGW)
LDLIBS   = -lraylib  -lopengl32 -lgdi32 -lwinmm -lenet -ltmxlite -lws2_32 

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
	@echo "Para depurar: gdb ./$(EXE)"

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(EXE) *.exe

run: all
	./$(EXE)

.PHONY: all clean run
